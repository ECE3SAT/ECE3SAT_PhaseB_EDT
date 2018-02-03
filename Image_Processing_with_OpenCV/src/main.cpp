#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

/// Global variables
Mat src, src_gray;
Mat dst, cannyImage;
Mat blurImage;
Mat houghImage;

char key;

int cannyLowThreshold, cannyMaxThreshold, ratio, cannyApertureSize ;
int gaussKernelSize ;
int rhoResolution, thetaResolution, thetaMin, thetaMax, houghThreshold, maxLineGap, minLineLength;

// Window Name
char srcWindowName[50];// = "Source";
char* gaussWindowName = "Gaussian Filter";
char* cannyWindowName = "Canny Filter";
char* houghWindowName = "Hough Transform";



void initGlobalVariables(){

	cannyMaxThreshold = 200;
    ratio = 3;
	cannyApertureSize = 3; // 3 or 5 or 7
	gaussKernelSize = 3;

	rhoResolution = 1 *10;		//Resolution of 1 pixel
	thetaResolution = 10 *10;	//Resolution of 10° degrees
	thetaMin = 0 *10;			//Minimum Angle
	thetaMax = 180 *10;			//Maximum Angle
	houghThreshold = 150;		//Minimum number of intersection
}

void houghCallback(int, void*)
{
    vector<Vec2f> lines;
    houghImage = src.clone();

    //	Convert Int to Double and Rescale by dividing by 1000
    double dbRhoResolution = (double)rhoResolution / 10;
	double dbThetaResolution = (double)thetaResolution / 10;
	double dbThetaMax = (double)thetaMax / 10;
	double dbThetaMin = (double)thetaMin / 10;

	//	Convert Degrees to Radian
	dbThetaMax *= CV_PI / 180;
	dbThetaMin *= CV_PI / 180;
	dbThetaResolution *= CV_PI / 180;

	//	Should always Min < Max
	if(dbThetaMin >= dbThetaMax){
		double tmp;
		tmp = dbThetaMin;
		dbThetaMin = dbThetaMax;
		dbThetaMax = tmp;
	}

	printf("rho res: %lf theta res: %lf\n", dbThetaResolution, dbRhoResolution);

    HoughLines(cannyImage, lines, dbRhoResolution, dbThetaResolution, houghThreshold, 0, 0 , dbThetaMin, dbThetaMax );

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
	}


	/*
	vector<Vec4i> linesP; // will hold the results of the detection
	HoughLinesP(cannyImage, linesP, dbRhoResolution, dbThetaResolution, houghThreshold, minLineLength, maxLineGap);//, 50, 10 ); // runs the actual detection

	// Draw the lines
	for( size_t i = 0; i < linesP.size(); i++ )
	{
		Vec4i l = linesP[i];
		line( houghImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 1, LINE_4);
	}*/

    imshow( houghWindowName, houghImage );

    printf("Hough Transform Printed : %lld lines\n", lines.size());
}

void cannyCallback(int, void*)
{
	if(cannyApertureSize%2 == 0){
		cannyApertureSize++;
		setTrackbarPos("Aperture size:", cannyWindowName, cannyApertureSize);
	}
	/// Canny detector
	Canny( blurImage, cannyImage, cannyLowThreshold, cannyMaxThreshold, cannyApertureSize, 1);//lowThreshold*ratio, kernelSize );

	/// Using Canny's output as a mask, we display our result
	dst = Scalar::all(0);
	src.copyTo( dst, cannyImage);

	imshow( cannyWindowName, dst );

	houghCallback(0, 0);
 }


void gaussCallback(int, void*)
{
	// Should be odd
	if(gaussKernelSize%2 == 0){
		gaussKernelSize++;
		setTrackbarPos("Kernel Size :", gaussWindowName, gaussKernelSize);
	}
	GaussianBlur( src_gray, blurImage, Size(gaussKernelSize,gaussKernelSize), 0 );
	//imshow(gaussWindowName, blurImage);
	cannyCallback(0,0);
}

