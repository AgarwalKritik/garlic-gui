// ==============================================================================
//               Copyright 2026 Kritik Agarwal
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//          http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// ==============================================================================
//
// File: MainWindow.cpp
// Description: Implementation of the main application window and global layout.
//
// ==============================================================================
#include "MainWindow.h"
#include "FileTreeWidget.h"
#include "CodeEditorWidget.h"
#include "WelcomeWidget.h"
#include "DecompilerInterface.h"
#include "ProjectManager.h"
#include "DecompilerProgressDialog.h"

#include <QDockWidget>
#include <QPlainTextEdit>
#include <QScrollBar>

#include <QApplication>
#include <QScreen>
#include <QFileDialog>
#include <QMessageBox>
#include <QDialog>
#include <QTextBrowser>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QStandardPaths>
#include <QDir>
#include <QThread>
#include <QFileInfo>

#ifdef Q_OS_MACOS
#include <QWindow>
#endif

inline void ensureOutputDir(const QString &path)
{
    QDir dir(path);
    if (!dir.exists())
    {
        dir.mkpath(".");
    }
}

// ==============================================================================
// Constructor & Destructor
// ==============================================================================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_menuBar(nullptr), m_statusBar(nullptr), m_centralSplitter(nullptr), m_fileTreeWidget(nullptr), m_codeEditorWidget(nullptr), m_logDock(nullptr), m_logTextEdit(nullptr), m_decompiler(nullptr), m_projectManager(nullptr), m_statusLabel(nullptr), m_progressBar(nullptr), m_progressDialog(nullptr)
{
    setupUI();

    // Initialize core components
    m_decompiler = new DecompilerInterface(this);
    m_projectManager = new ProjectManager(this);

    // Connect signals
    connect(m_decompiler, &DecompilerInterface::decompilationStarted,
            this, &MainWindow::onDecompilationStarted);
    connect(m_decompiler, &DecompilerInterface::decompilationFinished,
            this, &MainWindow::onDecompilationFinished);
    connect(m_decompiler, &DecompilerInterface::progressUpdated,
            this, &MainWindow::onDecompilationProgress);
    connect(m_decompiler, &DecompilerInterface::logMessage,
            this, &MainWindow::appendLogMessage);

    connect(m_fileTreeWidget, &FileTreeWidget::fileSelected,
            m_codeEditorWidget, &CodeEditorWidget::openFile);

    updateWindowTitle();
}

// ==============================================================================
// Method: MainWindow::~MainWindow
// ==============================================================================
MainWindow::~MainWindow()
{
}

// ==============================================================================
// UI Initialization Methods
// ==============================================================================
void MainWindow::setupUI()
{
#ifdef Q_OS_MACOS
    // macOS: reduce minimum size to fit 13" MacBook screens (1280×800 logical).
    // The Dock and menu bar consume ~100px vertically, so 900×600 is safe.
    setMinimumSize(960, 600);
    // Size the window to 85% of the available screen area on macOS.
    QRect available = QGuiApplication::primaryScreen()->availableGeometry();
    int w = qMin(1400, static_cast<int>(available.width() * 0.90));
    int h = qMin(900,  static_cast<int>(available.height() * 0.85));
    resize(w, h);
#else
    setMinimumSize(1200, 800);
    resize(1400, 900);
#endif

    setupMenuBar();
    setupStatusBar();
    setupCentralWidget();
}

