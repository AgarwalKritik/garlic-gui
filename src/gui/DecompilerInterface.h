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
    void logMessage(const QString &message);

private:
    QString createTempOutputDirectory(const QString &inputPath);

    QString m_currentOutputDir;

    static DecompilerInterface *s_instance;
};

#endif // DECOMPILERINTERFACE_H