#include "serialhandling.h"

SerialHandling::SerialHandling(string portName)
{
    //  List All Ports Available
    QList<QSerialPortInfo> allPorts = QSerialPortInfo::availablePorts();
    displayAllSerialPortInfo(allPorts);

    //  Initialize Available Serial Port (Open it, Set Baud Rate, Set bits)
    if(allPorts.count() > 0)
    {
        //  If Text Empty, we attribute the first port available
        if(portName.empty())
        {
            this->portName = allPorts.at(0).portName().toStdString();
            serialPort =  new QSerialPort(QString(this->portName.c_str()));
        }
        else{
            for(const QSerialPortInfo &serialPortInfo : allPorts)
            {
                if(portName == serialPortInfo.portName().toStdString())
                    serialPort = new QSerialPort(QString(portName.c_str())) ;
                else{
                    cerr << "Your Serial Port Does Not Exist"<<endl;
                    return;
                }
            }
        }

        //  Open Serial Port
        if(serialPort->open(QIODevice::ReadWrite)){
            cout<<"Opening Serial Port Succeed"<<endl;
            connect(serialPort, &QSerialPort::readyRead, this, &SerialHandling::serialReadyToRead);
        }
        else{
            cerr<<"Opening Serial Port Failed"<<endl;
            return;
        }

        serialPort->setBaudRate(QSerialPort::Baud38400);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
        savedLastSerialByte = 0x00;
        isReadingImage = false;
        fileNumber = 0;
    }
    else{
        cerr << "Error no Serial Port Available" << endl;
        return;
    }

}

SerialHandling::~SerialHandling()
{
    delete serialPort;
    delete imageFile;
}

void SerialHandling::serialReadyToRead()
{
    QByteArray serialData = serialPort->readAll();

    // DEBUG LINES
    /*
    cout<<"serial read: "<<serialData.toHex().toStdString()<<endl;
    cout<<"length: "<<serialData.length()<<endl;
    cout<<"last byte : ";
    cout<< hex <<(unsigned int)savedLastSerialByte<<endl;
    */

    /*
    if(isReadingVersion){
        string yo = "bonjour";
        emit sendVersion(yo);
    }
    */



   if(isReadingImage){
        imageFile->write(serialData);
        imageFile->flush();
        emit imageFileWriting();
        return;
    }

    //  Testing Begin/End of JPEG image : 0xFFD8/0xFFD9
    for(int i=1; i<serialData.size(); i++){
        uint8_t tmp_0 = serialData.at(i-1);
        uint8_t tmp_1 = serialData.at(i);

        if( ( tmp_0 == 0xFF && tmp_1 == 0xD9) || (savedLastSerialByte == 0xFF && tmp_0 == 0xD9 ) ){
            cout<<"Finish reading File"<<endl;
            isReadingImage = false;
            imageFile->write(serialData);
            imageFile->flush();
            emit imageFileWriting();
            fileNumber++;
            imageFile->close();
            return;
        }
        else if((tmp_0 == 0xFF && tmp_1 == 0xD8) || (savedLastSerialByte == 0xFF && tmp_0 == 0xD8 ) ){
            cout<<"Start reading File"<<endl;
            if(savedLastSerialByte == 0xFF)
                imageFile->putChar(0xFF);
            isReadingImage = true;
            imageFile->write(serialData);
            imageFile->flush();
            emit imageFileWriting();
            return;
        }
    }

    if(!serialData.isEmpty()){
        savedLastSerialByte = serialData.at( serialData.length() - 1 );
    }
}

void SerialHandling::serialCommandTakePhoto()
{
    imageFilePath = "../images/serial_Image"+to_string(fileNumber)+".jpg";
    imageFile = new QFile(QString(imageFilePath.c_str()));
    imageFile->open(QIODevice::WriteOnly);
    emit newImageFilePath(QString(imageFilePath.c_str()));

    uint8_t command = COMMAND_TAKEPHOTO;
    serialPort->putChar(command);

    cout<<"command: ";
    cout<<hex<<command;
    cout<<endl;
}

void SerialHandling::serialCommandResetSystem()
{
    uint8_t command = COMMAND_RESET;
    serialPort->putChar(command);

    cout<<"command: ";
    cout<<hex<<command;
    cout<<endl;
}

void SerialHandling::serialCommandSwitchColor()
{
    uint8_t command = COMMAND_SWITCHCOLOR;
    serialPort->putChar(command);

    cout<<"color command: ";
    cout<<hex<<command;
    cout<<endl;
}

void SerialHandling::serialCommandGetVersion()
{
    uint8_t command = COMMAND_GETVERSION;
    serialPort->putChar(command);

    cout<<"command: ";
    cout<<hex<<command;
    cout<<endl;
}

void SerialHandling::serialCommandSetCompression(int value)
{
    uint8_t arg = (unsigned int)value;
    uint8_t command = COMMAND_SETCOMPRESSION;

    serialPort->putChar( command | arg );

    cout<<"compression command: ";
    cout<<hex<<(command | arg);
    cout<<endl;
}

