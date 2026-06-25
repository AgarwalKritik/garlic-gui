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
// File: CodeEditorWidget.cpp
// Description: Implementation of the advanced code editor widget with syntax highlighting.
//
// ==============================================================================
#include "CodeEditorWidget.h"
#include "WelcomeWidget.h"
#include "FindReplaceWidget.h"
#include <QFileInfo>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QFont>
#include <QPainter>
#include <QTextBlock>

// ==============================================================================
// CodeEditor Sub-Widget Implementation
// ==============================================================================
CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}

// ==============================================================================
// Method: CodeEditor::lineNumberAreaWidth
// ==============================================================================
int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

// ==============================================================================
// Method: CodeEditor::updateLineNumberAreaWidth
// ==============================================================================
void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

// ==============================================================================
// Method: CodeEditor::updateLineNumberArea
// ==============================================================================
void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

// ==============================================================================
// Method: CodeEditor::resizeEvent
// ==============================================================================
void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

// ==============================================================================
// Method: CodeEditor::highlightCurrentLine
// ==============================================================================
void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor(40, 40, 40); // VS Code active line highlight

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

// ==============================================================================
// Method: CodeEditor::lineNumberAreaPaintEvent
// ==============================================================================
void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor(30, 30, 30)); // #1E1E1E

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor(133, 133, 133)); // #858585
            painter.drawText(0, top, lineNumberArea->width() - 8, fontMetrics().height(),
                             Qt::AlignRight | Qt::AlignVCenter, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}
// ---------------------------------

// ==============================================================================
// Method: CodeEditorWidget::CodeEditorWidget
// ==============================================================================
CodeEditorWidget::CodeEditorWidget(QWidget *parent)
    : QWidget(parent), m_layout(nullptr), m_tabWidget(nullptr)
{
    setupUI();
}

// ==============================================================================
// Method: CodeEditorWidget::setupUI
// ==============================================================================
void CodeEditorWidget::setupUI()
{
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);

    m_findReplaceWidget = new FindReplaceWidget(this);
    m_findReplaceWidget->hide();
    connect(m_findReplaceWidget, &FindReplaceWidget::closeRequested, this, &CodeEditorWidget::hideFindReplace);
    connect(m_findReplaceWidget, &FindReplaceWidget::findNextRequested, this, &CodeEditorWidget::findNext);
    connect(m_findReplaceWidget, &FindReplaceWidget::findPrevRequested, this, &CodeEditorWidget::findPrev);
    connect(m_findReplaceWidget, &FindReplaceWidget::replaceRequested, this, &CodeEditorWidget::replaceCurrent);
    connect(m_findReplaceWidget, &FindReplaceWidget::replaceAllRequested, this, &CodeEditorWidget::replaceAll);
    m_layout->addWidget(m_findReplaceWidget);

    m_stackedWidget = new QStackedWidget(this);

    m_welcomeWidget = new WelcomeWidget(this);
    connect(m_welcomeWidget, &WelcomeWidget::openFileRequested, this, &CodeEditorWidget::openFileRequested);

    m_tabWidget = new QTabWidget();
    m_tabWidget->setTabsClosable(true);
    m_tabWidget->setMovable(true);
    m_tabWidget->setStyleSheet(
        "QTabWidget::pane { "
        "  border: none; "
        "  border-top: 1px solid #191A1B; "
        "  background-color: #121314; "
        "} "
        "QTabWidget::tab-bar { "
        "  alignment: left; "
        "} "
        "QTabBar::tab { "
        "  background-color: #191A1B; "
        "  color: #969696; "
        "  border: none; "
        "  border-right: 1px solid #121314; "
        "  padding: 8px 16px; "
        "} "
        "QTabBar::tab:selected { "
        "  background-color: #121314; "
        "  color: #FFFFFF; "
        "  border-top: 1px solid #3994BC; "
        "} "
        "QTabBar::tab:hover:!selected { "
        "  background-color: #202122; "
        "}");

    connect(m_tabWidget, &QTabWidget::tabCloseRequested, this, &CodeEditorWidget::onTabCloseRequested);
    connect(m_tabWidget, &QTabWidget::currentChanged, this, [this](int index)
            {
        if (index >= 0) {
            CodeEditor *editor = qobject_cast<CodeEditor*>(m_tabWidget->widget(index));
            if (editor) {
                QTextCursor cursor = editor->textCursor();
                emit cursorPositionChanged(cursor.blockNumber() + 1, cursor.columnNumber() + 1);
                
                QString filePath = m_tabWidget->tabToolTip(index);
                QFileInfo info(filePath);
                QString ext = info.suffix().toUpper();
                if (ext.isEmpty()) ext = "Text";
                else if (ext == "JAVA") ext = "Java";
                else if (ext == "XML") ext = "XML";
                else if (ext == "MF") ext = "Manifest";
                emit fileTypeChanged(ext);
            }
        } });

    m_stackedWidget->addWidget(m_welcomeWidget);
    m_stackedWidget->addWidget(m_tabWidget);
    m_stackedWidget->setCurrentWidget(m_welcomeWidget);

    m_layout->addWidget(m_stackedWidget);
    setLayout(m_layout);

    m_findShortcut = new QShortcut(QKeySequence("Ctrl+F"), this);
    connect(m_findShortcut, &QShortcut::activated, this, &CodeEditorWidget::showFindReplace);
}

