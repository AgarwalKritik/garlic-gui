#include "DecompilerProgressDialog.h"

DecompilerProgressDialog::DecompilerProgressDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    setModal(true);
    setWindowTitle("Decompiling...");
    setFixedSize(400, 150);
}

void DecompilerProgressDialog::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(15);
    m_mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title
    m_titleLabel = new QLabel("Decompiling APK/DEX File");
    m_titleLabel->setStyleSheet("QLabel { font-size: 14px; font-weight: bold; }");
    m_mainLayout->addWidget(m_titleLabel);

    // Status text
    m_statusLabel = new QLabel("Initializing decompiler...");
    m_statusLabel->setStyleSheet("QLabel { color: #888; }");
    m_mainLayout->addWidget(m_statusLabel);

    // Progress bar
    m_progressBar = new QProgressBar();
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_progressBar->setStyleSheet(
        "QProgressBar { "
        "  border: 1px solid #555; "
        "  border-radius: 3px; "
        "  text-align: center; "
        "  background-color: #2b2b2b; "
        "} "
        "QProgressBar::chunk { "
        "  background-color: #3d7848; "
        "  border-radius: 2px; "
        "}");
    m_mainLayout->addWidget(m_progressBar);

    // Cancel button
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    m_cancelButton = new QPushButton("Cancel");
    m_cancelButton->setStyleSheet(
        "QPushButton { "
        "  padding: 8px 20px; "
        "  border: 1px solid #555; "
        "  border-radius: 3px; "
        "  background-color: #404040; "
        "  color: white; "
        "} "
        "QPushButton:hover { "
        "  background-color: #505050; "
        "} "
        "QPushButton:pressed { "
        "  background-color: #353535; "
        "}");
    connect(m_cancelButton, &QPushButton::clicked, this, &DecompilerProgressDialog::onCancelClicked);

    buttonLayout->addWidget(m_cancelButton);
    m_mainLayout->addLayout(buttonLayout);

    setLayout(m_mainLayout);
}

void DecompilerProgressDialog::updateProgress(int progress)
{
    m_progressBar->setValue(progress);

    if (progress < 25)
    {
        setStatusText("Parsing file structure...");
    }
    else if (progress < 50)
    {
        setStatusText("Extracting classes...");
    }
    else if (progress < 75)
    {
        setStatusText("Decompiling methods...");
    }
    else if (progress < 95)
    {
        setStatusText("Generating source code...");
    }
    else
    {
        setStatusText("Finalizing...");
    }
}

void DecompilerProgressDialog::setStatusText(const QString &text)
{
    m_statusLabel->setText(text);
}

void DecompilerProgressDialog::onCancelClicked()
{
    emit cancelled();
    reject();
}