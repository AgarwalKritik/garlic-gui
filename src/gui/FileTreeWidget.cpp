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

#include "FileTreeWidget.h"
#include <QDir>
#include <QFileInfo>
#include <QHeaderView>
#include <QIcon>

FileTreeWidget::FileTreeWidget(QWidget *parent)
    : QWidget(parent), m_layout(nullptr), m_titleLabel(nullptr), m_searchEdit(nullptr), m_treeWidget(nullptr)
{
    setupUI();
}

void FileTreeWidget::setupUI()
{
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(5, 5, 5, 5);

    // Title
    m_titleLabel = new QLabel("EXPLORER");
    m_titleLabel->setStyleSheet("QLabel { font-size: 11px; color: #BBBBBB; letter-spacing: 1px; padding: 5px 0px 5px 15px; }");
    m_layout->addWidget(m_titleLabel);

    // Search box
    m_searchEdit = new QLineEdit();
    m_searchEdit->setPlaceholderText("Search files...");
    m_searchEdit->setStyleSheet(
        "QLineEdit { "
        "  padding: 4px; "
        "  border: 1px solid #202122; "
        "  background-color: #202122; "
        "  color: #CCCCCC; "
        "} "
        "QLineEdit:focus { "
        "  border: 1px solid #3994BC; "
        "}");
    connect(m_searchEdit, &QLineEdit::textChanged, this, &FileTreeWidget::onSearchTextChanged);
    m_layout->addWidget(m_searchEdit);

    // Tree widget
    m_treeWidget = new QTreeWidget();
    m_treeWidget->setHeaderHidden(true);
    m_treeWidget->setRootIsDecorated(true);
    m_treeWidget->setAlternatingRowColors(false); // VS Code doesn't alternate
    m_treeWidget->setStyleSheet(
        "QTreeWidget { "
        "  border: none; "
        "  background-color: #191A1B; "
        "  color: #CCCCCC; "
        "} "
        "QTreeWidget::item { "
        "  padding: 3px 0px; "
        "  border: none; "
        "} "
        "QTreeWidget::item:hover { "
        "  background-color: #202122; "
        "} "
        "QTreeWidget::item:selected { "
        "  background-color: #3994BC; "
        "  color: #FFFFFF; "
        "}");

    connect(m_treeWidget, &QTreeWidget::itemClicked, this, &FileTreeWidget::onItemClicked);
    m_layout->addWidget(m_treeWidget);

    setLayout(m_layout);
}

void FileTreeWidget::loadProject(const QString &projectPath, const QString &projectName)
{
    m_currentProjectPath = projectPath;
    m_treeWidget->clear();

    if (!projectPath.isEmpty() && QDir(projectPath).exists())
    {
        populateTree(projectPath, projectName);
        m_treeWidget->expandToDepth(1);
    }
}

void FileTreeWidget::clearProject()
{
    m_currentProjectPath.clear();
    m_treeWidget->clear();
}

void FileTreeWidget::populateTree(const QString &rootPath, const QString &projectName)
{
    QDir rootDir(rootPath);
    if (!rootDir.exists())
        return;

    QString displayName = projectName.isEmpty() ? rootDir.dirName() : projectName;
    QTreeWidgetItem *rootItem = createTreeItem(displayName, rootPath);
    m_treeWidget->addTopLevelItem(rootItem);

    addDirectoryToTree(rootItem, rootPath);
    rootItem->setExpanded(true);
}

void FileTreeWidget::addDirectoryToTree(QTreeWidgetItem *parentItem, const QString &dirPath)
{
    QDir dir(dirPath);
    QStringList entries = dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::Name);

    foreach (const QString &entry, entries)
    {
        QString fullPath = dir.absoluteFilePath(entry);
        QFileInfo fileInfo(fullPath);

        if (fileInfo.isDir())
        {
            QTreeWidgetItem *dirItem = createTreeItem(entry, fullPath);
            parentItem->addChild(dirItem);
            addDirectoryToTree(dirItem, fullPath);
        }
        else if (isJavaFile(entry))
        {
            QTreeWidgetItem *fileItem = createTreeItem(entry, fullPath, true);
            parentItem->addChild(fileItem);
        }
    }
}

bool FileTreeWidget::isJavaFile(const QString &fileName)
{
    return fileName.endsWith(".java", Qt::CaseInsensitive) ||
           fileName.endsWith(".kt", Qt::CaseInsensitive) ||
           fileName.endsWith(".smali", Qt::CaseInsensitive);
}

QTreeWidgetItem *FileTreeWidget::createTreeItem(const QString &name, const QString &fullPath, bool isFile)
{
    QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, name);
    item->setData(0, Qt::UserRole, fullPath);

    if (isFile)
    {
        item->setIcon(0, style()->standardIcon(QStyle::SP_FileIcon));
    }
    else
    {
        item->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));
    }

    return item;
}

void FileTreeWidget::onItemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)

    if (item)
    {
        QString fullPath = item->data(0, Qt::UserRole).toString();
        QFileInfo fileInfo(fullPath);

        if (fileInfo.isFile())
        {
            emit fileSelected(fullPath);
        }
    }
}

void FileTreeWidget::onSearchTextChanged(const QString &text)
{
    filterTree(text);
}

void FileTreeWidget::filterTree(const QString &searchText)
{
    if (m_treeWidget->topLevelItemCount() == 0)
        return;

    QTreeWidgetItem *rootItem = m_treeWidget->topLevelItem(0);

    if (searchText.isEmpty())
    {
        setItemVisible(rootItem, true);
        return;
    }

    bool hasVisibleChildren = itemMatchesSearch(rootItem, searchText);
    setItemVisible(rootItem, hasVisibleChildren);

    if (hasVisibleChildren)
    {
        m_treeWidget->expandAll();
    }
}

void FileTreeWidget::setItemVisible(QTreeWidgetItem *item, bool visible)
{
    if (!item)
        return;

    item->setHidden(!visible);

    for (int i = 0; i < item->childCount(); ++i)
    {
        QTreeWidgetItem *child = item->child(i);
        bool childVisible = itemMatchesSearch(child, m_searchEdit->text());
        setItemVisible(child, childVisible);
    }
}

bool FileTreeWidget::itemMatchesSearch(QTreeWidgetItem *item, const QString &searchText)
{
    if (!item || searchText.isEmpty())
        return true;

    // Check if item name matches
    if (item->text(0).contains(searchText, Qt::CaseInsensitive))
    {
        return true;
    }

    // Check if any child matches
    for (int i = 0; i < item->childCount(); ++i)
    {
        if (itemMatchesSearch(item->child(i), searchText))
        {
            return true;
        }
    }

    return false;
}