// ==============================================================================
// Method: MainWindow::setupMenuBar
// ==============================================================================
void MainWindow::setupMenuBar()
{
    m_menuBar = menuBar();

    // File Menu
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
    // On macOS, the OS provides Quit (Cmd+Q) in the application menu.
    // Adding it to the File menu duplicates the action against macOS HIG.
    fileMenu->addAction(m_exitAction);
#endif

    // Edit, View, Go, Run Menus (Placeholders for VS Code aesthetic)
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

// ==============================================================================
// Method: MainWindow::setupStatusBar
// ==============================================================================
void MainWindow::setupStatusBar()
{
    m_statusBar = statusBar();
    m_statusBar->setStyleSheet(
        "QStatusBar { "
        "  background-color: #121314; "
        "  color: #858585; "
        "  border-top: 1px solid #2D2D2D; "
        "} "
        "QStatusBar::item { "
        "  border: none; "
        "} "
        "QLabel { "
        "  padding: 0px 5px; "
        "  font-size: 12px; "
        "}");

    // Left Side
    m_gitLabel = new QLabel(" Garlic-GUI ");
    m_statusBar->addWidget(m_gitLabel);

    m_statusLabel = new QLabel("Ready");
    m_statusBar->addWidget(m_statusLabel, 1);

    m_progressBar = new QProgressBar();
    m_progressBar->setVisible(false);
    m_progressBar->setMaximumWidth(150);
    m_progressBar->setStyleSheet(
        "QProgressBar { background-color: #1E1E1E; border: 1px solid #333333; color: white; text-align: center; } "
        "QProgressBar::chunk { background-color: #3994BC; }");
    m_statusBar->addWidget(m_progressBar);

    // Right Side
    m_cursorPositionLabel = new QLabel("Ln 1, Col 1");
    m_statusBar->addPermanentWidget(m_cursorPositionLabel);

    m_fileTypeLabel = new QLabel("Java");
    m_statusBar->addPermanentWidget(m_fileTypeLabel);
}

// ==============================================================================
// Method: MainWindow::setupCentralWidget
// ==============================================================================
void MainWindow::setupCentralWidget()
{
    m_centralSplitter = new QSplitter(Qt::Horizontal, this);
    m_centralSplitter->setStyleSheet(
        "QSplitter::handle { "
        "  background-color: #333333; "
        "} "
        "QSplitter::handle:horizontal { "
        "  width: 1px; "
        "}");

    // Create file tree widget
    m_fileTreeWidget = new FileTreeWidget(this);
    m_fileTreeWidget->setMinimumWidth(250);
    m_fileTreeWidget->setMaximumWidth(400);

    // Create code editor widget
    m_codeEditorWidget = new CodeEditorWidget(this);
    connect(m_codeEditorWidget, &CodeEditorWidget::cursorPositionChanged, this, &MainWindow::updateCursorPosition);
    connect(m_codeEditorWidget, &CodeEditorWidget::fileTypeChanged, this, &MainWindow::updateFileType);
    connect(m_codeEditorWidget, &CodeEditorWidget::openFileRequested, m_openAction, &QAction::trigger);

    // Connect the WelcomeWidget "Open File" button to the open action.
    // Note: WelcomeWidget is created inside CodeEditorWidget; we access it via signal.
    // The WelcomeWidget's openFileRequested signal is forwarded by CodeEditorWidget.
    // Direct connection to ensure the open dialog is triggered on all platforms.
    WelcomeWidget *welcome = m_codeEditorWidget->findChild<WelcomeWidget *>();
    if (welcome) {
        connect(welcome, &WelcomeWidget::openFileRequested, m_openAction, &QAction::trigger);
    }

    m_centralSplitter->addWidget(m_fileTreeWidget);
    m_centralSplitter->addWidget(m_codeEditorWidget);

    // Set splitter ratios
    m_centralSplitter->setStretchFactor(0, 0);
    m_centralSplitter->setStretchFactor(1, 1);

    setCentralWidget(m_centralSplitter);

    // Setup Log Dock
    m_logDock = new QDockWidget("Output Log", this);
    m_logDock->setAllowedAreas(Qt::BottomDockWidgetArea | Qt::TopDockWidgetArea);

    m_logTextEdit = new QPlainTextEdit(m_logDock);
    m_logTextEdit->setReadOnly(true);
    // Use a platform-appropriate monospace font cascade.
    // SF Mono / Menlo are native on macOS; Consolas on Windows; DejaVu on Linux.
    m_logTextEdit->setStyleSheet(
        "QPlainTextEdit { "
        "  background-color: #1e1e1e; "
        "  color: #d4d4d4; "
        "  font-family: 'SF Mono', Menlo, Monaco, Consolas, 'Courier New', monospace; "
        "}");
    m_logDock->setWidget(m_logTextEdit);

    addDockWidget(Qt::BottomDockWidgetArea, m_logDock);
}

// ==============================================================================
// Slots (Action Handlers)
// ==============================================================================
void MainWindow::openFile()
{
    QString fileName;

#ifdef Q_OS_MACOS
    // On macOS Sequoia, the native NSOpenPanel can silently fail without a
    // proper bundle identifier. Force Qt's own cross-platform dialog instead.
    fileName = QFileDialog::getOpenFileName(
        this,
        "Open Android/Java File",
        QString(),
        "Android/Java Files (*.apk *.dex *.jar *.class);;APK Files (*.apk);;DEX Files (*.dex);;JAR Files (*.jar);;Class Files (*.class);;All Files (*)",
        nullptr,
        QFileDialog::DontUseNativeDialog);
#else
    fileName = QFileDialog::getOpenFileName(
        this,
        "Open Android/Java File",
        QString(),
        "Android/Java Files (*.apk *.dex *.jar *.class);;APK Files (*.apk);;DEX Files (*.dex);;JAR Files (*.jar);;Class Files (*.class);;All Files (*)");
#endif

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

        // Disconnect any existing connections to the previous dialog before
        // reconnecting — prevents duplicate signal firings on subsequent file opens.
        disconnect(m_decompiler, &DecompilerInterface::progressUpdated, nullptr, nullptr);
        disconnect(m_decompiler, &DecompilerInterface::logMessage, nullptr, nullptr);

        // Reconnect to the new dialog and to the persistent log dock.
        connect(m_decompiler, &DecompilerInterface::progressUpdated,
                this, &MainWindow::onDecompilationProgress);
        connect(m_decompiler, &DecompilerInterface::logMessage,
                this, &MainWindow::appendLogMessage);
        connect(m_decompiler, &DecompilerInterface::progressUpdated,
                m_progressDialog, &DecompilerProgressDialog::updateProgress);
        connect(m_decompiler, &DecompilerInterface::logMessage,
                m_progressDialog, &DecompilerProgressDialog::setStatusText);

        m_decompiler->decompileFile(fileName);
    }
}

