#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initUI();

    initImage(ui->lineEditFilePath->text());

    sourceCallback();
    blurCallback();
    cannyCallback();
    houghCallback();

    initParameters();
}

MainWindow::~MainWindow()
{
    delete serial;
    delete ui;
}

void MainWindow::initUIConnection()
{
    connect(ui->gaussKernelSizeSlider,          SIGNAL(valueChanged(int)),   this,    SLOT(blurCallback())           );
    connect(ui->gaussSigmaSlider,               SIGNAL(valueChanged(int)),   this,    SLOT(blurCallback())           );
    connect(ui->cannyLowThresholdSlider,        SIGNAL(valueChanged(int)),   this,    SLOT(cannyCallback())          );
    connect(ui->cannyMaxThresholdSlider,        SIGNAL(valueChanged(int)),   this,    SLOT(cannyCallback())          );
    connect(ui->cannyApertureSizeSlider,        SIGNAL(valueChanged(int)),   this,    SLOT(cannyCallback())          );
    connect(ui->houghThresholdSlider,           SIGNAL(valueChanged(int)),   this,    SLOT(houghCallback())          );
    connect(ui->houghRhoResolutionSlider,       SIGNAL(valueChanged(int)),   this,    SLOT(houghCallback())          );
    connect(ui->houghThetaMinSlider,            SIGNAL(valueChanged(int)),   this,    SLOT(houghCallback())          );
    connect(ui->houghThetaMaxSlider,            SIGNAL(valueChanged(int)),   this,    SLOT(houghCallback())          );
    connect(ui->houghThetaResolutionSlider,     SIGNAL(valueChanged(int)),   this,    SLOT(houghCallback())          );
    connect(ui->checkBoxScaledContent,          SIGNAL(clicked(bool)    ),   this,    SLOT(scalingContent())         );
    connect(ui->buttonImageFile,                SIGNAL(clicked(bool)    ),   this,    SLOT(setStreamEntry())         );
    connect(ui->buttonSerialPort,               SIGNAL(clicked(bool)    ),   this,    SLOT(setStreamEntry())         );
    connect(ui->buttonMotorDir,                 SIGNAL(toggled(bool)    ),   this,    SLOT(motorButtonClicked(bool)) );
    connect(ui->buttonFilePath,                 SIGNAL(clicked(bool)    ),   this,    SLOT(chooseImageFilePath())    );
    connect(ui->lineEditFilePath,               SIGNAL(editingFinished()),   this,    SLOT(initImage())              );

    ui->comboBoxResolution->addItem( QString("640x480") );
    ui->comboBoxResolution->addItem( QString("320x280") );
    ui->comboBoxResolution->addItem( QString("160x120") );

    for(int i=0; i<6; i++){
        ui->comboBoxPCBaudRate->addItem(  QString::number(1200*pow(2, i)) );
        ui->comboBoxCamBaudRate->addItem( QString::number(1200*pow(2, i)) );
    }
    for(int i=1; i<2; i++){
        ui->comboBoxPCBaudRate->addItem( QString::number(57600*pow(2, i)) );
        ui->comboBoxCamBaudRate->addItem( QString::number(57600*pow(2, i)) );
    }
    ui->comboBoxPCBaudRate->setCurrentText( QString::number(1200*pow(2, 5)) );
    ui->comboBoxCamBaudRate->setCurrentText( QString::number(1200*pow(2, 5)) );

    //  Display Slider Values in Label
    connect(ui->sliderMotor,                    SIGNAL(valueChanged(int)),      ui->labelMotorPWM,                   SLOT(setNum(int))   );
    connect(ui->gaussKernelSizeSlider,          SIGNAL(valueChanged(int)),      ui->labelGaussKernelSizeValue,       SLOT(setNum(int))   );
    connect(ui->gaussSigmaSlider,               SIGNAL(valueChanged(int)),      ui->labelGaussSigmaValue,            SLOT(setNum(int))   );
    connect(ui->cannyLowThresholdSlider,        SIGNAL(valueChanged(int)),      ui->labelCannyLowThresholdValue,     SLOT(setNum(int))   );
    connect(ui->cannyMaxThresholdSlider,        SIGNAL(valueChanged(int)),      ui->labelCannyMaxThresholdValue,     SLOT(setNum(int))   );
    connect(ui->cannyApertureSizeSlider,        SIGNAL(valueChanged(int)),      ui->labelCannyApertureSizeValue,     SLOT(setNum(int))   );
    connect(ui->houghThresholdSlider,           SIGNAL(valueChanged(int)),      ui->labelHoughThresholdValue,        SLOT(setNum(int))   );
    connect(ui->houghThetaResolutionSlider,     SIGNAL(valueChanged(int)),      ui->labelHoughThetaResolutionValue,  SLOT(setNum(int))   );
    connect(ui->houghThetaMaxSlider,            SIGNAL(valueChanged(int)),      ui->labelHoughThetaMaxValue,         SLOT(setNum(int))   );
    connect(ui->houghThetaMinSlider,            SIGNAL(valueChanged(int)),      ui->labelHoughThetaMinValue,         SLOT(setNum(int))   );
    connect(ui->houghRhoResolutionSlider,       SIGNAL(valueChanged(int)),      ui->labelHoughRhoResolutionValue,    SLOT(setNum(int))   );
}

