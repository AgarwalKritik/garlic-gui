#include "MainWindow.h"
#include "FileTreeWidget.h"
#include "CodeEditorWidget.h"
#include "WelcomeWidget.h"
#include "DecompilerInterface.h"
#include "ProjectManager.h"
#include "DecompilerProgressDialog.h"
#include "TitleBarWidget.h"


#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

#include <QApplication>
#include <QScreen>
#include <QFileDialog>
#include <QMessageBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <QStandardPaths>
#include <QDir>
#include <QThread>
#include <QFileInfo>

#ifdef Q_OS_MACOS
#include <QWindow>
#endif

#ifdef Q_OS_WIN
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <windows.h>
#include <dwmapi.h>
#endif

static QString sanitizeEngineText(const QString &raw)
{
    QString out;
    out.reserve(raw.size());
    for (int i = 0; i < raw.size(); ++i) {
        QChar c = raw[i];
        if (c == '\033') {
            while (i < raw.size() && raw[i] != 'm') ++i;
            continue;
        }
        if (c.unicode() >= 0x20 || c == '\n' || c == '\t')
            out += c;
    }
    return out.trimmed();
}

inline void ensureOutputDir(const QString &path)
{
    QDir dir(path);
    if (!dir.exists())
    {
        dir.mkpath(".");
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_menuBar(nullptr), m_statusBar(nullptr), m_centralSplitter(nullptr), m_fileTreeWidget(nullptr), m_codeEditorWidget(nullptr), m_decompiler(nullptr), m_projectManager(nullptr), m_titleBar(nullptr), m_mainLayout(nullptr), m_centralContainer(nullptr), m_openAction(nullptr), m_saveAction(nullptr), m_exportAction(nullptr), m_exitAction(nullptr), m_aboutAction(nullptr), m_statusLabel(nullptr), m_progressBar(nullptr), m_gitLabel(nullptr), m_cursorPositionLabel(nullptr), m_fileTypeLabel(nullptr),  m_currentProject(), m_currentFile(), m_currentFileType(), m_progressDialog(nullptr), m_clearWorkspaceOnDecompile(true)
{
    setupUI();

    m_decompiler = new DecompilerInterface(this);
    m_projectManager = new ProjectManager(this);

    connect(m_decompiler, &DecompilerInterface::decompilationStarted,
            this, &MainWindow::onDecompilationStarted);
    connect(m_decompiler, &DecompilerInterface::decompilationFinished,
            this, &MainWindow::onDecompilationFinished);
    connect(m_decompiler, &DecompilerInterface::progressUpdated,
            this, &MainWindow::onDecompilationProgress);
    connect(m_decompiler, &DecompilerInterface::logMessage,
            this, [this](const QString &msg) {
        int lastB = msg.lastIndexOf('\b');
        QString last = (lastB >= 0) ? msg.mid(lastB + 1) : msg;
        QString clean = sanitizeEngineText(last);
        if (clean.isEmpty())
            return;
        if (clean.startsWith("Progress :"))
            return;
        setStatusMessage(clean, false);
    });

    connect(m_fileTreeWidget, &FileTreeWidget::fileSelected,
            m_codeEditorWidget, &CodeEditorWidget::openFile);

    updateWindowTitle();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
#ifdef Q_OS_MACOS
    setMinimumSize(960, 600);
    QRect available = QGuiApplication::primaryScreen()->availableGeometry();
    int w = qMin(1400, static_cast<int>(available.width() * 0.90));
    int h = qMin(900,  static_cast<int>(available.height() * 0.85));
    resize(w, h);
#else
    setMinimumSize(1200, 800);
    resize(1400, 900);
#endif

#ifndef Q_OS_MACOS
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

# ifdef Q_OS_WIN
    if (HWND hwnd = (HWND)effectiveWinId()) {
        const MARGINS m = { 0, 0, 0, 1 };
        DwmExtendFrameIntoClientArea(hwnd, &m);
    }
# endif
#endif

    m_centralContainer = new QWidget(this);
    m_mainLayout = new QVBoxLayout(m_centralContainer);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_mainLayout->setSpacing(0);

#ifndef Q_OS_MACOS
    setupTitleBar();
#endif

    setupMenuBar();
    setupStatusBar();
    setupCentralWidget();

    setCentralWidget(m_centralContainer);
}

void MainWindow::setupTitleBar()
{
    m_titleBar = new TitleBarWidget(m_centralContainer);
    m_mainLayout->addWidget(m_titleBar);

    connect(m_titleBar, &TitleBarWidget::minimizeClicked, this, [this]() {
        setWindowState(windowState() | Qt::WindowMinimized);
    });
    connect(m_titleBar, &TitleBarWidget::maximizeClicked, this, [this]() {
        if (isMaximized()) showNormal(); else showMaximized();
    });
    connect(m_titleBar, &TitleBarWidget::closeClicked, this, &QWidget::close);
}

void MainWindow::setupMenuBar()
{
#ifdef Q_OS_MACOS
    m_menuBar = menuBar();
#else
    m_menuBar = new QMenuBar(m_centralContainer);
    m_mainLayout->addWidget(m_menuBar);
#endif

    QMenu *fileMenu = m_menuBar->addMenu("&File");

    m_openAction = new QAction("&Open APK/CLASS/JAR/DEX...", this);
    m_openAction->setShortcut(QKeySequence::Open);
    m_openAction->setStatusTip("Open APK, CLASS, JAR, or DEX file for decompilation");
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openFile);
    fileMenu->addAction(m_openAction);

    fileMenu->addSeparator();

    m_saveAction = new QAction("&Save Project", this);
    m_saveAction->setShortcut(QKeySequence::Save);
    m_saveAction->setStatusTip("Save current decompiled project");
    m_saveAction->setEnabled(false);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveProject);
    fileMenu->addAction(m_saveAction);

    m_exportAction = new QAction("&Export as...", this);
    m_exportAction->setShortcut(QKeySequence("Ctrl+E"));
    m_exportAction->setStatusTip("Export decompiled project to directory");
    m_exportAction->setEnabled(false);
    connect(m_exportAction, &QAction::triggered, this, &MainWindow::exportProject);
    fileMenu->addAction(m_exportAction);

    fileMenu->addSeparator();

    m_exitAction = new QAction("E&xit", this);
    m_exitAction->setShortcut(QKeySequence::Quit);
    m_exitAction->setStatusTip("Exit the application");
    connect(m_exitAction, &QAction::triggered, this, &QWidget::close);
#ifndef Q_OS_MACOS
    fileMenu->addAction(m_exitAction);
#endif

    QMenu *editMenu = m_menuBar->addMenu("&Edit");
    editMenu->addAction("Undo");
    editMenu->addAction("Redo");

    QMenu *viewMenu = m_menuBar->addMenu("&View");
    viewMenu->addAction("Explorer");
    viewMenu->addAction("Search");

    QMenu *helpMenu = m_menuBar->addMenu("&Help");

    m_aboutAction = new QAction("&About", this);
    m_aboutAction->setStatusTip("Show information about the application");
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::aboutApplication);
    helpMenu->addAction(m_aboutAction);
}

