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

    // Create output directory
    m_currentOutputDir = createTempOutputDirectory();
    if (m_currentOutputDir.isEmpty())
    {
        emit decompilationFinished(false);
        return;
    }

    // Run decompilation in a separate thread to avoid blocking the GUI
    QPointer<DecompilerInterface> self(this);
    QThread *workerThread = QThread::create([self, inputPath]()
                                            {
        // Call Garlic decompiler
        int result = garlic_decompile_file(
            inputPath.toLocal8Bit().constData(),
            self ? self->m_currentOutputDir.toLocal8Bit().constData() : ""
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

QString DecompilerInterface::createTempOutputDirectory()
{
    QString tempDir = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString outputDir = tempDir + "/GarlicGUI_" + QString::number(QCoreApplication::applicationPid());

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
