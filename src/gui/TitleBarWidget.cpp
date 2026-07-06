#include "TitleBarWidget.h"
#include <QMouseEvent>
#include <QWindow>
#include <QIcon>
#include <QApplication>
#include <QShowEvent>
#include <QStyle>
#include <QPainter>
#include <QPixmap>

TitleBarWidget::TitleBarWidget(QWidget *parent)
    : QWidget(parent), m_titleLabel(nullptr), m_minimizeBtn(nullptr), m_maximizeBtn(nullptr), m_closeBtn(nullptr)
{
    setObjectName("titleBar");
    setFixedHeight(32);
    setupUI();

    installEventFilterOnWindow();
}

void TitleBarWidget::installEventFilterOnWindow()
{
    QWidget *topLevel = window();
    if (topLevel && topLevel != this)
        topLevel->installEventFilter(this);
}

static QIcon makeTitleBarIcon(void (*paint)(QPainter &, QColor))
{
    QPixmap pm(14, 14);
    pm.fill(Qt::transparent);
    QPainter painter(&pm);
    painter.setRenderHint(QPainter::Antialiasing);
    paint(painter, QColor(0xCC, 0xCC, 0xCC));
    painter.end();
    return QIcon(pm);
}

static void paintMinimizeIcon(QPainter &p, QColor c)
{
    p.setPen(Qt::NoPen);
    p.setBrush(c);
    int y = 10;
    p.drawRect(2, y, 10, 2);
}

static void paintMaximizeIcon(QPainter &p, QColor c)
{
    p.setPen(c);
    p.setBrush(Qt::NoBrush);
    p.drawRect(2, 2, 10, 10);
}

static void paintRestoreIcon(QPainter &p, QColor c)
{
    p.setPen(c);
    p.setBrush(Qt::NoBrush);
    p.drawRect(4, 2, 8, 8);
    p.drawRect(2, 4, 8, 8);
}

static void paintCloseIcon(QPainter &p, QColor c)
{
    p.setPen(QPen(c, 1.8));
    p.setBrush(Qt::NoBrush);
    p.drawLine(QPointF(3, 3), QPointF(11, 11));
    p.drawLine(QPointF(11, 3), QPointF(3, 11));
}

void TitleBarWidget::setupUI()
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QLabel *iconLabel = new QLabel(this);
    QPixmap iconPixmap = QIcon(":/icon/app.png").pixmap(16, 16);
    iconLabel->setPixmap(iconPixmap);
    iconLabel->setFixedSize(32, 32);
    iconLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(iconLabel);

    m_titleLabel = new QLabel("Garlic Decompiler", this);
    m_titleLabel->setContentsMargins(8, 0, 0, 0);
    m_titleLabel->setStyleSheet("font-size: 12px;");
    layout->addWidget(m_titleLabel, 1);

    layout->addStretch();

    m_minimizeBtn = new QPushButton(this);
    m_minimizeBtn->setObjectName("titleBtnMinimize");
    m_minimizeBtn->setFixedSize(46, 32);
    m_minimizeBtn->setIcon(makeTitleBarIcon(paintMinimizeIcon));
    m_minimizeBtn->setIconSize(QSize(14, 14));
    m_minimizeBtn->setToolTip("Minimize");
    connect(m_minimizeBtn, &QPushButton::clicked, this, &TitleBarWidget::minimizeClicked);

    m_maximizeBtn = new QPushButton(this);
    m_maximizeBtn->setObjectName("titleBtnMaximize");
    m_maximizeBtn->setFixedSize(46, 32);
    m_maximizeBtn->setIcon(makeTitleBarIcon(paintMaximizeIcon));
    m_maximizeBtn->setIconSize(QSize(14, 14));
    m_maximizeBtn->setToolTip("Maximize");
    connect(m_maximizeBtn, &QPushButton::clicked, this, &TitleBarWidget::maximizeClicked);

    m_closeBtn = new QPushButton(this);
    m_closeBtn->setObjectName("titleBtnClose");
    m_closeBtn->setFixedSize(46, 32);
    m_closeBtn->setIcon(makeTitleBarIcon(paintCloseIcon));
    m_closeBtn->setIconSize(QSize(14, 14));
    m_closeBtn->setToolTip("Close");
    connect(m_closeBtn, &QPushButton::clicked, this, &TitleBarWidget::closeClicked);

    layout->addWidget(m_minimizeBtn);
    layout->addWidget(m_maximizeBtn);
    layout->addWidget(m_closeBtn);

    setLayout(layout);
}

void TitleBarWidget::setTitle(const QString &title)
{
    if (m_titleLabel)
        m_titleLabel->setText(title);
}

void TitleBarWidget::updateMaximizeButton()
{
    if (!m_maximizeBtn) return;

    QWidget *win = window();
    if (win && win->isMaximized())
    {
        m_maximizeBtn->setIcon(makeTitleBarIcon(paintRestoreIcon));
        m_maximizeBtn->setIconSize(QSize(14, 14));
        m_maximizeBtn->setToolTip("Restore");
    }
    else
    {
        m_maximizeBtn->setIcon(makeTitleBarIcon(paintMaximizeIcon));
        m_maximizeBtn->setIconSize(QSize(14, 14));
        m_maximizeBtn->setToolTip("Maximize");
    }
}

void TitleBarWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    updateMaximizeButton();
}

void TitleBarWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        m_dragPos = event->globalPosition().toPoint();
    QWidget::mousePressEvent(event);
}

void TitleBarWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        if ((event->globalPosition().toPoint() - m_dragPos).manhattanLength() > 5)
        {
            QWindow *win = window()->windowHandle();
            if (win)
                win->startSystemMove();
        }
    }
    QWidget::mouseMoveEvent(event);
}

void TitleBarWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        emit maximizeClicked();
    QWidget::mouseDoubleClickEvent(event);
}

bool TitleBarWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange)
        updateMaximizeButton();
    return QWidget::eventFilter(obj, event);
}