void MainWindow::setupStatusBar()
{
    m_statusBar = statusBar();

    m_gitLabel = new QLabel(" Garlic-GUI ");
    m_statusBar->addWidget(m_gitLabel);

    m_statusLabel = new QLabel("Ready");
    m_statusBar->addWidget(m_statusLabel, 1);
    setStatusMessage("Ready", true);

    m_progressBar = new QProgressBar();
    m_progressBar->setVisible(false);
    m_progressBar->setMaximumWidth(150);
    m_statusBar->addWidget(m_progressBar);

    m_cursorPositionLabel = new QLabel("Ln 1, Col 1");
    m_statusBar->addPermanentWidget(m_cursorPositionLabel);

    m_fileTypeLabel = new QLabel("Java");
    m_statusBar->addPermanentWidget(m_fileTypeLabel);
}

void MainWindow::setupCentralWidget()
{
    m_centralSplitter = new QSplitter(Qt::Horizontal, this);

    m_fileTreeWidget = new FileTreeWidget(this);
    m_fileTreeWidget->setMinimumWidth(250);
    m_fileTreeWidget->setMaximumWidth(400);

    m_codeEditorWidget = new CodeEditorWidget(this);
    connect(m_codeEditorWidget, &CodeEditorWidget::cursorPositionChanged, this, &MainWindow::updateCursorPosition);
    connect(m_codeEditorWidget, &CodeEditorWidget::fileTypeChanged, this, &MainWindow::updateFileType);
    connect(m_codeEditorWidget, &CodeEditorWidget::openFileRequested, m_openAction, &QAction::trigger);

    WelcomeWidget *welcome = m_codeEditorWidget->welcomeWidget();
    connect(welcome, &WelcomeWidget::openFileRequested, m_openAction, &QAction::trigger);

    m_centralSplitter->addWidget(m_fileTreeWidget);
    m_centralSplitter->addWidget(m_codeEditorWidget);

    m_centralSplitter->setStretchFactor(0, 0);
    m_centralSplitter->setStretchFactor(1, 1);

    m_mainLayout->addWidget(m_centralSplitter, 1);
}

