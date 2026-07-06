#include "ThemeManager.h"
#include <QApplication>
#include <QFile>
#include <QStyleFactory>

ThemeManager &ThemeManager::instance()
{
    static ThemeManager inst;
    return inst;
}

ThemeManager::ThemeManager()
{
    m_darkPalette.setColor(QPalette::Window, QColor(25, 26, 27));
    m_darkPalette.setColor(QPalette::WindowText, QColor(204, 204, 204));
    m_darkPalette.setColor(QPalette::Base, QColor(18, 19, 20));
    m_darkPalette.setColor(QPalette::AlternateBase, QColor(25, 26, 27));
    m_darkPalette.setColor(QPalette::ToolTipBase, QColor(32, 33, 34));
    m_darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    m_darkPalette.setColor(QPalette::Text, Qt::white);
    m_darkPalette.setColor(QPalette::Button, QColor(32, 33, 34));
    m_darkPalette.setColor(QPalette::ButtonText, Qt::white);
    m_darkPalette.setColor(QPalette::BrightText, Qt::red);
    m_darkPalette.setColor(QPalette::Link, QColor(57, 148, 188));
    m_darkPalette.setColor(QPalette::Highlight, QColor(57, 148, 188));
    m_darkPalette.setColor(QPalette::HighlightedText, Qt::white);
}

void ThemeManager::init()
{
    QApplication *app = qobject_cast<QApplication *>(QCoreApplication::instance());
    if (!app) return;

    app->setStyle(QStyleFactory::create("Fusion"));
    app->setPalette(m_darkPalette);

    reloadQSS();
}

void ThemeManager::reloadQSS()
{
    QApplication *app = qobject_cast<QApplication *>(QCoreApplication::instance());
    if (!app) return;

    QFile qssFile(":/src/gui/theme.qss");
    if (qssFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString qss = qssFile.readAll();
        app->setStyleSheet(qss);
        qssFile.close();
    }
}

QColor ThemeManager::color(const QString &key) const
{
    static const QMap<QString, QColor> colors = {
        { "window.bg",         QColor(25, 26, 27) },
        { "base.bg",           QColor(18, 19, 20) },
        { "surface.bg",        QColor(32, 33, 34) },
        { "border",            QColor(51, 51, 51) },
        { "text.primary",      QColor(204, 204, 204) },
        { "text.secondary",    QColor(133, 133, 133) },
        { "text.link",         QColor(57, 148, 188) },
        { "accent",            QColor(57, 148, 188) },
        { "accent.hover",      QColor(43, 122, 158) },
        { "tab.bg",            QColor(25, 26, 27) },
        { "tab.active.bg",     QColor(18, 19, 20) },
        { "tab.active.border", QColor(57, 148, 188) },
        { "editor.bg",         QColor(18, 19, 20) },
        { "editor.fg",         QColor(212, 212, 212) },
        { "editor.selection",  QColor(38, 79, 120) },
        { "editor.line",       QColor(40, 40, 40) },
        { "editor.gutter",     QColor(30, 30, 30) },
        { "editor.gutter.fg",  QColor(133, 133, 133) },
        { "sidebar.bg",        QColor(25, 26, 27) },
        { "sidebar.hover",     QColor(32, 33, 34) },
        { "sidebar.selected",  QColor(57, 148, 188) },
        { "statusbar.bg",      QColor(18, 19, 20) },
        { "menu.bg",           QColor(32, 33, 34) },
        { "menu.hover",        QColor(57, 148, 188) },
        { "menubar.bg",        QColor(60, 60, 60) },
        { "titlebar.bg",       QColor(30, 30, 30) },
        { "titlebar.btn.hover", QColor(60, 60, 60) },
        { "titlebar.btn.close.hover", QColor(232, 17, 35) },
    };

    return colors.value(key, QColor());
}

QPalette ThemeManager::darkPalette() const
{
    return m_darkPalette;
}