// ==============================================================================
// Method: CodeEditorWidget::openFile
// ==============================================================================
void CodeEditorWidget::openFile(const QString &filePath)
{
    if (filePath.isEmpty())
        return;

    // Check if file is already open
    int existingTab = findExistingTab(filePath);
    if (existingTab != -1)
    {
        m_tabWidget->setCurrentIndex(existingTab);
        return;
    }

    // Read file content
    QString content = readFileContent(filePath);
    if (content.isNull())
    {
        QMessageBox::warning(this, "Error", "Failed to read file: " + filePath);
        return;
    }

    // Create new text editor
    CodeEditor *textEdit = createTextEditor();
    textEdit->setPlainText(content);

    // Add syntax highlighting for Java files
    if (filePath.endsWith(".java", Qt::CaseInsensitive))
    {
        new JavaSyntaxHighlighter(textEdit->document());
    }

    // Add tab
    QFileInfo fileInfo(filePath);
    int tabIndex = m_tabWidget->addTab(textEdit, fileInfo.fileName());
    m_tabWidget->setTabToolTip(tabIndex, filePath);

    connect(textEdit, &CodeEditor::cursorPositionChanged, this, [this, textEdit]()
            {
        if (m_tabWidget->currentWidget() == textEdit) {
            QTextCursor cursor = textEdit->textCursor();
            emit cursorPositionChanged(cursor.blockNumber() + 1, cursor.columnNumber() + 1);
        } });

    m_tabWidget->setCurrentIndex(tabIndex);

    m_openFiles.append(filePath);
    updateTabVisibility();
}

CodeEditor *CodeEditorWidget::createTextEditor()
{
    CodeEditor *textEdit = new CodeEditor();
    textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);

    // Set font
    QFont font("Consolas", 10);
    if (!font.exactMatch())
    {
        font.setFamily("Courier New");
    }
    font.setFixedPitch(true);
    textEdit->setFont(font);

    // Set style
    textEdit->setStyleSheet(
        "QPlainTextEdit { "
        "  background-color: #121314; "
        "  color: #D4D4D4; "
        "  border: none; "
        "  selection-background-color: #264F78; "
        "}");

    return textEdit;
}

// ==============================================================================
// Method: CodeEditorWidget::readFileContent
// ==============================================================================
QString CodeEditorWidget::readFileContent(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return QString();
    }

    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);
    return stream.readAll();
}

// ==============================================================================
// Method: CodeEditorWidget::findExistingTab
// ==============================================================================
int CodeEditorWidget::findExistingTab(const QString &filePath)
{
    for (int i = 0; i < m_openFiles.size(); ++i)
    {
        if (m_openFiles.at(i) == filePath)
        {
            return i;
        }
    }
    return -1;
}

// ==============================================================================
// Method: CodeEditorWidget::onTabCloseRequested
// ==============================================================================
void CodeEditorWidget::onTabCloseRequested(int index)
{
    closeTab(index);
}

// ==============================================================================
// Method: CodeEditorWidget::closeTab
// ==============================================================================
void CodeEditorWidget::closeTab(int index)
{
    if (index >= 0 && index < m_tabWidget->count())
    {
        m_tabWidget->removeTab(index);
        if (index < m_openFiles.size())
        {
            m_openFiles.removeAt(index);
        }
    }
    updateTabVisibility();
}

// ==============================================================================
// Method: CodeEditorWidget::closeAllTabs
// ==============================================================================
void CodeEditorWidget::closeAllTabs()
{
    m_tabWidget->clear();
    m_openFiles.clear();
    updateTabVisibility();
}

// ==============================================================================
// Method: CodeEditorWidget::showFindReplace
// ==============================================================================
void CodeEditorWidget::showFindReplace()
{
    if (m_stackedWidget->currentWidget() == m_tabWidget)
    {
        m_findReplaceWidget->show();
        m_findReplaceWidget->focusFindInput();
    }
}

// ==============================================================================
// Method: CodeEditorWidget::hideFindReplace
// ==============================================================================
void CodeEditorWidget::hideFindReplace()
{
    m_findReplaceWidget->hide();
    if (CodeEditor *editor = qobject_cast<CodeEditor *>(m_tabWidget->currentWidget()))
    {
        editor->setFocus();
    }
}

