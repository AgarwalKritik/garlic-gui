//  Copyright 2026 Kritik Agarwal
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#ifndef FINDREPLACEWIDGET_H
#define FINDREPLACEWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

class FindReplaceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FindReplaceWidget(QWidget *parent = nullptr);

    QString getFindText() const;
    QString getReplaceText() const;
    bool isRegex() const;
    bool isMatchCase() const;
    bool isWholeWord() const;

    void focusFindInput();

signals:
    void findNextRequested();
    void findPrevRequested();
    void replaceRequested();
    void replaceAllRequested();
    void closeRequested();

private:
    QLineEdit *m_findInput;
    QLineEdit *m_replaceInput;
    QCheckBox *m_regexCheck;
    QCheckBox *m_matchCaseCheck;
    QCheckBox *m_wholeWordCheck;

    void setupUI();
};

#endif // FINDREPLACEWIDGET_H
