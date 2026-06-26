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
// File: WelcomeWidget.cpp
// Description: Implementation of the Welcome dashboard.
//
// ==============================================================================
#include "WelcomeWidget.h"
#include <QHBoxLayout>
#include <QKeySequence>

// ==============================================================================
// Method: WelcomeWidget::WelcomeWidget
// ==============================================================================
WelcomeWidget::WelcomeWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

// ==============================================================================
// Method: WelcomeWidget::setupUI
// ==============================================================================
void WelcomeWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(20);

    QLabel *logoLabel = new QLabel(this);
    QPixmap logo(":/icon/garlic.png");
    if (!logo.isNull()) {
        // Scale the logo with devicePixelRatio awareness for crisp Retina rendering.
        int logicalSize = 128;
        qreal dpr = devicePixelRatioF();
        QPixmap scaled = logo.scaled(
            static_cast<int>(logicalSize * dpr),
            static_cast<int>(logicalSize * dpr),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation);
        scaled.setDevicePixelRatio(dpr);
        logoLabel->setPixmap(scaled);
    }
    logoLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(logoLabel);

    QLabel *titleLabel = new QLabel("Garlic Decompiler", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #E0E0E0;");
    mainLayout->addWidget(titleLabel);

    QLabel *subtitleLabel = new QLabel("A fast Android APK/CLASS/JAR/DEX decompiler", this);
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("font-size: 14px; color: #858585; margin-bottom: 20px;");
    mainLayout->addWidget(subtitleLabel);

    QString btnStyle = "QPushButton { background-color: #2D2D30; color: #E0E0E0; border: none; padding: 10px 20px; border-radius: 4px; font-size: 14px; } "
                       "QPushButton:hover { background-color: #3994BC; color: white; } "
                       "QPushButton:pressed { background-color: #2b7a9e; }";

    // Build a platform-native shortcut hint for the button label.
    // QKeySequence::Open maps to Cmd+O (⌘O) on macOS and Ctrl+O on Windows/Linux.
    QString shortcutHint = QKeySequence(QKeySequence::Open).toString(QKeySequence::NativeText);
    QPushButton *openFileBtn = new QPushButton(
        QString("Open File... (%1)").arg(shortcutHint), this);
    openFileBtn->setCursor(Qt::PointingHandCursor);
    openFileBtn->setStyleSheet(btnStyle);
    connect(openFileBtn, &QPushButton::clicked, this, &WelcomeWidget::openFileRequested);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setAlignment(Qt::AlignCenter);
    btnLayout->addWidget(openFileBtn);

    mainLayout->addLayout(btnLayout);
}
