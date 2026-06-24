//  Copyright 2026 Kritik Agarwal
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

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