#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qtmosq.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    qtmosq* mosq = NULL;
    QStringList subed;
    QString currentTopic;
    Ui::MainWindow *ui;

private slots:
    void setMessageStatus(bool);
    void connectPressed();
    void sendPressed();
    void connectEnabled();
    void subscribePressed();
    void subscribed();
    void connected();
    void showMessage(QString);
};

#endif // MAINWINDOW_H