void MainWindow::openFile()
{
    QString fileName;

    fileName = QFileDialog::getOpenFileName(
        this,
        "Open Android/Java File",
        QString(),
        "Android/Java Files (*.apk *.dex *.jar *.class);;APK Files (*.apk);;DEX Files (*.dex);;JAR Files (*.jar);;Class Files (*.class);;All Files (*)");

    if (!fileName.isEmpty())
    {
        if (!m_currentProject.isEmpty())
        {
            QMessageBox::StandardButton reply = QMessageBox::question(
                this, 
                "Clear Workspace?", 
                "A project is already open.\n\nDo you want to CLEAR the current workspace before opening this new file?\n\nSelect 'Yes' to clear, or 'No' to keep the current files open for simultaneous analysis.",
                QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
            );

            if (reply == QMessageBox::Cancel) {
                return;
            } else if (reply == QMessageBox::Yes) {
                m_clearWorkspaceOnDecompile = true;
                m_codeEditorWidget->closeAllTabs();
            } else {
                m_clearWorkspaceOnDecompile = false;
            }
        }
        else
        {
            m_clearWorkspaceOnDecompile = true;
        }

        m_currentFile = fileName;
        m_currentFileType = m_decompiler->getFileTypeString(fileName);
        m_statusLabel->setText(QString("Loading %1 file...").arg(m_currentFileType));

        if (m_progressDialog)
        {
            m_progressDialog->deleteLater();
        }
        m_progressDialog = new DecompilerProgressDialog(this);
        m_progressDialog->show();

        disconnect(m_decompiler, &DecompilerInterface::progressUpdated,
                   m_progressDialog, &DecompilerProgressDialog::updateProgress);
        disconnect(m_decompiler, &DecompilerInterface::logMessage,
                   m_progressDialog, &DecompilerProgressDialog::setStatusText);

        connect(m_decompiler, &DecompilerInterface::progressUpdated,
                m_progressDialog, &DecompilerProgressDialog::updateProgress);
        connect(m_decompiler, &DecompilerInterface::logMessage,
                m_progressDialog, &DecompilerProgressDialog::setStatusText);

        m_decompiler->decompileFile(fileName);
    }
}

void MainWindow::saveProject()
{
    if (!m_currentProject.isEmpty())
    {
        m_projectManager->saveProject(m_currentProject);
        m_statusLabel->setText("Project saved successfully");
    }
}

void MainWindow::exportProject()
{
    if (m_currentProject.isEmpty())
        return;

    QString exportDir;

    exportDir = QFileDialog::getExistingDirectory(
        this,
        "Export Project To",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));

    if (!exportDir.isEmpty())
    {
        if (m_projectManager->exportProject(m_currentProject, exportDir))
        {
            QMessageBox::information(this, "Export Complete",
                                     QString("Project exported successfully to:\n%1").arg(exportDir));
        }
        else
        {
            QMessageBox::warning(this, "Export Failed",
                                 "Failed to export project. Please check the destination directory.");
        }
    }
}

