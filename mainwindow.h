#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QString>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connect_butt_clicked();

    void init_window();

    void update_port();

    void update_baundrate();

    bool isPortConnected(const QSerialPortInfo &portInfo);

    void serialError();

    void on_led1_butt_clicked();

    void on_led2_butt_clicked();

    void on_send_butt_clicked();

    void on_clear_butt_clicked();

    void recieve_data();

private:
    Ui::MainWindow *ui;

    QSerialPort serial;
    QSerialPortInfo info;
    QTimer timer;
    QList<QSerialPortInfo> ports;
    bool init = true;
};
#endif // MAINWINDOW_H
