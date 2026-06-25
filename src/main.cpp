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

    // Global stylesheet for consistent flat look
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

    MainWindow window;
    window.show();

    return app.exec();
}