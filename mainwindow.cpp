/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   03_serialport
* @brief         mainwindow.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-03-12
*******************************************************************/
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>

#define SERIALPORT_QT_VERSION "1.0.0"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QDebug() << "Serialport_qt: " << SERIALPORT_QT_VERSION;

    /* 如果是ARM平台，直接设置大小为屏幕的大小 */
#if __arm__
    /* 获取屏幕的分辨率，Qt官方建议使用这
     * 种方法获取屏幕分辨率，防上多屏设备导致对应不上
     */
    QList <QScreen *> list_screen = QGuiApplication::screens();
    this->resize(list_screen.at(0)->geometry().width(),
                 list_screen.at(0)->geometry().height());
#endif

    serialPort = new QSerialPort(this);

    /* 扫描系统的串口 */
    scanSerialPort();

    /* 波特率项初始化 */
    baudRateItemInit();

    /* 数据位项初始化 */
    dataBitsItemInit();

    /* 检验位项初始化 */
    parityItemInit();

    /* 停止位项初始化 */
    stopBitsItemInit();

    /* 信号槽连接 */
    connect(ui->pushButton_0, SIGNAL(clicked()),
            this, SLOT(sendPushButtonClicked()));
    connect(ui->pushButton_1, SIGNAL(clicked()),
            this, SLOT(openSerialPortPushButtonClicked()));

    connect(serialPort, SIGNAL(readyRead()),
            this, SLOT(serialPortReadyRead()));
}

void MainWindow::scanSerialPort()
{
    /* 查找可用串口 */
    foreach (const QSerialPortInfo &info,
            QSerialPortInfo::availablePorts()) {
        ui->comboBox_0->addItem(info.portName());
    }
}

void MainWindow::baudRateItemInit()
{
    /* QList链表，字符串类型 */
    QList <QString> list;
    list<<"1200"<<"2400"<<"4800"<<"9600"
       <<"19200"<<"38400"<<"57600"
      <<"115200"<<"230400"<<"460800"
     <<"921600";
    for (int i = 0; i < 11; i++) {
        ui->comboBox_1->addItem(list[i]);
    }
    ui->comboBox_1->setCurrentIndex(7);
}

void MainWindow::dataBitsItemInit()
{
    /* QList链表，字符串类型 */
    QList <QString> list;
    list<<"5"<<"6"<<"7"<<"8";
    for (int i = 0; i < 4; i++) {
        ui->comboBox_2->addItem(list[i]);
    }
    ui->comboBox_2->setCurrentIndex(3);
}

void MainWindow::parityItemInit()
{
    /* QList链表，字符串类型 */
    QList <QString> list;
    list<<"None"<<"Even"<<"Odd"<<"Space"<<"Mark";
    for (int i = 0; i < 5; i++) {
        ui->comboBox_3->addItem(list[i]);
    }
    ui->comboBox_3->setCurrentIndex(0);
}

void MainWindow::stopBitsItemInit()
{
    /* QList链表，字符串类型 */
    QList <QString> list;
    list<<"1"<<"2";
    for (int i = 0; i < 2; i++) {
        ui->comboBox_4->addItem(list[i]);
    }
    ui->comboBox_4->setCurrentIndex(0);
}

void MainWindow::sendPushButtonClicked()
{
    /* 获取textEdit数据,转换成utf8格式的字节流 */
    QByteArray data = ui->textEdit->toPlainText().toUtf8();
    // data.append("\r\n");
    serialPort->write(data);
    //ui->textBrowser->insertPlainText(QString::fromUtf8(data));
}

void MainWindow::openSerialPortPushButtonClicked()
{
    if (ui->pushButton_1->text() == "Open") {
        /* 设置串口名 */
        serialPort->setPortName(ui->comboBox_0->currentText());
        /* 设置波特率 */
        serialPort->setBaudRate(ui->comboBox_1->currentText().toInt());
        /* 设置数据位数 */
        switch (ui->comboBox_2->currentText().toInt()) {
        case 5:
            serialPort->setDataBits(QSerialPort::Data5);
            break;
        case 6:
            serialPort->setDataBits(QSerialPort::Data6);
            break;
        case 7:
            serialPort->setDataBits(QSerialPort::Data7);
            break;
        case 8:
            serialPort->setDataBits(QSerialPort::Data8);
            break;
        default: break;
        }
        /* 设置奇偶校验 */
        switch (ui->comboBox_3->currentIndex()) {
        case 0:
            serialPort->setParity(QSerialPort::NoParity);
            break;
        case 1:
            serialPort->setParity(QSerialPort::EvenParity);
            break;
        case 2:
            serialPort->setParity(QSerialPort::OddParity);
            break;
        case 3:
            serialPort->setParity(QSerialPort::SpaceParity);
            break;
        case 4:
            serialPort->setParity(QSerialPort::MarkParity);
            break;
        default: break;
        }
        /* 设置停止位 */
        switch (ui->comboBox_4->currentText().toInt()) {
        case 1:
            serialPort->setStopBits(QSerialPort::OneStop);
            break;
        case 2:
            serialPort->setStopBits(QSerialPort::TwoStop);
            break;
        default: break;
        }
        /* 设置流控制 */
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
        if (!serialPort->open(QIODevice::ReadWrite)){
            perror("open failed");
            QMessageBox::about(NULL, "错误",
                               "串口无法打开！可能串口已经被占用！");

        } else {
            ui->comboBox_0->setEnabled(false);
            ui->comboBox_1->setEnabled(false);
            ui->comboBox_2->setEnabled(false);
            ui->comboBox_3->setEnabled(false);
            ui->comboBox_4->setEnabled(false);
            ui->pushButton_1->setText("Close");
            ui->pushButton_0->setEnabled(true);
        }
    } else {
        serialPort->close();
        ui->comboBox_0->setEnabled(true);
        ui->comboBox_1->setEnabled(true);
        ui->comboBox_2->setEnabled(true);
        ui->comboBox_3->setEnabled(true);
        ui->comboBox_4->setEnabled(true);
        ui->pushButton_1->setText("Open");
        ui->pushButton_0->setEnabled(false);
    }
}

void MainWindow::serialPortReadyRead()
{
    /* 接收缓冲区中读取数据 */
    QByteArray buf = serialPort->readAll();
    ui->textBrowser->insertPlainText(QString(buf));
}

MainWindow::~MainWindow()
{
    delete ui;
}