void createAllTrackbar(){
	/*******	Gauss TrackBar	*******
	  //createTrackbar( "Kernel Size :", gaussWindowName, &gaussKernelSize, 20, gaussCallback );
	  //setTrackbarMin( "Kernel Size :", gaussWindowName, 1);

	/*******	Canny Trackbar	*******/
	//		Low
	createTrackbar( "Low Threshold :", cannyWindowName, &cannyLowThreshold, 1000, cannyCallback );
	setTrackbarMin( "Low Threshold :", cannyWindowName, 1);
	//		Max
	createTrackbar( "Max Threshold :", cannyWindowName, &cannyMaxThreshold, 1000, cannyCallback);
	setTrackbarMin( "Max Threshold :", cannyWindowName, 1);
	//		Aperture Size
	createTrackbar( "Aperture size:", cannyWindowName, &cannyApertureSize, 7, cannyCallback);
	setTrackbarMin( "Aperture size:", cannyWindowName, 3);

	/*******	Hough Trackbar	*******/
	//		Rho Resolution
	createTrackbar( "Rho Resolution (x/10) :", houghWindowName, &rhoResolution, 5000, houghCallback);
	setTrackbarMin( "Rho Resolution (x/10) :", houghWindowName, 1);
	//		Theta Resolution
	createTrackbar( "Theta Resolution (x/10) :", houghWindowName, &thetaResolution, 180*10, houghCallback);
	setTrackbarMin( "Theta Resolution (x/10) :", houghWindowName, 1);
	//		Min Theta
	createTrackbar( "Max Theta (x/10)", houghWindowName, &thetaMax, 180*10, houghCallback);
	createTrackbar( "Min Theta (x/10)", houghWindowName, &thetaMin, 180*10, houghCallback);
	//		Threshold
	createTrackbar( "Threshold :", houghWindowName, &houghThreshold, max(src.cols, src.rows), houghCallback);
	//		Min Line Length
	createTrackbar(	"Min Line Length :", houghWindowName, &minLineLength, max(src.cols, src.rows), houghCallback);
	//		Max Line Gap
	createTrackbar( "Max Line Gap :", houghWindowName, &maxLineGap, (int)src.cols / 20, houghCallback);



}


/** @function main */
int main( int argc, char** argv )
{
	initGlobalVariables();
	printf("---CHOICE---\n");
	printf("Image File : 1 \n");
	printf("Serial Port : 2 \n\n?");
	int choice;
	cin >> choice;
	cin.ignore(std::numeric_limits<streamsize>::max(),'\n');
	if(choice == 1){

		//	Getting Image File
		string imageFilePath = "";
		printf("Image File Path : ");
		getline(cin, imageFilePath);		//GNU READLINE LIBRARY for autocompletion		imageFilePath = readLine( "Image File Path : ");
		printf("\n");

		//	Load an image
		src = imread( imageFilePath );
	}
	else if(choice == 2){

		//	Getting Serial Path
		string serialPath = "";
		printf("Serial Path : ");
		getline(cin, serialPath);		//GNU READLINE LIBRARY for autocompletion		imageFilePath = readLine( "Image File Path : ");
		printf("\n");

		int serialFile;
		serialFile = open(serialPath.c_str(),O_RDWR | O_NOCTTY);
		if(serialFile)
			printf("Error in Opening : %s\n", serialPath.c_str());
		else
			printf("%s Opened Successfully\n", serialPath.c_str());
		close(serialFile);
	}

	if( !src.data )
	{
		printf("Error : not a valid filename\n");
		return -1;
	}

	//	Create a matrix of the same type and size as src (for dst)
	dst.create( src.size(), src.type() );

	//	Convert the image to grayscale
	cvtColor( src, src_gray, CV_BGR2GRAY );

	//	Create a window
	sprintf(srcWindowName, "Source -%s %d x %d",argv[1], src.rows, src.cols);
	namedWindow( srcWindowName, WINDOW_NORMAL );
	  //namedWindow( gaussWindowName, CV_WINDOW_AUTOSIZE );
	namedWindow( cannyWindowName, CV_WINDOW_AUTOSIZE );
	namedWindow( houghWindowName, WINDOW_NORMAL ); //CV_WINDOW_AUTOSIZE);

	//	Display Image source
	imshow(srcWindowName, src);

	//	Create trackbar
	createAllTrackbar();

	/// Set Threshold
	gaussCallback(0, 0);
	cannyCallback(0, 0);
	houghCallback(0, 0);

	/// Wait until user exit program by pressing a key
	do{
		key = waitKey(0);
		if(key == 97){
			printf("a\n");
			resize(houghImage, houghImage, Size(), 0.5, 0.5, INTER_AREA);
			imshow( houghWindowName, houghImage );
		}
		else if( key == 122){
			printf("q\n");
			resize(houghImage, houghImage, Size(), 1.5, 1.5, INTER_AREA);
			imshow(houghWindowName, houghImage );
		}

	}while( key != 27);

	return 0;
}

