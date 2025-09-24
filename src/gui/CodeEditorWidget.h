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

#ifndef CODEEDITORWIDGET_H
#define CODEEDITORWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QRegularExpression>

class JavaSyntaxHighlighter;

class CodeEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CodeEditorWidget(QWidget *parent = nullptr);

public slots:
    void openFile(const QString &filePath);
    void closeTab(int index);
    void closeAllTabs();

private slots:
    void onTabCloseRequested(int index);

private:
    void setupUI();
    QTextEdit *createTextEditor();
    QString readFileContent(const QString &filePath);
    int findExistingTab(const QString &filePath);

    QVBoxLayout *m_layout;
    QTabWidget *m_tabWidget;

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