// ==============================================================================
// Method: CodeEditorWidget::updateTabVisibility
// ==============================================================================
void CodeEditorWidget::updateTabVisibility()
{
    if (m_tabWidget->count() > 0)
    {
        m_stackedWidget->setCurrentWidget(m_tabWidget);
    }
    else
    {
        m_stackedWidget->setCurrentWidget(m_welcomeWidget);
        m_findReplaceWidget->hide();
    }
}

// ==============================================================================
// Method: CodeEditorWidget::findNext
// ==============================================================================
void CodeEditorWidget::findNext()
{
    CodeEditor *editor = qobject_cast<CodeEditor *>(m_tabWidget->currentWidget());
    if (!editor)
        return;

    QString query = m_findReplaceWidget->getFindText();
    if (query.isEmpty())
        return;

    QTextDocument::FindFlags flags;
    if (m_findReplaceWidget->isMatchCase())
        flags |= QTextDocument::FindCaseSensitively;
    if (m_findReplaceWidget->isWholeWord())
        flags |= QTextDocument::FindWholeWords;

    bool found = false;
    if (m_findReplaceWidget->isRegex())
    {
        QRegularExpression regex(query, m_findReplaceWidget->isMatchCase() ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption);
        found = editor->find(regex, flags);
    }
    else
    {
        found = editor->find(query, flags);
    }

    // Wrap around
    if (!found)
    {
        QTextCursor cursor = editor->textCursor();
        cursor.movePosition(QTextCursor::Start);
        editor->setTextCursor(cursor);
        if (m_findReplaceWidget->isRegex())
        {
            QRegularExpression regex(query, m_findReplaceWidget->isMatchCase() ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption);
            editor->find(regex, flags);
        }
        else
        {
            editor->find(query, flags);
        }
    }
}

// ==============================================================================
// Method: CodeEditorWidget::findPrev
// ==============================================================================
void CodeEditorWidget::findPrev()
{
    CodeEditor *editor = qobject_cast<CodeEditor *>(m_tabWidget->currentWidget());
    if (!editor)
        return;

    QString query = m_findReplaceWidget->getFindText();
    if (query.isEmpty())
        return;

    QTextDocument::FindFlags flags = QTextDocument::FindBackward;
    if (m_findReplaceWidget->isMatchCase())
        flags |= QTextDocument::FindCaseSensitively;
    if (m_findReplaceWidget->isWholeWord())
        flags |= QTextDocument::FindWholeWords;

    bool found = false;
    if (m_findReplaceWidget->isRegex())
    {
        QRegularExpression regex(query, m_findReplaceWidget->isMatchCase() ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption);
        found = editor->find(regex, flags);
    }
    else
    {
        found = editor->find(query, flags);
    }

    // Wrap around
    if (!found)
    {
        QTextCursor cursor = editor->textCursor();
        cursor.movePosition(QTextCursor::End);
        editor->setTextCursor(cursor);
        if (m_findReplaceWidget->isRegex())
        {
            QRegularExpression regex(query, m_findReplaceWidget->isMatchCase() ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption);
            editor->find(regex, flags);
        }
        else
        {
            editor->find(query, flags);
        }
    }
}

// ==============================================================================
// Method: CodeEditorWidget::replaceCurrent
// ==============================================================================
void CodeEditorWidget::replaceCurrent()
{
    CodeEditor *editor = qobject_cast<CodeEditor *>(m_tabWidget->currentWidget());
    if (!editor)
        return;

    QTextCursor cursor = editor->textCursor();
    if (cursor.hasSelection())
    {
        cursor.insertText(m_findReplaceWidget->getReplaceText());
    }
    findNext();
}

// ==============================================================================
// Method: CodeEditorWidget::replaceAll
// ==============================================================================
void CodeEditorWidget::replaceAll()
{
    CodeEditor *editor = qobject_cast<CodeEditor *>(m_tabWidget->currentWidget());
    if (!editor)
        return;

    QString query = m_findReplaceWidget->getFindText();
    if (query.isEmpty())
        return;

    QTextDocument::FindFlags flags;
    if (m_findReplaceWidget->isMatchCase())
        flags |= QTextDocument::FindCaseSensitively;
    if (m_findReplaceWidget->isWholeWord())
        flags |= QTextDocument::FindWholeWords;

    QTextCursor cursor = editor->textCursor();
    cursor.beginEditBlock();
    cursor.movePosition(QTextCursor::Start);
    editor->setTextCursor(cursor);

    while (true)
    {
        bool found = false;
        if (m_findReplaceWidget->isRegex())
        {
            QRegularExpression regex(query, m_findReplaceWidget->isMatchCase() ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption);
            found = editor->find(regex, flags);
        }
        else
        {
            found = editor->find(query, flags);
        }

        if (!found)
            break;
        editor->textCursor().insertText(m_findReplaceWidget->getReplaceText());
    }

    cursor.endEditBlock();
}