void MainWindow::initUI()
{
    setStreamEntry();
    initUIConnection();
}

void MainWindow::initSerialConnection()
{
    connect( ui->buttonTakePhoto,    &QPushButton::clicked,             this->serial,   &SerialHandling::serialCommandTakePhoto     );
    connect( ui->buttonGetVersion,   &QPushButton::clicked,             this->serial,   &SerialHandling::serialCommandGetVersion    );
    connect( ui->buttonSwitchColor,  &QPushButton::clicked,             this->serial,   &SerialHandling::serialCommandSwitchColor   );
    connect( ui->buttonReset,        &QPushButton::clicked,             this->serial,   &SerialHandling::serialCommandResetSystem   );
    connect( ui->buttonMotorDir,     &QPushButton::clicked,             this->serial,   &SerialHandling::serialCommandSetMotorDir   );
    connect( ui->sliderMotor,        &QSlider::valueChanged,            this->serial,   &SerialHandling::serialCommandSetMotorPWM   );
    connect( ui->comboBoxResolution, &QComboBox::editTextChanged,       this->serial,   &SerialHandling::serialCommandSetResolution );

    connect( ui->comboBoxResolution, SIGNAL(activated(QString)),        this->serial,   SLOT(serialCommandSetResolution(QString))   );
    connect( ui->comboBoxCamBaudRate,SIGNAL(activated(QString)),        this->serial,   SLOT(serialCommandCamSetBaudRate(QString))  );
    connect( ui->comboBoxPCBaudRate, SIGNAL(activated(QString)),        this->serial,   SLOT(serialPCSetBaudRate(QString))          );

    connect( this->serial,          SIGNAL(imageFileWriting()),          this,         SLOT(displaySourceImage())                 );
    connect( this->serial,          SIGNAL(newImageFilePath(QString)),   this,         SLOT(writeFilePathLabel(QString))          );
    connect( this->serial,          SIGNAL(sendVersion(string) ),        this,         SLOT(openVersionMessage(string))           );

    connect( ui->spinnerCompression, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this->serial, &SerialHandling::serialCommandSetCompression  );
    //connect( ui->spinnerPCBaudRate,  static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this->serial, &SerialHandling::serialPCSetBaudRate          );
}

void MainWindow::initImage(QString path)
{
    srcImagePath = path;
    srcImage = imread(path.toStdString());
    houghImage = srcImage.clone();
}


void MainWindow::initImage()
{
    initImage(ui->lineEditFilePath->text());
}


void MainWindow::writeFilePathLabel(QString path)
{
    ui->lineEditFilePath->setText(path);
    srcImagePath = path;
}

void MainWindow::displaySourceImage()
{
    srcImage = imread(srcImagePath.toStdString());
    ui->imageSource->setPixmap( QPixmap::fromImage(cvMatToQImage(srcImage)) );
   initParameters();
}

