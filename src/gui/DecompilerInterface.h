#ifndef DECOMPILERINTERFACE_H
#define DECOMPILERINTERFACE_H

#include <QObject>
#include <QString>
#include <QPointer>

class DecompilerInterface : public QObject
{
    Q_OBJECT

public:
    explicit DecompilerInterface(QObject *parent = nullptr);
    ~DecompilerInterface();

    void decompileFile(const QString &inputPath);
    QString getOutputDirectory() const;

    QString getFileTypeString(const QString &filePath);

    static void progressCallback(int progress);

signals:
    void decompilationStarted();
    void decompilationFinished(bool success);
    void progressUpdated(int progress);

private:
    QString createTempOutputDirectory();

    QString m_currentOutputDir;

    static DecompilerInterface *s_instance;
};

#endif // DECOMPILERINTERFACE_H