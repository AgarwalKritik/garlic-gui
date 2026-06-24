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

#include "ProjectManager.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>
#include <QDebug>

ProjectManager::ProjectManager(QObject *parent)
    : QObject(parent)
{
}

bool ProjectManager::saveProject(const QString &projectPath)
{
    if (projectPath.isEmpty())
        return false;

    // Create project metadata file
    if (createProjectMetadata(projectPath))
    {
        emit projectSaved(projectPath);
        return true;
    }

    return false;
}

bool ProjectManager::exportProject(const QString &projectPath, const QString &exportPath)
{
    if (projectPath.isEmpty() || exportPath.isEmpty())
        return false;

    QDir exportDir(exportPath);
    QFileInfo projectInfo(projectPath);
    QString targetDir = exportDir.absoluteFilePath(projectInfo.baseName() + "_exported");

    if (copyDirectoryRecursively(projectPath, targetDir))
    {
        emit projectExported(targetDir);
        return true;
    }

    return false;
}

bool ProjectManager::loadProject(const QString &projectPath)
{
    QDir projectDir(projectPath);
    if (!projectDir.exists())
        return false;

    emit projectLoaded(projectPath);
    return true;
}

bool ProjectManager::copyDirectoryRecursively(const QString &sourceDir, const QString &targetDir)
{
    QDir source(sourceDir);
    if (!source.exists())
        return false;

    QDir target;
    if (!target.mkpath(targetDir))
        return false;

    QDir targetFolder(targetDir);

    // Copy all files
    QStringList files = source.entryList(QDir::Files);
    foreach (const QString &fileName, files)
    {
        QString sourcePath = source.absoluteFilePath(fileName);
        QString targetPath = targetFolder.absoluteFilePath(fileName);

        if (QFile::exists(targetPath))
        {
            QFile::remove(targetPath);
        }

        if (!QFile::copy(sourcePath, targetPath))
        {
            qDebug() << "Failed to copy file:" << sourcePath << "to" << targetPath;
            return false;
        }
    }

    // Recursively copy subdirectories
    QStringList dirs = source.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    foreach (const QString &dirName, dirs)
    {
        QString sourceSubDir = source.absoluteFilePath(dirName);
        QString targetSubDir = targetFolder.absoluteFilePath(dirName);

        if (!copyDirectoryRecursively(sourceSubDir, targetSubDir))
        {
            return false;
        }
    }

    return true;
}

bool ProjectManager::createProjectMetadata(const QString &projectPath)
{
    QJsonObject metadata;
    metadata["version"] = "1.0";
    metadata["created"] = QDateTime::currentDateTime().toString(Qt::ISODate);
    metadata["tool"] = "Garlic Decompiler";

    QJsonDocument doc(metadata);

    QString metadataPath = QDir(projectPath).absoluteFilePath(".project_info");
    QFile file(metadataPath);

    if (file.open(QIODevice::WriteOnly))
    {
        file.write(doc.toJson());
        file.close();
        return true;
    }

    return false;
}