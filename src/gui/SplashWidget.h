#ifndef SPLASHWIDGET_H
#define SPLASHWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>

class SplashWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SplashWidget(QWidget *parent = nullptr);
    void setProgress(int value);
    void setMessage(const QString &message);
    void finish(QWidget *mainWindow);

private:
    QLabel *m_imageLabel;
    QLabel *m_messageLabel;
    QProgressBar *m_progressBar;
};

#endif
