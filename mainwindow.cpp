#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init_window();

    timer.setInterval(1000);
    connect(&timer, &QTimer::timeout, this, &MainWindow::update_port);
    timer.start();

    connect(&serial, &QSerialPort::errorOccurred, this,&MainWindow::serialError);
    connect(&serial,SIGNAL(readyRead()),this,SLOT(recieve_data()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
bool MainWindow::isPortConnected(const QSerialPortInfo &portInfo) {
    QSerialPort port;
    port.setPort(portInfo);
    if (port.isOpen()) {

        return true;
    }

    return false;
}
void MainWindow::serialError()
{
    if(serial.error())
    {
    QString errorMessage = "Error: " + serial.errorString();
    QMessageBox::critical(this, "Error", errorMessage);
    on_connect_butt_clicked();
    }
}
void MainWindow::update_port()
{
    //Find COM on Computer

    QList<QString> strPorts;
    QList<QSerialPortInfo> currentports = info.availablePorts();
    for (int i = 0; i < currentports.size(); i++) {
        const QSerialPortInfo &portInfo = currentports.at(i);
        strPorts.append(portInfo.portName());

    }
    if (currentports.size() != ports.size() || init == true )
    {
    ui->port_cb->clear();
    ui->port_cb->addItems(strPorts);
    }
    ports = currentports;

}

void MainWindow::update_baundrate()
{
    // List all Baudrates your computer support
    QList<qint32> baudRates = info.standardBaudRates();
    QList<QString> strBaudRates;
    for(int i = 0 ; i < baudRates.size() ; i++){
        strBaudRates.append(QString::number(baudRates.at(i)));
    }
    ui->baud_cb->addItems(strBaudRates);
}
void MainWindow::init_window()
{
    setFixedSize(width(), height());
    setWindowTitle("Example UART");

    ui->parity_cb->setCurrentIndex(0);
    ui->dataBit_cb->setCurrentIndex(3);
    ui->stopBit_cb->setCurrentIndex(0);

    update_port();
    update_baundrate();

    ui->led1_butt->setDisabled(true);
    ui->led2_butt->setDisabled(true);
    ui->send_butt->setDisabled(true);
    ui->clear_butt->setDisabled(true);

    init = false;
}
void MainWindow::on_connect_butt_clicked()
{
        if (ui->connect_butt->text() == "Connect")
        {
        serial.setPortName(ui->port_cb->currentText());
        serial.open(QIODevice::ReadWrite);
            if(!serial.isOpen())
            {
            QString errorMessage = "Can't open " + ui->port_cb->currentText() + ", Error:" + serial.errorString();
            QMessageBox::critical(this, "Error", errorMessage);
            }
            else
            {
                serial.setBaudRate(ui->baud_cb->currentText().toInt());
                serial.setDataBits(QSerialPort::Data8);
                serial.setStopBits(QSerialPort::OneStop);
                serial.setParity(QSerialPort::NoParity);
                ui->connect_butt->setText("Disconnect");
                ui->connect_butt->setStyleSheet("QPushButton {color: red;}");
                ui->port_cb->setDisabled(true);
                ui->baud_cb->setDisabled(true);
                ui->dataBit_cb->setDisabled(true);
                ui->stopBit_cb->setDisabled(true);
                ui->parity_cb->setDisabled(true);
                ui->led1_butt->setDisabled(false);
                ui->led2_butt->setDisabled(false);
                ui->send_butt->setDisabled(false);
                ui->clear_butt->setDisabled(false);
                timer.stop();
            }
        }
        else
        {
            serial.close();
            ui->connect_butt->setText("Connect");
            ui->connect_butt->setStyleSheet("QPushButton {color: green;}");
            ui->port_cb->setDisabled(false);
            ui->baud_cb->setDisabled(false);
            ui->dataBit_cb->setDisabled(false);
            ui->stopBit_cb->setDisabled(false);
            ui->parity_cb->setDisabled(false);
            ui->led1_butt->setDisabled(true);
            ui->led2_butt->setDisabled(true);
            ui->send_butt->setDisabled(true);
            ui->clear_butt->setDisabled(true);
            timer.start();

        }
}


void MainWindow::on_led1_butt_clicked()
{
    if (ui->led1_butt->text() == "ON")
        {
            QString mess = "LED1 ON\n";
            serial.write(mess.toUtf8());
            ui->led1_butt->setText("OFF");
            ui->led1_butt->setStyleSheet("QPushButton {color: red;}");
        }
    else
    {
            QString mess = "LED1 OFF\n";
            serial.write(mess.toUtf8());
            ui->led1_butt->setText("ON");
            ui->led1_butt->setStyleSheet("QPushButton {color: green;}");
    }
}


void MainWindow::on_led2_butt_clicked()
{
    if (ui->led2_butt->text() == "ON")
    {
            QString mess = "LED2 ON\n";
            serial.write(mess.toUtf8());
            ui->led2_butt->setText("OFF");
            ui->led2_butt->setStyleSheet("QPushButton {color: red;}");
    }
    else
    {
            QString mess = "LED2 OFF\n";
            serial.write(mess.toUtf8());
            ui->led2_butt->setText("ON");
            ui->led2_butt->setStyleSheet("QPushButton {color: green;}");
    }
}


void MainWindow::on_send_butt_clicked()
{
    QString mess = ui->lineEdit->text();
    serial.write(mess.toUtf8());
}


void MainWindow::on_clear_butt_clicked()
{
    ui->textBrowser->clear();
}

void MainWindow::recieve_data()
{
    QByteArray data = serial.readAll();
    QString    mess = QString(data);
    ui->textBrowser->append(mess);
}
