#include "SplashWidget.h"
#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QPixmap>

SplashWidget::SplashWidget(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setFixedSize(520, 400);

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);

    QWidget *container = new QWidget(this);
    container->setObjectName("splashContainer");
    container->setStyleSheet(
        "#splashContainer { background-color: #1E1E1E; border: 1px solid #333333; border-radius: 8px; }"
    );

    QVBoxLayout *layout = new QVBoxLayout(container);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(16);

    m_imageLabel = new QLabel(container);
    QPixmap pixmap(":/icon/splash.png");
    m_imageLabel->setPixmap(pixmap.scaled(480, 280, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    m_imageLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_imageLabel, 1);

    m_progressBar = new QProgressBar(container);
    m_progressBar->setRange(0, 0);
    m_progressBar->setTextVisible(false);
    m_progressBar->setFixedHeight(4);
    m_progressBar->setStyleSheet(
        "QProgressBar { background-color: #2b2b2b; border: none; border-radius: 2px; }"
        "QProgressBar::chunk { background-color: #3994BC; border-radius: 2px; }"
    );
    layout->addWidget(m_progressBar);

    m_messageLabel = new QLabel("Loading...", container);
    m_messageLabel->setAlignment(Qt::AlignCenter);
    m_messageLabel->setStyleSheet("color: #858585; font-size: 12px;");
    layout->addWidget(m_messageLabel);

    outerLayout->addWidget(container);

    QRect screen = QGuiApplication::primaryScreen()->geometry();
    move(screen.center() - rect().center());
}

void SplashWidget::setProgress(int value)
{
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(value);
}

void SplashWidget::setMessage(const QString &message)
{
    m_messageLabel->setText(message);
}

void SplashWidget::finish(QWidget *mainWindow)
{
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(100);

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(effect);
    QPropertyAnimation *anim = new QPropertyAnimation(effect, "opacity", this);
    anim->setDuration(300);
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);
    connect(anim, &QPropertyAnimation::finished, this, [this]() {
        hide();
    });
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    if (mainWindow)
        mainWindow->show();
}
