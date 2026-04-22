/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   03_serialport
* @brief         mainwindow.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-03-12
*******************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    /* 串口对象 */
    QSerialPort *serialPort;
    
    /* 扫描系统可用串口 */
    void scanSerialPort();

    /* 波特率项初始化 */
    void baudRateItemInit();

    /* 数据位项初始化 */
    void dataBitsItemInit();

    /* 检验位项初始化 */
    void parityItemInit();

    /* 停止位项初始化 */
    void stopBitsItemInit();

private slots:
    void sendPushButtonClicked();
    void openSerialPortPushButtonClicked();
    void serialPortReadyRead();
};
#endif // MAINWINDOW_H
