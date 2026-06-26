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
// File: MainWindow.h
// Description: Header for MainWindow.
//
// ==============================================================================
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
class QDockWidget;
class QPlainTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // ==============================================================================
    // Public Interface
    // ==============================================================================
    // ==============================================================================
    // Constructor & Destructor
    // ==============================================================================
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // ==============================================================================
    // Private Slots
    // ==============================================================================
    // ==============================================================================
    // Qt Slots (Action Handlers)
    // ==============================================================================
    void openFile();
    void saveProject();
    void exportProject();
    void aboutApplication();
    void onDecompilationStarted();
    void onDecompilationFinished(bool success);
    void onDecompilationProgress(int progress);
    void updateCursorPosition(int line, int col);
    void updateFileType(const QString &type);
    void appendLogMessage(const QString &message);

private:
    // ==============================================================================
    // Private Members & Methods
    // ==============================================================================
    void setupUI();
    void setupMenuBar();
    void setupStatusBar();
    void setupCentralWidget();
    void updateWindowTitle(const QString &projectPath = QString());

    // ==============================================================================
    // UI Components
    // ==============================================================================
    QMenuBar *m_menuBar;
    QStatusBar *m_statusBar;
    QSplitter *m_centralSplitter;

    // ==============================================================================
    // Custom Widgets
    // ==============================================================================
    FileTreeWidget *m_fileTreeWidget;
    CodeEditorWidget *m_codeEditorWidget;
    QDockWidget *m_logDock;
    QPlainTextEdit *m_logTextEdit;

    // ==============================================================================
    // Core Logic Components
    // ==============================================================================
    DecompilerInterface *m_decompiler;
    ProjectManager *m_projectManager;

    // ==============================================================================
    // Menu Actions
    // ==============================================================================
    QAction *m_openAction;
    QAction *m_saveAction;
    QAction *m_exportAction;
    QAction *m_exitAction;
    QAction *m_aboutAction;

    // ==============================================================================
    // Status Bar Widgets
    // ==============================================================================
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

#endif // MAINWINDOW_H