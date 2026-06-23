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

#include "MainWindow.h"
#include "FileTreeWidget.h"
#include "CodeEditorWidget.h"
#include "DecompilerInterface.h"
#include "ProjectManager.h"
#include "DecompilerProgressDialog.h"

#include <QToolBar>
#include <QToolButton>
#include <QIcon>

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include <QThread>
#include <QDir>
#include <QFileInfo>

inline void ensureOutputDir(const QString &path)
{
    QDir dir(path);
    if (!dir.exists())
    {
        dir.mkpath(".");
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_menuBar(nullptr), m_statusBar(nullptr), m_centralSplitter(nullptr), m_fileTreeWidget(nullptr), m_codeEditorWidget(nullptr), m_decompiler(nullptr), m_projectManager(nullptr), m_statusLabel(nullptr), m_progressBar(nullptr), m_progressDialog(nullptr)
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

    connect(m_fileTreeWidget, &FileTreeWidget::fileSelected,
            m_codeEditorWidget, &CodeEditorWidget::openFile);

    updateWindowTitle();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    setMinimumSize(1200, 800);
    resize(1400, 900);

    setupMenuBar();
    setupStatusBar();
    setupCentralWidget();
}

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
    fileMenu->addAction(m_exitAction);

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
    m_remoteLabel = new QLabel(" >< ");
    m_remoteLabel->setStyleSheet("background-color: #3994BC; color: #FFFFFF; font-weight: bold; border-radius: 2px; margin-right: 5px;");
    m_statusBar->addWidget(m_remoteLabel);

    m_gitLabel = new QLabel(" Garlic-GUI ");
    m_statusBar->addWidget(m_gitLabel);

    m_errorWarningLabel = new QLabel(" (x) 0  (!) 0 ");
    m_statusBar->addWidget(m_errorWarningLabel);

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

    m_spacesLabel = new QLabel("Spaces: 4");
    m_statusBar->addPermanentWidget(m_spacesLabel);

    m_encodingLabel = new QLabel("UTF-8");
    m_statusBar->addPermanentWidget(m_encodingLabel);

    m_crlfLabel = new QLabel("CRLF");
    m_statusBar->addPermanentWidget(m_crlfLabel);

    m_fileTypeLabel = new QLabel("Java");
    m_statusBar->addPermanentWidget(m_fileTypeLabel);
}



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
    connect(m_codeEditorWidget, &CodeEditorWidget::openFileRequested, m_openAction, &QAction::trigger);

    m_centralSplitter->addWidget(m_fileTreeWidget);
    m_centralSplitter->addWidget(m_codeEditorWidget);

    // Set splitter ratios
    m_centralSplitter->setStretchFactor(0, 0);
    m_centralSplitter->setStretchFactor(1, 1);

    setCentralWidget(m_centralSplitter);
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "Open Android/Java File",
        QString(),
        "Android/Java Files (*.apk *.dex *.jar *.class);;APK Files (*.apk);;DEX Files (*.dex);;JAR Files (*.jar);;Class Files (*.class);;All Files (*)");

    if (!fileName.isEmpty())
    {
        m_currentFile = fileName;
        m_currentFileType = m_decompiler->getFileTypeString(fileName);
        m_statusLabel->setText(QString("Loading %1 file...").arg(m_currentFileType));

        if (m_progressDialog)
        {
            m_progressDialog->deleteLater();
        }
        m_progressDialog = new DecompilerProgressDialog(this);
        m_progressDialog->show();

        connect(m_decompiler, &DecompilerInterface::progressUpdated,
                m_progressDialog, &DecompilerProgressDialog::updateProgress);

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

    QString exportDir = QFileDialog::getExistingDirectory(
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
    QMessageBox::about(this, "About Garlic Decompiler GUI",
                       "<h3>Garlic Decompiler GUI</h3>"
                       "<h4>Version 1.0</h4>"
                       "<p>A fast Android APK/CLASS/JAR/DEX decompiler with modern GUI interface.</p>"
                       "<p>Built with Qt6 with C++ and powered by Garlic decompiler engine written in C.</p>"
                       "<p>Acknowledgements:</p>"
                       "<p>1. <a href='https://github.com/neocanable/garlic'>Garlic Decompiler</a></p>"
                       "<p>2. <a href='https://www.qt.io/'>Qt Framework</a></p>"
                       "<p>3. GUI Concept and idea by <a href='https://lin.ky/abhithemodder'>AbhiTheModder</a>.</p>"
                       "<br>"
                       "<p>Designed and developed with ❤︎ by <a href='https://github.com/AgarwalKritik'>Kritik Agarwal</a>.</p>"
                       "<p>© 2025 AgarwalKritik. All rights reserved.</p>");
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
        m_fileTreeWidget->loadProject(outputDir, fileInfo.fileName());
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
    
    // Also update file type label based on current file
    if (!m_currentFileType.isEmpty()) {
        m_fileTypeLabel->setText(m_currentFileType);
    }
}

void MainWindow::updateWindowTitle(const QString &projectPath)
{
    QString title = "Garlic Decompiler";
    if (!projectPath.isEmpty())
    {
        QFileInfo fileInfo(projectPath);
        title += " - " + fileInfo.fileName();
    }
    setWindowTitle(title);
}
