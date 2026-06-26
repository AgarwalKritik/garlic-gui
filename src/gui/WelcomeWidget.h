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
// File: WelcomeWidget.h
// Description: Header for WelcomeWidget.
//
// ==============================================================================
#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPixmap>
#include <QPushButton>

class WelcomeWidget : public QWidget
{
    Q_OBJECT

public:
    // ==============================================================================
    // Public Interface
    // ==============================================================================
    explicit WelcomeWidget(QWidget *parent = nullptr);

signals:
    // ==============================================================================
    // Signals
    // ==============================================================================
    void openFileRequested();

private:
    // ==============================================================================
    // Private Members & Methods
    // ==============================================================================
    void setupUI();
};

#endif // WELCOMEWIDGET_H
