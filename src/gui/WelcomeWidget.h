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
    explicit WelcomeWidget(QWidget *parent = nullptr);

signals:
    void openFileRequested();
    void openProjectRequested();

private:
    void setupUI();
};

#endif // WELCOMEWIDGET_H
