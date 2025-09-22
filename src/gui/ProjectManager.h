#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QObject>
#include <QString>

class ProjectManager : public QObject
{
    Q_OBJECT

public:
    explicit ProjectManager(QObject *parent = nullptr);

    bool saveProject(const QString &projectPath);
    bool exportProject(const QString &projectPath, const QString &exportPath);
    bool loadProject(const QString &projectPath);

signals:
    void projectSaved(const QString &projectPath);
    void projectExported(const QString &exportPath);
    void projectLoaded(const QString &projectPath);

private:
    bool copyDirectoryRecursively(const QString &sourceDir, const QString &targetDir);
    bool createProjectMetadata(const QString &projectPath);
};

#endif // PROJECTMANAGER_H