// ==============================================================================
// Method: MainWindow::saveProject
// ==============================================================================
void MainWindow::saveProject()
{
    if (!m_currentProject.isEmpty())
    {
        m_projectManager->saveProject(m_currentProject);
        m_statusLabel->setText("Project saved successfully");
    }
}

// ==============================================================================
// Method: MainWindow::exportProject
// ==============================================================================
void MainWindow::exportProject()
{
    if (m_currentProject.isEmpty())
        return;

    QString exportDir;

#ifdef Q_OS_MACOS
    // Force non-native dialog on macOS Sequoia for reliability.
    exportDir = QFileDialog::getExistingDirectory(
        this,
        "Export Project To",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        QFileDialog::ShowDirsOnly | QFileDialog::DontUseNativeDialog);
#else
    exportDir = QFileDialog::getExistingDirectory(
        this,
        "Export Project To",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
#endif

    if (!exportDir.isEmpty())
    {
        if (m_projectManager->exportProject(m_currentProject, exportDir))
        {

// ==============================================================================
// Method: QMessageBox::information
// ==============================================================================
            QMessageBox::information(this, "Export Complete",
                                     QString("Project exported successfully to:\n%1").arg(exportDir));
        }
        else
        {

// ==============================================================================
// Method: QMessageBox::warning
// ==============================================================================
            QMessageBox::warning(this, "Export Failed",
                                 "Failed to export project. Please check the destination directory.");
        }
    }
}

// ==============================================================================
// Method: MainWindow::aboutApplication
// ==============================================================================
void MainWindow::aboutApplication()
{

    // Use a custom QDialog with QTextBrowser so HTML links are clickable.
    // QMessageBox::about() does not support clickable links on macOS.
    QDialog *aboutDlg = new QDialog(this);
    aboutDlg->setWindowTitle("About Garlic Decompiler GUI");
    aboutDlg->setMinimumSize(480, 320);

    QVBoxLayout *layout = new QVBoxLayout(aboutDlg);
    layout->setContentsMargins(20, 20, 20, 16);

    QTextBrowser *browser = new QTextBrowser(aboutDlg);
    browser->setOpenExternalLinks(true);
    browser->setStyleSheet(
        "QTextBrowser { background-color: #1e1e1e; color: #e0e0e0; "
        "border: none; font-family: -apple-system, 'Segoe UI', sans-serif; }");
    browser->setHtml(
        "<div style='font-family: -apple-system, \"Segoe UI\", sans-serif;'>"
        "<h2 style='color: #4da6ff; margin-bottom: 5px; margin-top: 0px;'>Garlic Decompiler GUI</h2>"
        "<h4 style='color: #a0a0a0; margin-top: 0px; margin-bottom: 15px;'>Version 1.0</h4>"
        "<p style='font-size: 13px; color: #e0e0e0;'>"
        "A lightning-fast Android <b>APK / CLASS / JAR / DEX</b> decompiler featuring a modern, responsive interface."
        "</p>"
        "<hr style='border: none; height: 1px; background-color: #333; margin: 15px 0px;'/>"
        "<p style='font-size: 13px; color: #e0e0e0;'>"
        "<b style='color: #ffffff;'>Powered By:</b><br/>"
        "&nbsp;&bull;&nbsp; <a style='color: #4da6ff;' href='https://github.com/neocanable/garlic'>Garlic Decompiler Engine</a> (C)<br/>"
        "&nbsp;&bull;&nbsp; <a style='color: #4da6ff;' href='https://www.qt.io/'>Qt 6 Framework</a> (C++)"
        "</p>"
        "<p style='font-size: 13px; color: #e0e0e0;'>"
        "<b style='color: #ffffff;'>Acknowledgements:</b><br/>"
        "&nbsp;&bull;&nbsp; GUI Concept &amp; Idea by <a style='color: #4da6ff;' href='https://lin.ky/abhithemodder'>AbhiTheModder</a>"
        "</p>"
        "<hr style='border: none; height: 1px; background-color: #333; margin: 15px 0px;'/>"
        "<p style='font-size: 12px; color: #888; text-align: center; margin-bottom: 0px;'>"
        "Designed and developed with ❤︎ by Kritik Agarwal.<br/>"
        "&copy; 2026 <a style='color: #4da6ff;' href='https://github.com/AgarwalKritik'>Kritik Agarwal</a>. All rights reserved."
        "</p>"
        "</div>");
    layout->addWidget(browser);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok, aboutDlg);
    connect(buttons, &QDialogButtonBox::accepted, aboutDlg, &QDialog::accept);
    layout->addWidget(buttons);

    aboutDlg->exec();
    aboutDlg->deleteLater();
}

