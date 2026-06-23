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