void MainWindow::initParameters()
{
/*
    ui->houghRhoResolutionSlider->setMaximum((int)srcImage.cols/10);
    ui->houghThetaResolutionSlider->setMaximum((int)srcImage.cols/10);
    ui->houghThresholdSlider->setMaximum(srcImage.cols);
    */

    /*ui->cannyMaxThresholdSlider->setMaximum(srcImage.cols);
    ui->cannyLowThresholdSlider->setValue(100);
    ui->cannyMaxThresholdSlider->setValue(200);

    ui->houghThresholdSlider->setValue(150);
    ui->houghThetaMaxSlider->setValue(145);
    ui->houghThetaMinSlider->setValue(75);*/

    //ui->houghThresholdSlider->setValue(srcImage.cols/3);

    ui->labelImageChannels->setText( QString::number(srcImage.channels())               );
    ui->labelImageColumns->setText(  QString::number(srcImage.cols)                     );
    ui->labelImageRows->setText(     QString::number(srcImage.rows)                     );
    ui->labelImageDepth->setText(    QString::number(srcImage.depth())                  );
    ui->labelImageSize->setText(     QString::number(QFile(srcImagePath).size()) +" kB" );


    //ui->gaussKernelSizeSlider->setValue(3);
}

void MainWindow::setStreamEntry()
{
    //  Set Image File as Source
    if(ui->buttonImageFile->isChecked())
    {
        initImage(ui->lineEditFilePath->text());
    }

    //  Set Serial Port as Source
    else if(ui->buttonSerialPort->isChecked())
    {
        serial = new SerialHandling(ui->lineEditSerialPort->text().toStdString());
        ui->lineEditSerialPort->setText(QString(serial->getPortName().c_str()) );
        //if(serial->serialPort){
            initSerialConnection();
       // }
    }
}

void MainWindow::sourceCallback()
{
    ui->imageSource->setPixmap( QPixmap::fromImage(cvMatToQImage(srcImage)) );

    //  Updating Gauss
    blurCallback();

    //  Updating Canny
    cannyCallback();

    //  Updating Hough
    houghCallback();
}

void MainWindow::houghCallback()
{
    houghImage = srcImage.clone();

    //  Setting values
    houghRhoResolution   = ui->houghRhoResolutionSlider->value();
    houghThreshold       = ui->houghThresholdSlider->value();
    houghThetaResolution = CV_PI/180 * ui->houghThetaResolutionSlider->value();
    houghThetaMin        = CV_PI/180 * ui->houghThetaMinSlider->value();
    houghThetaMax        = CV_PI/180 * ui->houghThetaMaxSlider->value();

    //	Should always Min < Max
    if(houghThetaMin >= houghThetaMax){
        double tmp;
        tmp = houghThetaMin;
        houghThetaMin = houghThetaMax;
        houghThetaMax = tmp;
    }

    //  Processing Hough Line Detection
    HoughLines(cannyImage, lines, houghRhoResolution, houghThetaResolution, houghThreshold, 0, 0, houghThetaMin, houghThetaMax );

   /*	Parameters tested :
    *		Canny : lowThreshold : 160 - maxThreshold : 220
    *		Hough : minTheta : 960 - maxTheta : 1200 threshold : 20 - thetaResolution : 1-10 - rhoResolution : 1
    */

       for( size_t i = 0; i < lines.size(); i++ )
       {
           float rho = lines[i][0], theta = lines[i][1];

           Point pt1, pt2;
           double a = cos(theta), b = sin(theta);
           double x0 = a*rho, y0 = b*rho;
           pt1.x = cvRound(x0 + 1000*(-b));
           pt1.y = cvRound(y0 + 1000*(a));
           pt2.x = cvRound(x0 - 1000*(-b));
           pt2.y = cvRound(y0 - 1000*(a));
           line( houghImage, pt1, pt2, Scalar(0,0,255), 1, CV_AA);

           /*   Testing the true equation y =  -cos0/sin0.x + r/sin0    pt1(0,r/sin0)   pt2(img.width, y(img.width));
           Point test1, test2;
           test1.x = 0;
           test1.y = cvRound(rho/sin(theta));

           test2.x = srcImage.cols;
           test2.y = cvRound( -(cos(theta)/sin(theta))*test2.x + rho/sin(theta));
           line( houghImage, test1, test2, Scalar(0,255,0), 1, 4);
           */

       }

    //  Displaying Image
    ui->imageHough->setPixmap( QPixmap::fromImage(cvMatToQImage(houghImage)) );
}