// ==============================================================================
// Method: MainWindow::onDecompilationStarted
// ==============================================================================
void MainWindow::onDecompilationStarted()
{
    m_statusLabel->setText(QString("Decompiling %1...").arg(m_currentFileType));
    m_progressBar->setVisible(true);
    m_progressBar->setValue(0);
    m_openAction->setEnabled(false);
}

// ==============================================================================
// Method: MainWindow::onDecompilationFinished
// ==============================================================================
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

// ==============================================================================
// Method: QMessageBox::warning
// ==============================================================================
        QMessageBox::warning(this, "Decompilation Error",
                             "Failed to decompile the selected file. Please check if the file is valid.");
    }
}

// ==============================================================================
// Method: MainWindow::onDecompilationProgress
// ==============================================================================
void MainWindow::onDecompilationProgress(int progress)
{
    m_progressBar->setValue(progress);
    m_statusLabel->setText(QString("Decompiling %1... %2%").arg(m_currentFileType).arg(progress));
}

// ==============================================================================
// Method: MainWindow::updateCursorPosition
// ==============================================================================
void MainWindow::updateCursorPosition(int line, int col)
{
    m_cursorPositionLabel->setText(QString("Ln %1, Col %2").arg(line).arg(col));
}

// ==============================================================================
// Method: MainWindow::updateFileType
// ==============================================================================
void MainWindow::updateFileType(const QString &type)
{
    m_fileTypeLabel->setText(type);
    m_currentFileType = type;
}

// ==============================================================================
// Method: MainWindow::updateWindowTitle
// ==============================================================================
void MainWindow::updateWindowTitle(const QString &projectPath)
{
    QString title = "Garlic Decompiler";
    if (!projectPath.isEmpty())
    {
        QFileInfo fileInfo(projectPath);
        title += " - " + fileInfo.fileName();

#ifdef Q_OS_MACOS
        // Set the macOS proxy icon in the title bar so users can
        // drag the file from the title bar directly to Finder.
        if (windowHandle())
            windowHandle()->setFilePath(projectPath);
#endif
    }
    setWindowTitle(title);
}

// ==============================================================================
// Method: MainWindow::appendLogMessage
// ==============================================================================
void MainWindow::appendLogMessage(const QString &message)
{
    if (m_logTextEdit)
    {
        QTextCursor cursor = m_logTextEdit->textCursor();
        cursor.beginEditBlock();
        cursor.movePosition(QTextCursor::End);

        for (int i = 0; i < message.length(); ++i)
        {
            QChar c = message[i];
            if (c == '\b')
            {
                cursor.deletePreviousChar();
            }
            else if (c == '\r')
            {
                cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
                cursor.removeSelectedText();
            }
            else
            {
                cursor.insertText(QString(c));
            }
        }

        cursor.endEditBlock();
        m_logTextEdit->setTextCursor(cursor);
        QScrollBar *bar = m_logTextEdit->verticalScrollBar();
        bar->setValue(bar->maximum());
    }
}
