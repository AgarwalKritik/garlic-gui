// ==============================================================================
//               Copyright 2026 Kritik Agarwal
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//          http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// ==============================================================================
//
// File: DecompilerInterface.cpp
// Description: Bridge between the Garlic C-core and Qt6 frontend.
//
// ==============================================================================
#include "DecompilerInterface.h"
#include "../garlic/garlic_wrapper.h"
#include <QDir>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDebug>
#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include <QPointer>
#include <QRegularExpression>
#include <stdio.h>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#define PIPE(fd) _pipe((fd), 4096, _O_TEXT)
#define DUP _dup
#define DUP2 _dup2
#define FILENO _fileno
#define CLOSE _close
#define FDOPEN _fdopen
#else
#include <unistd.h>
#define PIPE(fd) pipe((fd))
#define DUP dup
#define DUP2 dup2
#define FILENO fileno
#define CLOSE close
#define FDOPEN fdopen
#endif

// Static instance for callback
DecompilerInterface *DecompilerInterface::s_instance = nullptr;

// ==============================================================================
// Method: DecompilerInterface::DecompilerInterface
// ==============================================================================
DecompilerInterface::DecompilerInterface(QObject *parent)
    : QObject(parent)
{
    s_instance = this;

    // Register progress callback with Garlic
    garlic_set_progress_callback(DecompilerInterface::progressCallback);
}

// ==============================================================================
// Method: DecompilerInterface::~DecompilerInterface
// ==============================================================================
DecompilerInterface::~DecompilerInterface()
{
    if (s_instance == this)
        s_instance = nullptr;
}

// ==============================================================================
// Method: DecompilerInterface::decompileFile
// ==============================================================================
void DecompilerInterface::decompileFile(const QString &inputPath)
{
    // Validate input file using Garlic's validation
    if (!garlic_is_valid_file(inputPath.toUtf8().constData()))
    {
        emit decompilationFinished(false);
        return;
    }

    emit decompilationStarted();

    // Create output directory using the input file's base name
    m_currentOutputDir = createTempOutputDirectory(inputPath);
    if (m_currentOutputDir.isEmpty())
    {
        emit decompilationFinished(false);
        return;
    }

    QString outputDir = m_currentOutputDir;
    // Match the CLI default of 4 threads to prevent lock contention overhead
    int threadNum = 4;

    // Flush before redirecting
    fflush(stdout);
    fflush(stderr);

    // Set up stdout/stderr redirection
    int pipeFds[2];
    if (PIPE(pipeFds) == -1) {
        emit decompilationFinished(false);
        return;
    }

    // Ensure stdout/stderr have valid file descriptors in Windows GUI apps
#ifdef _WIN32
    if (_fileno(stdout) < 0) freopen("NUL", "w", stdout);
    if (_fileno(stderr) < 0) freopen("NUL", "w", stderr);
#endif

    int fdOut = FILENO(stdout);
    int fdErr = FILENO(stderr);

    int oldStdout = -1;
    int oldStderr = -1;

    if (fdOut >= 0) {
        oldStdout = DUP(fdOut);
        DUP2(pipeFds[1], fdOut);
    }
    if (fdErr >= 0) {
        oldStderr = DUP(fdErr);
        DUP2(pipeFds[1], fdErr);
    }

    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    QPointer<DecompilerInterface> self(this);

    // Spawn reader thread
    QThread *readerThread = QThread::create([self, pipeFds]() {
        int fd = pipeFds[0];
        char buffer[4096];
        int bytesRead;
        QString rawBuffer;
        
        qint64 lastEmitTime = QDateTime::currentMSecsSinceEpoch();

#ifdef _WIN32
        while ((bytesRead = _read(fd, buffer, sizeof(buffer) - 1)) > 0) {
#else
        while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
#endif
            buffer[bytesRead] = '\0';
            rawBuffer.append(QString::fromUtf8(buffer));
            
            qint64 now = QDateTime::currentMSecsSinceEpoch();
            if (now - lastEmitTime > 50 || rawBuffer.endsWith('\n')) {
                if (!rawBuffer.isEmpty() && self) {
                    QString chunk = rawBuffer;

// ==============================================================================
// Method: QMetaObject::invokeMethod
// ==============================================================================
                    QMetaObject::invokeMethod(self, [self, chunk]() {
                        emit self->logMessage(chunk);
                    }, Qt::QueuedConnection);
                    rawBuffer.clear();
                    lastEmitTime = now;
                }
            }
        }
        
        if (!rawBuffer.isEmpty() && self) {
            QString chunk = rawBuffer;

// ==============================================================================
// Method: QMetaObject::invokeMethod
// ==============================================================================
            QMetaObject::invokeMethod(self, [self, chunk]() {
                emit self->logMessage(chunk);
            }, Qt::QueuedConnection);
        }
    });
    connect(readerThread, &QThread::finished, readerThread, &QThread::deleteLater);
    readerThread->start();

    // Run decompilation in a separate thread to avoid blocking the GUI
    QThread *workerThread = QThread::create([self, inputPath, outputDir, threadNum, pipeFds, oldStdout, oldStderr, fdOut, fdErr]() {
        // Call Garlic decompiler
        int result = garlic_decompile_file(
            inputPath.toUtf8().constData(),
            outputDir.toUtf8().constData(),
            threadNum
        );
        
        // Restore stdout/stderr
        fflush(stdout);
        fflush(stderr);
        if (oldStdout >= 0) { DUP2(oldStdout, fdOut); CLOSE(oldStdout); }
        if (oldStderr >= 0) { DUP2(oldStderr, fdErr); CLOSE(oldStderr); }
        
        // Close write end of pipe so the reader thread sees EOF
        CLOSE(pipeFds[1]);
        
        if (self) {
            // Emit result on main thread

// ==============================================================================
// Method: QMetaObject::invokeMethod
// ==============================================================================
            QMetaObject::invokeMethod(self, [self, result]() {
                emit self->decompilationFinished(result == 1);
            }, Qt::QueuedConnection);
        }
    });

    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater);
    workerThread->start();
}

