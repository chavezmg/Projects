#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QList>
#include<QDebug>
#include <string>
#include <iostream>

char salidas[20]= "0000\n";
char enviar[20];
int val= 0;
float value_slider;
bool control_remoto = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    serial = new QSerialPort();
    serial_disponible = false;
    serialBuffer = "";

    ui->progressBar->setRange(0,100);
    ui->horizontalSlider->setRange(0,100);
    ui->progressBar->setValue(0);
    connect(ui->horizontalSlider, &QSlider::valueChanged, ui->progressBar, &QProgressBar::setValue);


    foreach(const QSerialPortInfo &serial_info, QSerialPortInfo::availablePorts()){
        qDebug()<<"Puerto: "<<serial_info.portName();
        portname = serial_info.portName();
        qDebug()<<"Vendor Id: " <<serial_info.vendorIdentifier();
        vendorId = serial_info.vendorIdentifier();
        qDebug()<<"Product Id:" <<serial_info.productIdentifier();
        productId = serial_info.productIdentifier();
        serial_disponible = true;
    }
    if(serial_disponible){
        bluepill_init();
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::bluepill_init()
{
    serial->setPortName(portname);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->open(QIODevice::ReadWrite);
    connect(serial, SIGNAL(readyRead()), this, SLOT(serial_read()));
}

void MainWindow::on_pushButton_2_clicked()
{
        enviarDatos();
        //qDebug()<<"Salidas:"<<salidas;
}


void MainWindow::serial_read()
{
    //QStringList bufferSplit =
    if(serial->isWritable()&&serial_disponible){
        QByteArray serialData = serial->readAll();
        serialBuffer += QString::fromStdString(serialData.toStdString());
        QStringList bufferSplit = serialBuffer.split("\n\r");
        if(bufferSplit.length()<3){
            serialData = serial->readAll();
            serialBuffer += QString::fromStdString(serialData.toStdString());
        }
        else{
            QStringList dataIn = bufferSplit[1].split(',');

            ui->lcdNumber_2->display(bufferSplit[1]);//string que llega al qt
            qDebug()<<"Datos que llegan serial:"<<bufferSplit[1];

            if(dataIn[0] == '0'){
                ui->label_3->setText("Enable");
                }
             else if(dataIn[0] == '1'){
                ui->label_3->setText("Disable");
                }

            val = dataIn[1].toInt();

            if(!control_remoto){
                value_slider = ((dataIn[1].toInt() * 100) / 4096);
                ui->horizontalSlider->setValue(value_slider);
                ui->lcdNumber_3->display(dataIn[1].toInt());
                ui->lcdNumber->display((val * 180) / 4096);
            }
            serialBuffer = "";
        }
    }

}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    val=value*1.8;
    val = (val * 4096) / 180;
    ui->lcdNumber_3->display(val);

    if(val<10){
            sprintf(salidas, "000%d\n", val);
    }
    else if(val<100){
            sprintf(salidas, "00%d\n", val);
    }
    else if(val<1000){
            sprintf(salidas, "0%d\n", val);
    }
    else if(val>1000){
            sprintf(salidas, "%d\n", val);
    }

    enviarDatos();
}

void MainWindow::on_pushButton_clicked()
{
    if(control_remoto){
        control_remoto = false;
        ui->label_4->setText("OFF");
        ui->horizontalSlider->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
    }
    else{
        control_remoto = true;
        ui->label_4->setText("ON");
        ui->horizontalSlider->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
    }
    enviarDatos();
}


void MainWindow::enviarDatos(){
    if(!control_remoto){
        ui->lcdNumber->display((val * 180) / 4096);
    }

    sprintf(enviar, "%d,%c%c%c%c", control_remoto, salidas[0],salidas[1],salidas[2],salidas[3]);
    serial->write(enviar);
    qDebug()<<"Enviado:"<<enviar;
}
