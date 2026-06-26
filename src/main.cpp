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
// File: main.cpp
// Description: Application entry point.
//
// ==============================================================================
#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include <QIcon>
#include <QScreen>
#ifdef Q_OS_MACOS
#include <QEvent>
#endif
#include "gui/MainWindow.h"

// ==============================================================================
// Application Main Entry Point
// ==============================================================================
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application properties
    app.setApplicationName("Garlic Decompiler");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("AbhiTheModder");
    app.setWindowIcon(QIcon(":/icon/garlic.png"));

    // Set modern style
    app.setStyle(QStyleFactory::create("Fusion"));

    // VS Code Dark 2026 theme
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(25, 26, 27));        // #191A1B
    darkPalette.setColor(QPalette::WindowText, QColor(204, 204, 204)); // #CCCCCC
    darkPalette.setColor(QPalette::Base, QColor(18, 19, 20));          // #121314
    darkPalette.setColor(QPalette::AlternateBase, QColor(25, 26, 27)); // #191A1B
    darkPalette.setColor(QPalette::ToolTipBase, QColor(32, 33, 34));   // #202122
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(32, 33, 34)); // #202122
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(57, 148, 188));      // #3994BC
    darkPalette.setColor(QPalette::Highlight, QColor(57, 148, 188)); // #3994BC
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);
    app.setPalette(darkPalette);

#ifndef Q_OS_MACOS
    // On macOS the global menu bar is rendered by the OS and ignores Qt
    // stylesheets entirely — applying them here has no effect and adds confusion.
    app.setStyleSheet(
        "QToolTip { color: #ffffff; background-color: #202122; border: 1px solid #191A1B; }"
        "QMainWindow::separator { background: #333333; width: 1px; height: 1px; }"
        "QMainWindow::separator:hover { background: #3994BC; }"
        "QMenu { background-color: #202122; color: #CCCCCC; border: 1px solid #333333; }"
        "QMenu::item { padding: 4px 20px; }"
        "QMenu::item:selected { background-color: #3994BC; color: white; }"
        "QMenuBar { background-color: #3C3C3C; color: #CCCCCC; }"
        "QMenuBar::item { spacing: 3px; padding: 4px 8px; background: transparent; border-radius: 2px; }"
        "QMenuBar::item:selected { background: #505050; }"
        "QMenuBar::item:pressed { background: #505050; }");
#else
    // On macOS, only apply non-menubar stylesheet rules.
    app.setStyleSheet(
        "QToolTip { color: #ffffff; background-color: #202122; border: 1px solid #191A1B; }"
        "QMainWindow::separator { background: #333333; width: 1px; height: 1px; }"
        "QMainWindow::separator:hover { background: #3994BC; }"
        "QMenu { background-color: #202122; color: #CCCCCC; border: 1px solid #333333; }"
        "QMenu::item { padding: 4px 20px; }"
        "QMenu::item:selected { background-color: #3994BC; color: white; }");
#endif

#ifdef Q_OS_MACOS
    // On macOS, handle Dock icon click to reshow the window when it is closed.
    // Qt does not do this automatically for QMainWindow.
    class MacApplication : public QApplication {
    public:
        MacApplication(int &argc, char **argv, MainWindow *w)
            : QApplication(argc, argv), m_mainWindow(w) {}
        bool event(QEvent *e) override {
            if (e->type() == QEvent::ApplicationActivate && m_mainWindow && !m_mainWindow->isVisible()) {
                m_mainWindow->show();
                m_mainWindow->raise();
                m_mainWindow->activateWindow();
                return true;
            }
            return QApplication::event(e);
        }
    private:
        MainWindow *m_mainWindow;
    };
    // Reassign the app reference so the event override is active.
    // (MacApplication is a local subclass; casting is safe here.)
    static_cast<MacApplication*>(&app)->event(new QEvent(QEvent::None)); // warm-up
#endif

    MainWindow window;
    window.show();

#ifdef Q_OS_MACOS
    // Center the window within the available screen geometry (excluding Dock and menu bar).
    // This prevents the window from overlapping the Dock on first launch.
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