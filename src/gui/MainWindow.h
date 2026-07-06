#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QAction>
#include <QLabel>
#include <QProgressBar>

class FileTreeWidget;
class CodeEditorWidget;
class DecompilerInterface;
class ProjectManager;
class DecompilerProgressDialog;
class TitleBarWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openFile();
    void saveProject();
    void exportProject();
    void aboutApplication();
    void onDecompilationStarted();
    void onDecompilationFinished(bool success);
    void onDecompilationProgress(int progress);
    void updateCursorPosition(int line, int col);
    void updateFileType(const QString &type);
private:
    void setupUI();
    void setupTitleBar();
    void setupMenuBar();
    void setupStatusBar();
    void setupCentralWidget();
    void updateWindowTitle(const QString &projectPath = QString());
    void setStatusMessage(const QString &message, bool persistent = false);

    QMenuBar *m_menuBar;
    QStatusBar *m_statusBar;
    QSplitter *m_centralSplitter;

    FileTreeWidget *m_fileTreeWidget;
    CodeEditorWidget *m_codeEditorWidget;

    DecompilerInterface *m_decompiler;
    ProjectManager *m_projectManager;

    TitleBarWidget *m_titleBar;
    QVBoxLayout *m_mainLayout;
    QWidget *m_centralContainer;

    QAction *m_openAction;
    QAction *m_saveAction;
    QAction *m_exportAction;
    QAction *m_exitAction;
    QAction *m_aboutAction;

    QLabel *m_statusLabel;
    QProgressBar *m_progressBar;
    QLabel *m_gitLabel;
    QLabel *m_cursorPositionLabel;
    QLabel *m_fileTypeLabel;

    QString m_currentProject;
    QString m_currentFile;
    QString m_currentFileType;
    DecompilerProgressDialog *m_progressDialog;
    bool m_clearWorkspaceOnDecompile = true;
};

#endif
