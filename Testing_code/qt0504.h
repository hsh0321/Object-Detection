#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qt0504.h"
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <qfile.h>
#include <QFileSystemWatcher.h>


using namespace std;
using namespace cv;

class qt0504 : public QMainWindow
{
	Q_OBJECT

public:
	qt0504(QWidget *parent = Q_NULLPTR);
	QImage qt0504::convertOpenCVMatToQtQImage(cv::Mat mat);

private:
	Ui::qt0504Class ui;
	Ui::qt0504Class *ui2;
	private slots:
	//void pushbutton();
	void Display();
	void capture();
	void closeCam();
	void selectXml();
	void selectImg();
	void detectImg();
	void startVideo();
	void selectVideo();
	void videostop();
private:
	Mat frame;
	Mat myImg,myImg2;
	QImage qt_image;
	VideoCapture cap1;
	
};