#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QFileDialog>
#include <iostream>
#include <QMessageBox>

#include "serialhandling.h"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
        void initUIConnection();
        void initUI();
        void initSerialConnection();
        QImage cvMatToQImage( const Mat &inMat );
        void initParameters();

    private:
        Ui::MainWindow *ui;

        SerialHandling *serial;

        QString srcImagePath;

        Mat srcImage;
        Mat cannyImage;
        Mat blurImage;
        Mat houghImage;

        vector<Vec2f> lines;

        int gaussKernelSize, gaussSigma;
        int cannyLowThreshold, cannyMaxThreshold;
        int cannyApertureSize;
        double houghRhoResolution, houghThetaResolution, houghThetaMin, houghThetaMax, houghThreshold, houghMaxLineGap, houghMinLineLength;

    public slots:

        //  Image Processing
        void houghCallback();
        void cannyCallback();
        void blurCallback();
        void sourceCallback();

        void motorButtonClicked(bool value);

        void chooseImageFilePath();

        void scalingContent();
        void setStreamEntry();

        void initImage();
        void initImage(QString path);
        void displaySourceImage();

        void writeFilePathLabel(QString path);

        void openVersionMessage(string message);

};

#endif // MAINWINDOW_H
