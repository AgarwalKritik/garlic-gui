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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
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

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void setupCentralWidget();
    void updateWindowTitle(const QString &projectPath = QString());

    // UI Components
    QMenuBar *m_menuBar;
    QToolBar *m_toolBar;
    QStatusBar *m_statusBar;
    QSplitter *m_centralSplitter;

    // Custom widgets
    FileTreeWidget *m_fileTreeWidget;
    CodeEditorWidget *m_codeEditorWidget;

    // Core components
    DecompilerInterface *m_decompiler;
    ProjectManager *m_projectManager;

    // Actions
    QAction *m_openAction;
    QAction *m_saveAction;
    QAction *m_exportAction;
    QAction *m_exitAction;
    QAction *m_aboutAction;

    // Status bar widgets
    QLabel *m_statusLabel;
    QProgressBar *m_progressBar;

    QString m_currentProject;
};

#endif // MAINWINDOW_H