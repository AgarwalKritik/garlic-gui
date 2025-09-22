#include "CodeEditorWidget.h"
#include <QFileInfo>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QFont>

CodeEditorWidget::CodeEditorWidget(QWidget *parent)
    : QWidget(parent), m_layout(nullptr), m_tabWidget(nullptr)
{
    setupUI();
}

void CodeEditorWidget::setupUI()
{
    m_layout = new QVBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);

    m_tabWidget = new QTabWidget();
    m_tabWidget->setTabsClosable(true);
    m_tabWidget->setMovable(true);
    m_tabWidget->setStyleSheet(
        "QTabWidget::pane { "
        "  border: 1px solid #555; "
        "  background-color: #2b2b2b; "
        "} "
        "QTabWidget::tab-bar { "
        "  left: 5px; "
        "} "
        "QTabBar::tab { "
        "  background-color: #404040; "
        "  color: white; "
        "  border: 1px solid #555; "
        "  padding: 8px 12px; "
        "  margin-right: 2px; "
        "} "
        "QTabBar::tab:selected { "
        "  background-color: #2b2b2b; "
        "  border-bottom: 1px solid #2b2b2b; "
        "} "
        "QTabBar::tab:hover { "
        "  background-color: #505050; "
        "}");

    connect(m_tabWidget, &QTabWidget::tabCloseRequested, this, &CodeEditorWidget::onTabCloseRequested);

    m_layout->addWidget(m_tabWidget);
    setLayout(m_layout);
}

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
    QTextEdit *textEdit = createTextEditor();
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
    m_tabWidget->setCurrentIndex(tabIndex);

    m_openFiles.append(filePath);
}

QTextEdit *CodeEditorWidget::createTextEditor()
{
    QTextEdit *textEdit = new QTextEdit();
    textEdit->setReadOnly(true);
    textEdit->setLineWrapMode(QTextEdit::NoWrap);

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
        "QTextEdit { "
        "  background-color: #2b2b2b; "
        "  color: #ffffff; "
        "  border: none; "
        "  selection-background-color: #3d7848; "
        "}");

    return textEdit;
}

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

void CodeEditorWidget::onTabCloseRequested(int index)
{
    closeTab(index);
}

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
}

void CodeEditorWidget::closeAllTabs()
{
    m_tabWidget->clear();
    m_openFiles.clear();
}

// Java Syntax Highlighter Implementation
JavaSyntaxHighlighter::JavaSyntaxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // Java keywords
    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(QColor(86, 156, 214)); // Blue
    keywordFormat.setFontWeight(QFont::Bold);

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

    // Class names
    QTextCharFormat classFormat;
    classFormat.setForeground(QColor(78, 201, 176)); // Teal
    classFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression("\\b[A-Z][A-Za-z0-9_]*\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    // String literals
    QTextCharFormat stringFormat;
    stringFormat.setForeground(QColor(214, 157, 133)); // Orange
    rule.pattern = QRegularExpression("\"[^\"\\\\]*(\\\\.[^\"\\\\]*)*\"");
    rule.format = stringFormat;
    highlightingRules.append(rule);

    // Single line comments
    QTextCharFormat singleLineCommentFormat;
    singleLineCommentFormat.setForeground(QColor(106, 153, 85)); // Green
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    // Multi-line comments
    xmlCommentFormat.setForeground(QColor(106, 153, 85)); // Green
}

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