// ==============================================================================
// Method: DecompilerInterface::getOutputDirectory
// ==============================================================================
QString DecompilerInterface::getOutputDirectory() const
{
    return m_currentOutputDir;
}

// ==============================================================================
// Method: DecompilerInterface::progressCallback
// ==============================================================================
void DecompilerInterface::progressCallback(int progress)
{
    if (s_instance)
    {
        // Emit signal on main thread

// ==============================================================================
// Method: QMetaObject::invokeMethod
// ==============================================================================
        QMetaObject::invokeMethod(s_instance, [progress]()
                                  { s_instance->progressUpdated(progress); }, Qt::QueuedConnection);
    }
}

#include <QDateTime>

// ==============================================================================
// Method: DecompilerInterface::createTempOutputDirectory
// ==============================================================================
QString DecompilerInterface::createTempOutputDirectory(const QString &inputPath)
{
    QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss_zzz");

    QString baseName = "Project";
    if (!inputPath.isEmpty())
    {
        baseName = QFileInfo(inputPath).baseName();
        // Remove spaces and special characters for a safe directory name
        baseName.replace(QRegularExpression("[^a-zA-Z0-9_-]"), "_");
    }

    QString outputDir = tempDir + "/" + baseName + "_" + QString::number(QCoreApplication::applicationPid()) + "_" + timestamp;

    QDir dir;
    if (!dir.mkpath(outputDir))
    {
        return QString();
    }

    return outputDir;
}

// ==============================================================================
// Method: DecompilerInterface::getFileTypeString
// ==============================================================================
QString DecompilerInterface::getFileTypeString(const QString &filePath)
{
    const char *typeStr = garlic_get_file_type_string(filePath.toUtf8().constData());
    return typeStr ? QString::fromLocal8Bit(typeStr) : QString();
}
