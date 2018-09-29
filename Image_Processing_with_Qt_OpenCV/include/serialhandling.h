#ifndef SERIALHANDLING_H
#define SERIALHANDLING_H

#include <QMainWindow>
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QFile>
#include <QString>

#include <iostream>
#include <string.h>

#define COMMAND_TAKEPHOTO       0x10
#define COMMAND_GETVERSION      0x20
#define COMMAND_SETCOMPRESSION  0x30
#define COMMAND_SWITCHCOLOR     0x40
#define COMMAND_RESET           0x50
#define COMMAND_SETBAUDRATE     0x60
#define COMMAND_SETMOTORPWM     0x70
#define COMMAND_SETMOTORDIR     0x80
#define COMMAND_SETRESOLUTION   0x90

using namespace std;

class SerialHandling : public QObject
{
    Q_OBJECT

    public:
        SerialHandling(string portName);
        ~SerialHandling();
        void initSerialPort();
        string getPortName();
        void displayAllSerialPortInfo(QList<QSerialPortInfo> allPorts);
        string getImageFilePath();

        QSerialPort *serialPort;

    private:
        QFile *imageFile;
        int fileNumber;
        string imageFilePath;
        string portName;

        uint8_t savedLastSerialByte;

        bool isReadingImage;
        bool isReadingVersion;

    public slots :
        void serialCommandTakePhoto();
        void serialCommandResetSystem();
        void serialCommandSwitchColor();
        void serialCommandGetVersion();
        void serialCommandSetCompression(int value);
        //void serialCommandSetBaudRate(int value);
        void serialCommandSetMotorPWM(int value);
        void serialCommandSetMotorDir(int value);
        void serialCommandSetResolution(QString value);
        void serialCommandCamSetBaudRate(QString value);
        void serialReadyToRead();
        void serialPCSetBaudRate(QString value);

    signals:
        //void imageFileWritten(QString path);
        void imageFileWriting();
        void newImageFilePath(QString path);
        void sendVersion(string message);
        void imageFileEnding();
};

#endif // SERIALHANDLING_H
