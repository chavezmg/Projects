#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <string>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSerialPort;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void on_pushButton_2_clicked();

   // void on_pushButton_2_OFF_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void serial_read();

    void on_pushButton_clicked();
    void enviarDatos();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QString portname;
    quint16 vendorId;
    quint16 productId;
    bool serial_disponible;
    void bluepill_init();

    QString serialBuffer;

};

#endif // MAINWINDOW_H
