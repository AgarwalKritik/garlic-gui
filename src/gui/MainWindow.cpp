#include "MainWindow.h"
#include "FileTreeWidget.h"
#include "CodeEditorWidget.h"
#include "DecompilerInterface.h"
#include "ProjectManager.h"
#include "DecompilerProgressDialog.h"

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_menuBar(nullptr), m_toolBar(nullptr), m_statusBar(nullptr), m_centralSplitter(nullptr), m_fileTreeWidget(nullptr), m_codeEditorWidget(nullptr), m_decompiler(nullptr), m_projectManager(nullptr), m_statusLabel(nullptr), m_progressBar(nullptr)
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
    setupToolBar();
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

    // Help Menu
    QMenu *helpMenu = m_menuBar->addMenu("&Help");

    m_aboutAction = new QAction("&About", this);
    m_aboutAction->setStatusTip("Show information about the application");
    connect(m_aboutAction, &QAction::triggered, this, &MainWindow::aboutApplication);
    helpMenu->addAction(m_aboutAction);
}

void MainWindow::setupToolBar()
{
    m_toolBar = addToolBar("Main");
    m_toolBar->setMovable(false);

    m_toolBar->addAction(m_openAction);
    m_toolBar->addSeparator();
    m_toolBar->addAction(m_saveAction);
    m_toolBar->addAction(m_exportAction);
}

void MainWindow::setupStatusBar()
{
    m_statusBar = statusBar();

    m_statusLabel = new QLabel("Ready");
    m_statusBar->addWidget(m_statusLabel, 1);

    m_progressBar = new QProgressBar();
    m_progressBar->setVisible(false);
    m_progressBar->setMaximumWidth(200);
    m_statusBar->addPermanentWidget(m_progressBar);
}

void MainWindow::setupCentralWidget()
{
    m_centralSplitter = new QSplitter(Qt::Horizontal, this);

    // Create file tree widget
    m_fileTreeWidget = new FileTreeWidget(this);
    m_fileTreeWidget->setMinimumWidth(250);
    m_fileTreeWidget->setMaximumWidth(400);

    // Create code editor widget
    m_codeEditorWidget = new CodeEditorWidget(this);

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
        // Show file type information
        QString fileType = m_decompiler->getFileTypeString(fileName);
        m_statusLabel->setText(QString("Loading %1 file...").arg(fileType));

        // Show progress dialog
        DecompilerProgressDialog *progressDialog = new DecompilerProgressDialog(this);
        progressDialog->show();

        // Start decompilation in background thread
        QThread *thread = new QThread(this);
        m_decompiler->moveToThread(thread);

        connect(thread, &QThread::started, [=]()
                { m_decompiler->decompileFile(fileName); });

        connect(m_decompiler, &DecompilerInterface::decompilationFinished, [=](bool success)
                {
            thread->quit();
            thread->wait();
            progressDialog->close();
            progressDialog->deleteLater();
            
            if (success) {
                QString outputDir = m_decompiler->getOutputDirectory();
                m_fileTreeWidget->loadProject(outputDir);
                m_currentProject = outputDir;
                updateWindowTitle(fileName);
                
                m_saveAction->setEnabled(true);
                m_exportAction->setEnabled(true);
                
                m_statusLabel->setText(QString("Successfully decompiled %1").arg(fileType));
            } else {
                m_statusLabel->setText("Decompilation failed");
            } });

        connect(m_decompiler, &DecompilerInterface::progressUpdated,
                progressDialog, &DecompilerProgressDialog::updateProgress);

        thread->start();
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
                       "<h3>Garlic Decompiler GUI v1.0.0</h3>"
                       "<p>A fast Android APK/CLASS/JAR/DEX decompiler with modern GUI interface.</p>"
                       "<p>Built with Qt6 and powered by Garlic decompiler engine.</p>"
                       "<p>Acknowledgements:</p>"
                       "<p><a href='https://github.com/neocanable/garlic'>Garlic Decompiler</a></p>"
                       "<p>Developed with ❤️ by AbhiTheModder & Kritik Agarwal.</p>"
                       "<p>© 2025 AbhiTheModder. All rights reserved.</p>");
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Prompt to save project if there are unsaved changes
    if (m_projectManager->hasUnsavedChanges(m_currentProject))
    {
        QMessageBox::StandardButton resBtn = QMessageBox::question(
            this, "Unsaved Changes",
            "You have unsaved changes. Do you want to save the project before exiting?",
            QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
            QMessageBox::Yes);

        if (resBtn == QMessageBox::Yes)
        {
            saveProject();
            event->accept();
        }
        else if (resBtn == QMessageBox::No)
        {
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
    else
    {
        event->accept();
    }
}

void MainWindow::onDecompilationStarted()
{
    m_statusLabel->setText("Decompiling...");
    m_progressBar->setVisible(true);
    m_progressBar->setValue(0);
    m_openAction->setEnabled(false);
}

void MainWindow::onDecompilationFinished(bool success)
{
    m_progressBar->setVisible(false);
    m_openAction->setEnabled(true);

    if (success)
    {
        m_statusLabel->setText("Decompilation completed successfully");
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
}

void MainWindow::updateWindowTitle(const QString &projectPath)
{
    QString title = "Garlic Decompiler GUI";
    if (!projectPath.isEmpty())
    {
        QFileInfo fileInfo(projectPath);
        title += " - " + fileInfo.fileName();
    }
    setWindowTitle(title);
}
