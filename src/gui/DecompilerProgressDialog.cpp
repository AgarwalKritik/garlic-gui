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
// File: DecompilerProgressDialog.cpp
// Description: Dialog UI for displaying extraction and decompilation progress.
//
// ==============================================================================
#include "DecompilerProgressDialog.h"

// ==============================================================================
// Method: DecompilerProgressDialog::DecompilerProgressDialog
// ==============================================================================
DecompilerProgressDialog::DecompilerProgressDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    setModal(true);
    setWindowTitle("Decompiling...");
    setFixedSize(400, 150);
}

// ==============================================================================
// Method: DecompilerProgressDialog::setupUI
// ==============================================================================
void DecompilerProgressDialog::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(15);
    m_mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title
    m_titleLabel = new QLabel("Decompiling the file...");
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

    // Cancel button - Removed because Garlic C engine does not support cancellation.
    // A synchronous blocking call in a background thread cannot be cleanly aborted.

    setLayout(m_mainLayout);
}

// ==============================================================================
// Method: DecompilerProgressDialog::updateProgress
// ==============================================================================
void DecompilerProgressDialog::updateProgress(int progress)
{
    m_progressBar->setValue(progress);
}

// ==============================================================================
// Method: DecompilerProgressDialog::setStatusText
// ==============================================================================
void DecompilerProgressDialog::setStatusText(const QString &text)
{
    m_statusLabel->setText(text);
}

// ==============================================================================
// Method: DecompilerProgressDialog::onCancelClicked
// ==============================================================================
void DecompilerProgressDialog::onCancelClicked()
{
    emit cancelled();
    reject();
}