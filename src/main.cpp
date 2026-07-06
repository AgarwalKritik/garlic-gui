#include <QApplication>
#include <QDir>
#include <QIcon>
#include <QScreen>
#include <QTimer>
#include <QFontDatabase>
#include <csignal>
#include <cstdlib>
#include <fstream>
#ifdef Q_OS_MACOS
#include <QEvent>
#endif

#ifdef QT_STATIC
#include <QtPlugin>
#ifdef Q_OS_WIN
Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
#elif defined(Q_OS_MACOS)
Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin)
#elif defined(Q_OS_LINUX)
Q_IMPORT_PLUGIN(QXcbIntegrationPlugin)
#endif
#endif

#include "gui/MainWindow.h"
#include "gui/ThemeManager.h"
#include "gui/SplashWidget.h"

static void crashHandler(int sig)
{
    std::ofstream file("garlic_crash.log");
    if (file.is_open()) {
        file << "Garlic GUI crashed.\n";
        file << "Signal: " << sig << "\n";
        if (sig == SIGSEGV) file << "Cause: Segmentation fault (invalid memory access)\n";
        else if (sig == SIGABRT) file << "Cause: Aborted (assertion or internal error)\n";
        file.close();
    }
    std::_Exit(128 + sig);
}

int main(int argc, char *argv[])
{
    signal(SIGSEGV, crashHandler);
    signal(SIGABRT, crashHandler);

    QApplication app(argc, argv);

    app.setApplicationName("Garlic Decompiler");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("AbhiTheModder");
    app.setWindowIcon(QIcon(":/icon/app.png"));

    QFontDatabase::addApplicationFont(":/fonts/JetBrainsMono-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/JetBrainsMono-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/JetBrainsMono-Italic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/JetBrainsMono-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/JetBrainsMono-Medium.ttf");

    SplashWidget splash;
    splash.show();
    splash.setMessage("Loading...");
    app.processEvents();

    ThemeManager::instance().init();
    splash.setProgress(25);
    splash.setMessage("Initializing theme...");
    app.processEvents();

    MainWindow window;
    splash.setProgress(50);
    splash.setMessage("Initializing UI...");
    app.processEvents();

#ifdef Q_OS_MACOS
    QObject::connect(&app, &QGuiApplication::applicationStateChanged, [&](Qt::ApplicationState state) {
        if (state == Qt::ApplicationActive && !window.isVisible()) {
            window.show();
            window.raise();
            window.activateWindow();
        }
    });
#endif

    splash.setProgress(75);
    splash.setMessage("Ready");
    app.processEvents();

    splash.finish(&window);

#ifdef Q_OS_MACOS
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect avail = screen->availableGeometry();
        int x = avail.x() + (avail.width()  - window.width())  / 2;
        int y = avail.y() + (avail.height() - window.height()) / 2;
        window.move(x, y);
    }
#endif

    return app.exec();
}
