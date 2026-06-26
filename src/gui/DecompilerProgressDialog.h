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
// File: DecompilerProgressDialog.h
// Description: Header for DecompilerProgressDialog.
//
// ==============================================================================
#ifndef DECOMPILERPROGRESSDIALOG_H
#define DECOMPILERPROGRESSDIALOG_H

#include <QDialog>
#include <QProgressBar>
#include <QLabel>
#include <QVBoxLayout>

class DecompilerProgressDialog : public QDialog
{
    Q_OBJECT

public:
    // ==============================================================================
    // Public Interface
    // ==============================================================================
    explicit DecompilerProgressDialog(QWidget *parent = nullptr);

public slots:
    // ==============================================================================
    // Public Slots
    // ==============================================================================
    void updateProgress(int progress);
    void setStatusText(const QString &text);

private:
    // ==============================================================================
    // Private Members & Methods
    // ==============================================================================
    void setupUI();

    QVBoxLayout *m_mainLayout;
    QLabel *m_titleLabel;
    QLabel *m_statusLabel;
    QProgressBar *m_progressBar;
};

#endif // DECOMPILERPROGRESSDIALOG_H