#ifndef FILETREEWIDGET_H
#define FILETREEWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTreeWidgetItem>
#include <QTimer>

class FileTreeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileTreeWidget(QWidget *parent = nullptr);

    void loadProject(const QString &projectPath, const QString &projectName = QString(), bool clearExisting = true);
    void clearProject();

signals:
    void fileSelected(const QString &filePath);

private slots:
    void onItemClicked(QTreeWidgetItem *item, int column);
    void onItemExpanded(QTreeWidgetItem *item);
    void onItemCollapsed(QTreeWidgetItem *item);
    void onSearchTextChanged(const QString &text);
    void performSearch();

private:
    void setupUI();
    void populateTree(const QString &rootPath, const QString &projectName = QString());
    void addDirectoryToTree(QTreeWidgetItem *parentItem, const QString &dirPath);
    void ensureChildrenLoaded(QTreeWidgetItem *item);
    bool hasPlaceholder(QTreeWidgetItem *item) const;
    bool isJavaFile(const QString &fileName);
    QTreeWidgetItem *createTreeItem(const QString &name, const QString &fullPath, bool isFile = false);
    void filterTree(const QString &searchText);
    void setItemVisible(QTreeWidgetItem *item, bool visible);
    bool itemMatchesSearch(QTreeWidgetItem *item, const QString &searchText);
    QIcon iconForFile(const QString &fileName);

    QVBoxLayout *m_layout;
    QLabel *m_titleLabel;
    QLineEdit *m_searchEdit;
    QTreeWidget *m_treeWidget;
    QTimer *m_searchDebounceTimer;
    QString m_currentProjectPath;
};

#endif