void MainWindow::cannyCallback()
{

    //  Setting value
    cannyLowThreshold = ui->cannyLowThresholdSlider->value();
    cannyMaxThreshold = ui->cannyMaxThresholdSlider->value();
    cannyApertureSize = ui->cannyApertureSizeSlider->value();

    //  Aperture Size should not be even
    if(cannyApertureSize%2 == 0){
        cannyApertureSize++;
    }

    //  Processing Canny Edge Detection
    Canny( blurImage, cannyImage, cannyLowThreshold, cannyMaxThreshold, cannyApertureSize, 1);//lowThreshold*ratio, kernelSize );

    //  Displaying Image
    ui->imageCanny->setPixmap( QPixmap::fromImage(cvMatToQImage(cannyImage)) );

    //  Updating Hough
    houghCallback();
}

void MainWindow::blurCallback()
{
    //  Setting value
    gaussKernelSize = ui->gaussKernelSizeSlider->value();
    if(gaussKernelSize%2 == 0){
        gaussKernelSize++;
    }

    gaussSigma = ui->gaussSigmaSlider->value();

    //  Processing Gaussian Filter
    GaussianBlur(srcImage, blurImage, Size(gaussKernelSize, gaussKernelSize), gaussSigma);

    //  Displaying Image
    ui->imageBlur->setPixmap( QPixmap::fromImage(cvMatToQImage(blurImage)) );

    //  Updating Canny
    cannyCallback();
    //  Updating Hough
    houghCallback();
}

QImage  MainWindow::cvMatToQImage( const Mat &inMat )
{
    //https://asmaloney.com/2013/11/code/converting-between-cvmat-and-qimage-or-qpixmap/

    switch ( inMat.type() )
    {
        // 8-bit, 4 channel
        case CV_8UC4:
        {
            QImage image( inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step), QImage::Format_ARGB32 );
            return image;
        }

        // 8-bit, 3 channel
        case CV_8UC3:
        {
            QImage image( inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step), QImage::Format_RGB888 );
            return image.rgbSwapped();
        }

        // 8-bit, 1 channel
        case CV_8UC1:
        {
            QImage image( inMat.data,inMat.cols, inMat.rows, static_cast<int>(inMat.step), QImage::Format_Grayscale8 );
            return image;
        }

        default:
            cerr << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
            break;
    }

    return QImage();
}

void MainWindow::motorButtonClicked(bool value)
{
    if(value)
        ui->buttonMotorDir->setIcon( QIcon(":/arrow_clockwise.png") );
    else
        ui->buttonMotorDir->setIcon( QIcon(":/arrow_anticlockwise.png") );
}

void MainWindow::scalingContent()
{
    ui->imageBlur->setScaledContents(ui->checkBoxScaledContent->isChecked());
    ui->imageCanny->setScaledContents(ui->checkBoxScaledContent->isChecked());
    ui->imageSource->setScaledContents(ui->checkBoxScaledContent->isChecked());
    ui->imageHough->setScaledContents(ui->checkBoxScaledContent->isChecked());
}

void MainWindow::chooseImageFilePath()
{
    QString imageFilePath = QFileDialog::getOpenFileName(this, "Open Image File", QDir::currentPath() + "/../", "Images (*.png *.jpg *jpeg *pgm *.bmp *tiff)");

    ui->lineEditFilePath->setText(imageFilePath);

    initImage();
    initParameters();
    sourceCallback();
    blurCallback();
    cannyCallback();
    houghCallback();
}

void MainWindow::openVersionMessage(string message)
{
    QMessageBox::information(this, "Version", "VC0706");
}

