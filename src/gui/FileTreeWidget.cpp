#include "FileTreeWidget.h"
#include <QDir>
#include <QFileInfo>
#include <QCoreApplication>
#include <QHeaderView>
#include <QIcon>

FileTreeWidget::FileTreeWidget(QWidget *parent)
    : QWidget(parent), m_layout(nullptr), m_titleLabel(nullptr), m_searchEdit(nullptr), m_treeWidget(nullptr), m_searchDebounceTimer(nullptr)
{
    setupUI();
}

void FileTreeWidget::setupUI()
{
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(5, 5, 5, 5);

    m_titleLabel = new QLabel("EXPLORER");
    m_titleLabel->setStyleSheet("QLabel { font-size: 11px; color: #BBBBBB; letter-spacing: 1px; padding: 5px 0px 5px 15px; }");
    m_layout->addWidget(m_titleLabel);

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
    m_layout->addWidget(m_searchEdit);

    m_searchDebounceTimer = new QTimer(this);
    m_searchDebounceTimer->setSingleShot(true);
    m_searchDebounceTimer->setInterval(150);
    connect(m_searchDebounceTimer, &QTimer::timeout, this, &FileTreeWidget::performSearch);
    connect(m_searchEdit, &QLineEdit::textChanged, this, &FileTreeWidget::onSearchTextChanged);

    m_treeWidget = new QTreeWidget();
    m_treeWidget->setObjectName("fileTree");
    m_treeWidget->setHeaderHidden(true);
    m_treeWidget->setRootIsDecorated(true);
    m_treeWidget->setAnimated(true);
    m_treeWidget->setAlternatingRowColors(false);

    connect(m_treeWidget, &QTreeWidget::itemClicked, this, &FileTreeWidget::onItemClicked);
    connect(m_treeWidget, &QTreeWidget::itemExpanded, this, &FileTreeWidget::onItemExpanded);
    connect(m_treeWidget, &QTreeWidget::itemCollapsed, this, &FileTreeWidget::onItemCollapsed);
    m_layout->addWidget(m_treeWidget);

    setLayout(m_layout);
}

void FileTreeWidget::loadProject(const QString &projectPath, const QString &projectName, bool clearExisting)
{
    m_currentProjectPath = projectPath;

    if (clearExisting)
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
    if (!rootDir.exists()) return;

    QString displayName = projectName.isEmpty() ? rootDir.dirName() : projectName;
    QTreeWidgetItem *rootItem = createTreeItem(displayName, rootPath);
    m_treeWidget->addTopLevelItem(rootItem);

    addDirectoryToTree(rootItem, rootPath);

    rootItem->setExpanded(true);
    for (int i = 0; i < rootItem->childCount(); ++i)
        ensureChildrenLoaded(rootItem->child(i));
}

void FileTreeWidget::addDirectoryToTree(QTreeWidgetItem *parentItem, const QString &dirPath)
{
    QDir dir(dirPath);
    QStringList entries = dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot, QDir::DirsFirst | QDir::Name);

    int count = 0;
    foreach (const QString &entry, entries)
    {
        if (++count % 50 == 0)
            QCoreApplication::processEvents();

        QString fullPath = dir.absoluteFilePath(entry);
        QFileInfo fileInfo(fullPath);

        if (fileInfo.isDir())
        {
            QTreeWidgetItem *dirItem = createTreeItem(entry, fullPath);
            parentItem->addChild(dirItem);

            QDir subDir(fullPath);
            QStringList subEntries = subDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
            bool hasChildren = false;
            for (const auto &sub : subEntries)
            {
                QFileInfo subInfo(subDir.absoluteFilePath(sub));
                if (subInfo.isDir() || isJavaFile(sub))
                {
                    hasChildren = true;
                    break;
                }
            }
            if (hasChildren)
            {
                QTreeWidgetItem *placeholder = new QTreeWidgetItem();
                placeholder->setData(0, Qt::UserRole, QStringLiteral("__placeholder__"));
                dirItem->addChild(placeholder);
            }
        }
        else if (isJavaFile(entry))
        {
            QTreeWidgetItem *fileItem = createTreeItem(entry, fullPath, true);
            parentItem->addChild(fileItem);
        }
    }
}

