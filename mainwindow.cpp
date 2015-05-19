#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->send->setDisabled(true);
    ui->message->setDisabled(true);
    ui->subscribe->setDisabled(true);
    ui->topic->setDisabled(true);

    ui->lineIP->setText(QString("plainstar.tk"));
    ui->linePort->setText(QString("1883"));

    lib_init();

    connect (ui->connect, SIGNAL(clicked()), this, SLOT(connectPressed()));
    connect (ui->send, SIGNAL(clicked()), this, SLOT(sendPressed()));
    connect (ui->subscribe, SIGNAL(clicked()), this, SLOT(subscribePressed()));
}

void MainWindow::showMessage(QString message)
{
    ui->text->setText(message + tr("\n") + ui->text->text());
}

void MainWindow::connectPressed()
{
    QByteArray host = ui->lineIP->text().toLocal8Bit();
    QByteArray id = ui->lineID->text().toLocal8Bit();

    int port = 0;
    port = ui->linePort->text().toInt();
    if (!port)
    {
        ui->linePort->clear();
        return ;
    }
    ui->connect->setDisabled(true);

    mosq = new qtmosq(id, false);
    connect (mosq, SIGNAL(connectEnable()), this, SLOT(connectEnabled()));
    connect (mosq, SIGNAL(subscribed()), this ,SLOT(subscribed()));
    connect (mosq, SIGNAL(connected()), this, SLOT(connected()));
    connect (mosq, SIGNAL(messageSent(bool)), this, SLOT(setMessageStatus(bool)));
    connect (mosq, SIGNAL(messageReceived(QString)), this, SLOT(showMessage(QString)));
    mosq->connect_async(host.data(), port);
    mosq->loop_start();
}

void MainWindow::subscribePressed()
{
    setMessageStatus(false);
    currentTopic = ui->topic->text();
    QByteArray topic = currentTopic.toLocal8Bit();

    mosq->subscribe(mosq->getMID(), topic.data(), 2);
}

void MainWindow::sendPressed()
{
    setMessageStatus(false);
    QByteArray payload = QString("[" + ui->lineID->text() + "] " + ui->message->text()).toLocal8Bit();
    currentTopic = ui->topic->text();
    QByteArray topic = currentTopic.toLocal8Bit();

    if (!subed.contains(currentTopic)) mosq->subscribe(mosq->getMID(), topic.data(), 2);
    mosq->publish(mosq->getMID(), topic.data(), payload.size(), payload.data(), 2, false);
}

void MainWindow::connected()
{
    ui->lineID->setDisabled(true);
    ui->lineIP->setDisabled(true);
    ui->linePort->setDisabled(true);
    setMessageStatus(true);
}
void MainWindow::setMessageStatus(bool o)
{
    ui->message->setEnabled(o);
    ui->send->setEnabled(o);
    ui->subscribe->setEnabled(o);
    ui->topic->setEnabled(o);
}
void MainWindow::subscribed()
{
    ui->text->setText(QString("Topic ") + currentTopic + QString(" has been subscribed.\n" + ui->text->text()));
    subed.append(currentTopic);
    setMessageStatus(true);
}
void MainWindow::connectEnabled() {ui->connect->setEnabled(true);}

MainWindow::~MainWindow()
{
    mosq->mosquittopp::disconnect();
    mosq->loop_stop();
    lib_cleanup();
    delete mosq;
    delete ui;
}
