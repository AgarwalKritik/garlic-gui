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

#ifndef FILETREEWIDGET_H
#define FILETREEWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTreeWidgetItem>

class FileTreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileTreeWidget(QWidget *parent = nullptr);

    void loadProject(const QString &projectPath);
    void clearProject();

signals:
    void fileSelected(const QString &filePath);

private slots:
    void onItemClicked(QTreeWidgetItem *item, int column);
    void onSearchTextChanged(const QString &text);

private:
    void setupUI();
    void populateTree(const QString &rootPath);
    void addDirectoryToTree(QTreeWidgetItem *parentItem, const QString &dirPath);
    bool isJavaFile(const QString &fileName);
    QTreeWidgetItem *createTreeItem(const QString &name, const QString &fullPath, bool isFile = false);
    void filterTree(const QString &searchText);
    void setItemVisible(QTreeWidgetItem *item, bool visible);
    bool itemMatchesSearch(QTreeWidgetItem *item, const QString &searchText);

    QVBoxLayout *m_layout;
    QLabel *m_titleLabel;
    QLineEdit *m_searchEdit;
    QTreeWidget *m_treeWidget;
    QString m_currentProjectPath;
};

#endif // FILETREEWIDGET_H