#include "FindReplaceWidget.h"
#include <QLabel>
#include <QToolButton>

FindReplaceWidget::FindReplaceWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void FindReplaceWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 5, 10, 5);
    mainLayout->setSpacing(5);

    setStyleSheet("FindReplaceWidget { background-color: #252526; border-bottom: 1px solid #3E3E42; } "
                  "QLineEdit { background-color: #3C3C3C; color: #CCCCCC; border: 1px solid #3E3E42; padding: 2px 5px; } "
                  "QLineEdit:focus { border: 1px solid #007ACC; } "
                  "QPushButton { background-color: #3C3C3C; color: #CCCCCC; border: 1px solid #3E3E42; padding: 4px 8px; } "
                  "QPushButton:hover { background-color: #4C4C4C; } "
                  "QCheckBox { color: #CCCCCC; }");

    // First row: Find
    QHBoxLayout *findLayout = new QHBoxLayout();
    
    m_findInput = new QLineEdit(this);
    m_findInput->setPlaceholderText("Find");
    
    m_matchCaseCheck = new QCheckBox("Aa", this);
    m_matchCaseCheck->setToolTip("Match Case");
    m_wholeWordCheck = new QCheckBox("\"\"", this);
    m_wholeWordCheck->setToolTip("Match Whole Word");
    m_regexCheck = new QCheckBox(".*", this);
    m_regexCheck->setToolTip("Use Regular Expression");

    QPushButton *findPrevBtn = new QPushButton("<", this);
    QPushButton *findNextBtn = new QPushButton(">", this);
    QPushButton *closeBtn = new QPushButton("X", this);

    findLayout->addWidget(m_findInput);
    findLayout->addWidget(m_matchCaseCheck);
    findLayout->addWidget(m_wholeWordCheck);
    findLayout->addWidget(m_regexCheck);
    findLayout->addWidget(findPrevBtn);
    findLayout->addWidget(findNextBtn);
    findLayout->addWidget(closeBtn);

    // Second row: Replace
    QHBoxLayout *replaceLayout = new QHBoxLayout();
    
    m_replaceInput = new QLineEdit(this);
    m_replaceInput->setPlaceholderText("Replace");

    QPushButton *replaceBtn = new QPushButton("Replace", this);
    QPushButton *replaceAllBtn = new QPushButton("Replace All", this);

    replaceLayout->addWidget(m_replaceInput);
    replaceLayout->addWidget(replaceBtn);
    replaceLayout->addWidget(replaceAllBtn);

    mainLayout->addLayout(findLayout);
    mainLayout->addLayout(replaceLayout);

    connect(findNextBtn, &QPushButton::clicked, this, &FindReplaceWidget::findNextRequested);
    connect(findPrevBtn, &QPushButton::clicked, this, &FindReplaceWidget::findPrevRequested);
    connect(replaceBtn, &QPushButton::clicked, this, &FindReplaceWidget::replaceRequested);
    connect(replaceAllBtn, &QPushButton::clicked, this, &FindReplaceWidget::replaceAllRequested);
    connect(closeBtn, &QPushButton::clicked, this, &FindReplaceWidget::closeRequested);
    connect(m_findInput, &QLineEdit::returnPressed, this, &FindReplaceWidget::findNextRequested);
    connect(m_replaceInput, &QLineEdit::returnPressed, this, &FindReplaceWidget::replaceRequested);
}

QString FindReplaceWidget::getFindText() const { return m_findInput->text(); }
QString FindReplaceWidget::getReplaceText() const { return m_replaceInput->text(); }
bool FindReplaceWidget::isRegex() const { return m_regexCheck->isChecked(); }
bool FindReplaceWidget::isMatchCase() const { return m_matchCaseCheck->isChecked(); }
bool FindReplaceWidget::isWholeWord() const { return m_wholeWordCheck->isChecked(); }
void FindReplaceWidget::focusFindInput() { m_findInput->setFocus(); m_findInput->selectAll(); }
