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

#ifndef CODEEDITORWIDGET_H
#define CODEEDITORWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QRegularExpression>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QRect>
#include <QStackedWidget>
#include <QShortcut>

class JavaSyntaxHighlighter;
class CodeEditor;
class WelcomeWidget;
class FindReplaceWidget;

class LineNumberArea;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget *lineNumberArea;
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor), codeEditor(editor)
    {
    }

    QSize sizeHint() const override
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};

class CodeEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CodeEditorWidget(QWidget *parent = nullptr);

public slots:
    void openFile(const QString &filePath);
    void closeTab(int index);
    void closeAllTabs();

signals:
    void cursorPositionChanged(int line, int col);
    void openFileRequested();

private slots:
    void onTabCloseRequested(int index);
    void showFindReplace();
    void hideFindReplace();
    void findNext();
    void findPrev();
    void replaceCurrent();
    void replaceAll();
    void updateTabVisibility();

private:
    void setupUI();
    CodeEditor *createTextEditor();
    QString readFileContent(const QString &filePath);
    int findExistingTab(const QString &filePath);

    QVBoxLayout *m_layout;
    QStackedWidget *m_stackedWidget;
    WelcomeWidget *m_welcomeWidget;
    QTabWidget *m_tabWidget;
    FindReplaceWidget *m_findReplaceWidget;
    QShortcut *m_findShortcut;

    QList<QString> m_openFiles;
};

class JavaSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit JavaSyntaxHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat xmlElementFormat;
    QTextCharFormat xmlKeywordFormat;
    QTextCharFormat xmlValueFormat;
    QTextCharFormat xmlCommentFormat;
};

#endif // CODEEDITORWIDGET_H