void MainWindow::aboutApplication()
{
    QDialog *aboutDlg = new QDialog(this);
    aboutDlg->setWindowTitle("About Garlic Decompiler GUI");
    aboutDlg->setStyleSheet(
        "QDialog { background-color: #1e1e1e; }"
        "QLabel { color: #e0e0e0; font-family: -apple-system, 'Segoe UI', sans-serif; }");

    QVBoxLayout *layout = new QVBoxLayout(aboutDlg);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QLabel *logo = new QLabel(aboutDlg);
    QPixmap pix(":/icon/garlic.png");
    if (!pix.isNull())
        logo->setPixmap(pix.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logo->setAlignment(Qt::AlignCenter);
    logo->setStyleSheet("padding-top: 24px; padding-bottom: 8px;");
    layout->addWidget(logo);

    QLabel *nameLabel = new QLabel("Garlic Decompiler GUI", aboutDlg);
    nameLabel->setAlignment(Qt::AlignCenter);
    nameLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #4da6ff; padding-bottom: 2px;");
    layout->addWidget(nameLabel);

    QLabel *versionLabel = new QLabel("Version 1.0", aboutDlg);
    versionLabel->setAlignment(Qt::AlignCenter);
    versionLabel->setStyleSheet("font-size: 12px; color: #858585; padding-bottom: 16px;");
    layout->addWidget(versionLabel);

    QFrame *sep1 = new QFrame(aboutDlg);
    sep1->setFrameShape(QFrame::HLine);
    sep1->setStyleSheet("color: #333333; margin: 0 24px;");
    layout->addWidget(sep1);

    QLabel *descLabel = new QLabel(
        "A lightning-fast Android APK / CLASS / JAR / DEX\ndecompiler with a modern, responsive interface.", aboutDlg);
    descLabel->setAlignment(Qt::AlignCenter);
    descLabel->setStyleSheet("font-size: 12px; color: #b0b0b0; padding: 14px 24px 14px 24px;");
    layout->addWidget(descLabel);

    QFrame *sep2 = new QFrame(aboutDlg);
    sep2->setFrameShape(QFrame::HLine);
    sep2->setStyleSheet("color: #333333; margin: 0 24px;");
    layout->addWidget(sep2);

    auto makeLink = [](const QString &label, const QString &url) {
        return QString("<a href='%1' style='color: #4da6ff; text-decoration: none;'>%2</a>").arg(url, label);
    };

    QLabel *poweredBy = new QLabel(aboutDlg);
    poweredBy->setTextFormat(Qt::RichText);
    poweredBy->setText(
        QString("<b style='color: #ffffff;'>Powered By</b><br>"
                "&bull; %1 &mdash; C<br>"
                "&bull; %2 &mdash; C++")
            .arg(makeLink("Garlic Decompiler Engine", "https://github.com/neocanable/garlic"),
                 makeLink("Qt 6 Framework", "https://www.qt.io/")));
    poweredBy->setAlignment(Qt::AlignCenter);
    poweredBy->setStyleSheet("font-size: 12px; color: #b0b0b0; padding: 12px 24px 4px 24px;");
    poweredBy->setOpenExternalLinks(true);
    layout->addWidget(poweredBy);

    QLabel *creditLabel = new QLabel(aboutDlg);
    creditLabel->setTextFormat(Qt::RichText);
    creditLabel->setText(
        QString("<b style='color: #ffffff;'>Acknowledgements</b><br>"
                "&bull; GUI Concept by %1")
            .arg(makeLink("AbhiTheModder", "https://lin.ky/abhithemodder")));
    creditLabel->setAlignment(Qt::AlignCenter);
    creditLabel->setStyleSheet("font-size: 12px; color: #b0b0b0; padding: 4px 24px 4px 24px;");
    creditLabel->setOpenExternalLinks(true);
    layout->addWidget(creditLabel);

    QFrame *sep3 = new QFrame(aboutDlg);
    sep3->setFrameShape(QFrame::HLine);
    sep3->setStyleSheet("color: #333333; margin: 12px 24px 0 24px;");
    layout->addWidget(sep3);

    QLabel *footer = new QLabel(aboutDlg);
    footer->setTextFormat(Qt::RichText);
    footer->setText(
        QString("Designed and developed with ❤ by %1<br>"
                "&copy; 2026 %2. All rights reserved.<br>"
                "Source: %3")
            .arg(makeLink("Kritik Agarwal", "https://github.com/AgarwalKritik"),
                 makeLink("Kritik Agarwal", "https://github.com/AgarwalKritik"),
                 makeLink("github.com/AgarwalKritik/garlic-gui", "https://github.com/AgarwalKritik/garlic-gui")));
    footer->setAlignment(Qt::AlignCenter);
    footer->setStyleSheet("font-size: 10px; color: #777777; padding: 10px 24px 16px 24px;");
    footer->setOpenExternalLinks(true);
    layout->addWidget(footer);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setContentsMargins(0, 0, 0, 0);
    btnLayout->setAlignment(Qt::AlignCenter);
    QPushButton *okBtn = new QPushButton("OK", aboutDlg);
    okBtn->setFixedWidth(80);
    okBtn->setStyleSheet(
        "QPushButton { background-color: #3994BC; color: white; border: none; "
        "padding: 6px 20px; border-radius: 4px; font-size: 13px; } "
        "QPushButton:hover { background-color: #4da6ff; }");
    connect(okBtn, &QPushButton::clicked, aboutDlg, &QDialog::accept);
    btnLayout->addWidget(okBtn);
    layout->addLayout(btnLayout);

    aboutDlg->setFixedSize(aboutDlg->sizeHint());
    aboutDlg->exec();
    aboutDlg->deleteLater();
}

void MainWindow::onDecompilationStarted()
{
    m_statusLabel->setText(QString("Decompiling %1...").arg(m_currentFileType));
    m_progressBar->setVisible(true);
    m_progressBar->setValue(0);
    m_openAction->setEnabled(false);
}

void MainWindow::onDecompilationFinished(bool success)
{
    m_progressBar->setVisible(false);
    m_openAction->setEnabled(true);

    if (m_progressDialog)
    {
        m_progressDialog->close();
        m_progressDialog->deleteLater();
        m_progressDialog = nullptr;
    }

    if (success)
    {
        QString outputDir = m_decompiler->getOutputDirectory();
        QFileInfo fileInfo(m_currentFile);
        
        m_fileTreeWidget->loadProject(outputDir, fileInfo.fileName(), m_clearWorkspaceOnDecompile);
        
        m_currentProject = outputDir;
        updateWindowTitle(m_currentFile);

        m_saveAction->setEnabled(true);
        m_exportAction->setEnabled(true);

        m_statusLabel->setText(QString("Successfully decompiled %1").arg(m_currentFileType));
    }
    else
    {
        m_statusLabel->setText("Decompilation failed");
        QMessageBox::warning(this, "Decompilation Error",
                             "Failed to decompile the selected file. Please check if the file is valid.");
    }
}

void MainWindow::onDecompilationProgress(int progress)
{
    m_progressBar->setValue(progress);
    m_statusLabel->setText(QString("Decompiling %1... %2%").arg(m_currentFileType).arg(progress));
}

void MainWindow::updateCursorPosition(int line, int col)
{
    m_cursorPositionLabel->setText(QString("Ln %1, Col %2").arg(line).arg(col));
}

void MainWindow::updateFileType(const QString &type)
{
    m_fileTypeLabel->setText(type);
    m_currentFileType = type;
}

void MainWindow::updateWindowTitle(const QString &projectPath)
{
    QString title = "Garlic Decompiler";
    if (!projectPath.isEmpty())
    {
        QFileInfo fileInfo(projectPath);
        title += " - " + fileInfo.fileName();

#ifdef Q_OS_MACOS
        if (windowHandle())
            windowHandle()->setFilePath(projectPath);
#endif
    }

    setWindowTitle(title);
    if (m_titleBar)
        m_titleBar->setTitle(title);
}

void MainWindow::setStatusMessage(const QString &message, bool persistent)
{
    m_statusLabel->setGraphicsEffect(nullptr);
    m_statusLabel->setText(message);

    if (!persistent)
    {
        QTimer::singleShot(5000, this, [this, message]() {
            if (m_statusLabel && m_statusLabel->text() == message)
            {
                QGraphicsOpacityEffect *fade = new QGraphicsOpacityEffect(m_statusLabel);
                m_statusLabel->setGraphicsEffect(fade);
                QPropertyAnimation *anim = new QPropertyAnimation(fade, "opacity", this);
                anim->setDuration(400);
                anim->setStartValue(1.0);
                anim->setEndValue(0.4);
                anim->setEasingCurve(QEasingCurve::OutCubic);
                connect(anim, &QPropertyAnimation::finished, this, [this]() {
                    if (m_statusLabel)
                        m_statusLabel->setGraphicsEffect(nullptr);
                });
                anim->start(QAbstractAnimation::DeleteWhenStopped);
            }
        });
    }
}




