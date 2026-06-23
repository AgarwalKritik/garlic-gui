/*
 * Copyright 2025 Kritik Agarwal
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

// Static instance for callback
DecompilerInterface *DecompilerInterface::s_instance = nullptr;

DecompilerInterface::DecompilerInterface(QObject *parent)
    : QObject(parent)
{
    s_instance = this;

    // Register progress callback with Garlic
    garlic_set_progress_callback(DecompilerInterface::progressCallback);
}

DecompilerInterface::~DecompilerInterface()
{
    if (s_instance == this)
        s_instance = nullptr;
}

void DecompilerInterface::decompileFile(const QString &inputPath)
{
    // Validate input file using Garlic's validation
    if (!garlic_is_valid_file(inputPath.toLocal8Bit().constData()))
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

    // Run decompilation in a separate thread to avoid blocking the GUI
    QPointer<DecompilerInterface> self(this);
    QThread *workerThread = QThread::create([self, inputPath, outputDir, threadNum]()
                                            {
        // Call Garlic decompiler
        int result = garlic_decompile_file(
            inputPath.toLocal8Bit().constData(),
            outputDir.toLocal8Bit().constData(),
            threadNum
        );
        if (self) {
            // Emit result on main thread
            QMetaObject::invokeMethod(self, [self, result]() {
                emit self->decompilationFinished(result == 1);
            }, Qt::QueuedConnection);
        } });

    connect(workerThread, &QThread::finished, workerThread, &QThread::deleteLater);
    workerThread->start();
}

QString DecompilerInterface::getOutputDirectory() const
{
    return m_currentOutputDir;
}

void DecompilerInterface::progressCallback(int progress)
{
    if (s_instance)
    {
        // Emit signal on main thread
        QMetaObject::invokeMethod(s_instance, [progress]()
                                  { s_instance->progressUpdated(progress); }, Qt::QueuedConnection);
    }
}

#include <QDateTime>

QString DecompilerInterface::createTempOutputDirectory(const QString &inputPath)
{
    QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss_zzz");
    
    QString baseName = "Project";
    if (!inputPath.isEmpty()) {
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

QString DecompilerInterface::getFileTypeString(const QString &filePath)
{
    const char *typeStr = garlic_get_file_type_string(filePath.toLocal8Bit().constData());
    return typeStr ? QString::fromLocal8Bit(typeStr) : QString();
}
