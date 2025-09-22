#ifndef DECOMPILERPROGRESSDIALOG_H
#define DECOMPILERPROGRESSDIALOG_H

#include <QDialog>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class DecompilerProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DecompilerProgressDialog(QWidget *parent = nullptr);

public slots:
    void updateProgress(int progress);
    void setStatusText(const QString &text);

private slots:
    void onCancelClicked();

signals:
    void cancelled();

private:
    void setupUI();

    QVBoxLayout *m_mainLayout;
    QLabel *m_titleLabel;
    QLabel *m_statusLabel;
    QProgressBar *m_progressBar;
    QPushButton *m_cancelButton;
};

#endif // DECOMPILERPROGRESSDIALOG_H