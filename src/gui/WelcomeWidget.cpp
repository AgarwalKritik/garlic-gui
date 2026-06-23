#include "WelcomeWidget.h"
#include <QHBoxLayout>

WelcomeWidget::WelcomeWidget(QWidget *parent) : QWidget(parent)
{
    setupUI();
}

void WelcomeWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(20);

    QLabel *logoLabel = new QLabel(this);
    QPixmap logo(":/icon/garlic.png");
    if (!logo.isNull()) {
        logoLabel->setPixmap(logo.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
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

    QPushButton *openFileBtn = new QPushButton("Open File... (Ctrl+O)", this);
    openFileBtn->setCursor(Qt::PointingHandCursor);
    openFileBtn->setStyleSheet(btnStyle);
    connect(openFileBtn, &QPushButton::clicked, this, &WelcomeWidget::openFileRequested);
    
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setAlignment(Qt::AlignCenter);
    btnLayout->addWidget(openFileBtn);

    mainLayout->addLayout(btnLayout);
}