void FileTreeWidget::ensureChildrenLoaded(QTreeWidgetItem *item)
{
    if (!hasPlaceholder(item)) return;

    delete item->takeChild(0);
    addDirectoryToTree(item, item->data(0, Qt::UserRole).toString());
}

bool FileTreeWidget::hasPlaceholder(QTreeWidgetItem *item) const
{
    return item->childCount() == 1 &&
           item->child(0)->data(0, Qt::UserRole).toString() == QStringLiteral("__placeholder__");
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
        item->setIcon(0, iconForFile(name));
    else
        item->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));

    return item;
}

QIcon FileTreeWidget::iconForFile(const QString &fileName)
{
    static struct { const char *ext; const char *resource; } map[] = {
        { ".java",  ":/icon/java.svg" },
        { ".kt",    ":/icon/kt.svg" },
        { ".smali", ":/icon/smali.svg" },
        { ".xml",   ":/icon/xml.svg" },
        { ".apk",   ":/icon/apk.svg" },
        { ".jar",   ":/icon/apk.svg" },
        { ".dex",   ":/icon/apk.svg" },
        { "",       ":/icon/file.svg" }
    };
    for (int i = 0; map[i].ext[0]; ++i)
    {
        if (fileName.endsWith(map[i].ext, Qt::CaseInsensitive))
            return QIcon(map[i].resource);
    }
    return QIcon(":/icon/file.svg");
}

void FileTreeWidget::onItemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    if (!item) return;

    QString fullPath = item->data(0, Qt::UserRole).toString();
    QFileInfo fileInfo(fullPath);
    if (fileInfo.isFile())
        emit fileSelected(fullPath);
}

void FileTreeWidget::onItemExpanded(QTreeWidgetItem *item)
{
    ensureChildrenLoaded(item);
}

void FileTreeWidget::onItemCollapsed(QTreeWidgetItem *item)
{
    if (item->childCount() > 0 && !hasPlaceholder(item))
    {
        QString fullPath = item->data(0, Qt::UserRole).toString();
        QDir dir(fullPath);
        QStringList entries = dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);

        item->takeChildren();

        bool hasChildren = false;
        for (const auto &entry : entries)
        {
            QFileInfo info(dir.absoluteFilePath(entry));
            if (info.isDir() || isJavaFile(entry))
            {
                hasChildren = true;
                break;
            }
        }
        if (hasChildren)
        {
            QTreeWidgetItem *placeholder = new QTreeWidgetItem();
            placeholder->setData(0, Qt::UserRole, QStringLiteral("__placeholder__"));
            item->addChild(placeholder);
        }
    }
}

void FileTreeWidget::onSearchTextChanged(const QString &text)
{
    Q_UNUSED(text)
    m_searchDebounceTimer->start();
}

void FileTreeWidget::performSearch()
{
    filterTree(m_searchEdit->text());
}

void FileTreeWidget::filterTree(const QString &searchText)
{
    if (m_treeWidget->topLevelItemCount() == 0) return;

    QTreeWidgetItem *rootItem = m_treeWidget->topLevelItem(0);

    if (searchText.isEmpty())
    {
        setItemVisible(rootItem, true);
        return;
    }

    bool hasVisibleChildren = itemMatchesSearch(rootItem, searchText);
    setItemVisible(rootItem, hasVisibleChildren);

    if (hasVisibleChildren)
        m_treeWidget->expandAll();
}

void FileTreeWidget::setItemVisible(QTreeWidgetItem *item, bool visible)
{
    if (!item) return;

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
    if (!item || searchText.isEmpty()) return true;

    if (item->data(0, Qt::UserRole).toString() == QStringLiteral("__placeholder__"))
        return false;

    if (item->text(0).contains(searchText, Qt::CaseInsensitive))
        return true;

    for (int i = 0; i < item->childCount(); ++i)
    {
        if (itemMatchesSearch(item->child(i), searchText))
            return true;
    }
    return false;
}