void SerialHandling::serialCommandCamSetBaudRate(QString value)
{
    int tmp;
    switch(value.toInt()){
        case 1200:
                    tmp = 0;
                    break;
        case 2400:
                    tmp = 1;
                    break;
        case 4800:
                    tmp = 2;
                    break;
    }

    uint8_t arg = (uint8_t)tmp;
    uint8_t command = COMMAND_SETBAUDRATE;
    serialPort->putChar( command | arg );

    cout<<"command: ";
    cout<<hex<<(command | arg);
    cout<<endl;
}

void SerialHandling::serialCommandSetMotorPWM(int value)
{
    uint8_t arg = (uint8_t)value;
    uint8_t command = COMMAND_SETMOTORPWM;
    serialPort->putChar( command | arg );

    cout<<"motorPWM command: ";
    cout<<hex<<(command | arg);
    cout<<endl;
}

void SerialHandling::serialCommandSetMotorDir(int value)
{
    uint8_t arg = (uint8_t)value;
    uint8_t command = COMMAND_SETMOTORDIR;
    serialPort->putChar( command | arg );

    /*
    cout<<"command: ";
    cout<<hex<<command;
    cout<<endl<<"arg :";
    cout<<hex<<arg;
    cout<<endl<<"command | arg :";
    cout<<hex<<(command | arg);
    cout<<endl;
    */
    cout<<"motorDir command: ";
    cout<<hex<<(command | arg);
    cout<<endl;
}

void SerialHandling::serialPCSetBaudRate(QString value)
{
    serialPort->setBaudRate(value.toInt());
    cout<<"PC Baud Rate Set to : "<<value.toStdString()<<endl;
}

void SerialHandling::serialCommandSetResolution(QString value)
{
    int tmp;
    if(value == "640x480")
        tmp = 0;
    else if(value == "320x280")
        tmp = 1;
    else if(value == "160x120")
        tmp= 2;
    else
        tmp = 0;

    uint8_t arg = (uint8_t)tmp;
    uint8_t command = COMMAND_SETRESOLUTION;
    serialPort->putChar( command | arg );

    cout<<"resolution command: ";
    cout<<hex<<(command | arg);
    cout<<endl;
}

void SerialHandling::displayAllSerialPortInfo(QList<QSerialPortInfo> allPorts)
{
    //const auto serialPortInfos = QSerialPortInfo::availablePorts();
    const string blankString = "N/A";
    string descriptions, manufacturers, serialNumbers, portNames, systemLocations;

    //  List All Available Ports
    cout << "Total number of ports available: " << allPorts.count() << endl;
    for (const QSerialPortInfo &serialPortInfo : allPorts) {

        descriptions = serialPortInfo.description().toStdString();
        manufacturers = serialPortInfo.manufacturer().toStdString();
        serialNumbers = serialPortInfo.serialNumber().toStdString();
        portNames = serialPortInfo.portName().toStdString();
        systemLocations = serialPortInfo.systemLocation().toStdString();

        cout << "Port: "            << portNames                                                  << endl
             << "Location: "        << systemLocations                                            << endl
             << "Description: "     << (!descriptions.empty() ? descriptions : blankString)       << endl
             << "Manufacturer: "    << (!manufacturers.empty() ? manufacturers : blankString)     << endl
             << "Serial number: "   << (!serialNumbers.empty() ? serialNumbers : blankString)     << endl
             << "Busy: "            << (serialPortInfo.isBusy() ? "Yes" : "No")                   << endl << endl;
    }
}

string SerialHandling::getPortName()
{
    return portName;
}

string SerialHandling::getImageFilePath()
{
    return imageFilePath;
}








/*
void MainWindow::serialCommandTakePhoto()
{

    imageFile = new QFile("../serialImage.jpg");
    imageFile->open(QIODevice::WriteOnly);
    serialPort->write("c");

    connect(serialPort, SIGNAL(readyRead()), this, SLOT(serialReadyToRead()));

}

void MainWindow::serialReadyToRead()
{

    QByteArray serialData = serialPort->readAll();

    if(!serialData.isEmpty())
        savedLastSerialByte = serialData.at(serialData.length()-1);

    //  DEBUG LINES
    //cout<<"serial read: "<<serialData.toHex().toStdString()<<endl;
    //cout<<"length: "<<serialData.length()<<endl;
    //cout<<"last byte : ";
    //cout<< hex <<(unsigned int)savedLastSerialByte<<endl;



    //  Testing End of JPEG image : 0xFFD9
    for(int i=1; i<serialData.size(); i++){
        uint8_t tmp_0 = serialData.at(i-1);
        uint8_t tmp_1 = serialData.at(i);

        if((tmp_0 == 0xFF && tmp_1 == 0xD9) || (tmp_0 == 0xD9 && savedLastSerialByte == 0xFF) ){
            disconnect(serialPort, SIGNAL(readyRead()), this, SLOT(serialReadyToRead()));
            imageFile->write(serialData);
            imageFile->flush();
            imageFile->close();
            return;
        }

    }

    imageFile->write(serialData);
    imageFile->flush();


    srcImage = imread("../serialImage.jpg");
    sourceCallback();
}
*/