// Java Syntax Highlighter Implementation

// ==============================================================================
// Method: JavaSyntaxHighlighter::JavaSyntaxHighlighter
// ==============================================================================
JavaSyntaxHighlighter::JavaSyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // Java keywords
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor(255, 123, 114)); // #FF7B72 (Red)

    QStringList keywordPatterns;
    keywordPatterns << "\\babstract\\b" << "\\bassert\\b" << "\\bboolean\\b"
                    << "\\bbreak\\b" << "\\bbyte\\b" << "\\bcase\\b"
                    << "\\bcatch\\b" << "\\bchar\\b" << "\\bclass\\b"
                    << "\\bcontinue\\b" << "\\bdefault\\b" << "\\bdo\\b"
                    << "\\bdouble\\b" << "\\belse\\b" << "\\benum\\b"
                    << "\\bextends\\b" << "\\bfinal\\b" << "\\bfinally\\b"
                    << "\\bfloat\\b" << "\\bfor\\b" << "\\bif\\b"
                    << "\\bimplements\\b" << "\\bimport\\b" << "\\binstanceof\\b"
                    << "\\bint\\b" << "\\binterface\\b" << "\\blong\\b"
                    << "\\bnative\\b" << "\\bnew\\b" << "\\bpackage\\b"
                    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                    << "\\breturn\\b" << "\\bshort\\b" << "\\bstatic\\b"
                    << "\\bstrictfp\\b" << "\\bsuper\\b" << "\\bswitch\\b"
                    << "\\bsynchronized\\b" << "\\bthis\\b" << "\\bthrow\\b"
                    << "\\bthrows\\b" << "\\btransient\\b" << "\\btry\\b"
                    << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bwhile\\b";

    foreach (const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // Class names and Types
    QTextCharFormat classFormat;
    classFormat.setForeground(QColor(78, 201, 176)); // #4EC9B0 (Teal)
    rule.pattern = QRegularExpression("\\b[A-Z][A-Za-z0-9_]*\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    // Methods
    QTextCharFormat methodFormat;
    methodFormat.setForeground(QColor(210, 168, 255)); // #D2A8FF (Purple)
    rule.pattern = QRegularExpression("\\b[a-z][A-Za-z0-9_]*(?=\\s*\\()");
    rule.format = methodFormat;
    highlightingRules.append(rule);

    // Fields / Variables
    QTextCharFormat fieldFormat;
    fieldFormat.setForeground(QColor(156, 220, 254)); // #9CDCFE
    rule.pattern = QRegularExpression("\\b[a-z][A-Za-z0-9_]*\\b");
    rule.format = fieldFormat;
    highlightingRules.append(rule);

    // Numbers
    QTextCharFormat numberFormat;
    numberFormat.setForeground(QColor(121, 192, 255)); // #79C0FF (Light Blue)
    rule.pattern = QRegularExpression("\\b[0-9]+[a-zA-Z]*\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);

    // String literals
    QTextCharFormat stringFormat;
    stringFormat.setForeground(QColor(165, 214, 255)); // #A5D6FF (Light Blue)
    rule.pattern = QRegularExpression("\"[^\"\\\\]*(\\\\.[^\"\\\\]*)*\"");
    rule.format = stringFormat;
    highlightingRules.append(rule);

    // Single line comments
    QTextCharFormat singleLineCommentFormat;
    singleLineCommentFormat.setForeground(QColor(139, 148, 158)); // #8B949E (Gray-Blue)
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    // Multi-line comments
    xmlCommentFormat.setForeground(QColor(139, 148, 158)); // #8B949E (Gray-Blue)
}

// ==============================================================================
// Method: JavaSyntaxHighlighter::highlightBlock
// ==============================================================================
void JavaSyntaxHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    // Multi-line comments
    QRegularExpression startExpression("/\\*");
    QRegularExpression endExpression("\\*/");

    setCurrentBlockState(0);

    QRegularExpressionMatch startMatch = startExpression.match(text);
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = startMatch.capturedStart();

    while (startIndex >= 0)
    {
        QRegularExpressionMatch endMatch = endExpression.match(text, startIndex);
        int endIndex = endMatch.capturedStart();
        int commentLength = 0;
        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex + endMatch.capturedLength();
        }
        setFormat(startIndex, commentLength, xmlCommentFormat);
        startMatch = startExpression.match(text, startIndex + commentLength);
        startIndex = startMatch.capturedStart();
    }
}