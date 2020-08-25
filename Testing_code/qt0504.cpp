#include "qt0504.h"
#include <iostream>
#include <string.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/core.hpp>

#include <qfiledialog.h>

//#include <Windows.h>

#include <QSpinBox>
#include <QPushButton>
#include <qmessagebox.h>
#include <qlistwidget.h>



using namespace std;
using namespace cv;

int i = 0;
Mat blackimg = Mat::zeros(1000, 800, CV_8UC3);

qt0504::qt0504(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	/*ui.spin1->setRange();
	ui.spin2->setRange();
	ui.*/
}

//void qt0504::pushbutton() {
//	QMessageBox::information(this, "Hello", "Good");
//
//	ui.listWidget->addItem(QString::number(i));
//	i++;
//	// Display
//}

// Convert OpenCV::Mat -> Qt:QImage
QImage qt0504::convertOpenCVMatToQtQImage(cv::Mat mat) {
	if (mat.channels() == 1) {                  // if grayscale image
		return QImage((uchar*)mat.data, (int)mat.cols, (int)mat.rows, (int)mat.step, QImage::Format_Indexed8);     // declare and return a QImage
	}
	else if (mat.channels() == 3) {             // if 3 channel color image
		cv::cvtColor(mat, mat, COLOR_BGR2RGB);     // invert BGR to RGB
		return QImage((uchar*)mat.data, (int)mat.cols, (int)mat.rows, (int)mat.step, QImage::Format_RGB888);       // declare and return a QImage
	}
	return QImage();  // return a blank QImage if the above did not work
}


int s = 0;

int selectType = 0;
CascadeClassifier face_cascade;

void qt0504::Display() {
	double ticks = getTickFrequency();
	if (selectType == 1) {
		s = 2;

		int64 t0;
		// Image read
		VideoCapture cap(0);
		cap.open(0);
		
		cap >> frame;


		//Mat img;
		int i, li = 1;

		for (;;) {
			cap >> frame;
			cap.retrieve(frame, CAP_OPENNI_BGR_IMAGE);

			vector<Rect> faces;
			t0 = getTickCount();
			// Detect faces
			face_cascade.detectMultiScale(frame, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(ui.spinBox_2->value(), ui.spinBox->value()));

			//face_cascade.detectMultiScale(frame, faces, 1.1, 4);
			t0 = getTickCount() - t0; // ms

			for (unsigned i = 0; i < faces.size(); i++) {
				rectangle(frame, faces[i], Scalar(255, 0, 0), 2, 1);
				faces[i].width;
			}
			
			char t2[50];
			sprintf(&t2[0], "Processing Time : %.6f s", ((double)t0 / ticks ));
			li++;
			string str(t2);
			//QString t = QString::number((double)t0 / ticks);

			QString qstring = QString::fromStdString(t2);
			//t.prepend("Processing Time : ");
			//t.append("sec");

			ui.listWidget->addItem(QString::QString(qstring));
			ui.listWidget->scrollToBottom();
			if (li == 100) {
				ui.listWidget->clear();
				li = 0;
			}
			// To draw rectangles around detected faces
			

			// Convert Image


			waitKey(0);
			if (s == 1) {
				QString save_lot = QFileDialog::getSaveFileName(this, tr("Save File"), "C://", tr("Image (*.jpg *.png *.jpeg)"));
				if (save_lot != 0) {
					string stdstring = save_lot.toStdString();
					imwrite(stdstring, frame);
					s = 2;
				}

				else {
					s = 2;
				}
			}
			else if (s == 3) {
				s = 0;

				QImage qimgOutput = convertOpenCVMatToQtQImage(blackimg);

				// Display output
				ui.label->setPixmap(QPixmap::fromImage(qimgOutput));
				break;
			}
			cv::resize(frame, frame, Size(751, 541));

			QImage qimgOutput = convertOpenCVMatToQtQImage(frame);

			// Display output
			ui.label->setPixmap(QPixmap::fromImage(qimgOutput));
			//ui->listWidget_client1->scrollToBottom();
		}
	}
	else {
		selectType = 0;
		QString errMsg = "Cannot find XML";
		ui.listWidget_2->addItem(QString::QString(errMsg));
		ui.listWidget_2->scrollToBottom();
	}
}

void qt0504::capture() {
	// Image read
	if (selectType == 1 && s == 2) {
		s = 1;
	}
	else {
		QString errMsg = "Webcam is not running.";
		ui.listWidget_2->addItem(QString::QString(errMsg));
		ui.listWidget_2->scrollToBottom();
	}
}

void qt0504::closeCam() {
	// Image read
	if (selectType == 1 && s == 2) {
		s = 3;
	}
	else {
		QString errMsg = "Webcam is not running.";
		ui.listWidget_2->addItem(QString::QString(errMsg));
		ui.listWidget_2->scrollToBottom();
	}
}

void qt0504::selectXml() {
	QString save_lot = QFileDialog::getOpenFileName(this, tr("Open XML"), "C://training//classifier//", tr("Image (*.xml)"));
	if (save_lot != 0) {
		string stdstring = save_lot.toStdString();
		face_cascade.load(stdstring);
		selectType = 1;
		ui.label_2->setText(save_lot);
		ui.label_5->setText(save_lot);
		ui.label_19->setText(save_lot);

	}
}

int imgstate = 0, ds = 0;

void qt0504::selectImg() {
	if (selectType == 1) {
		QString loadImg = QFileDialog::getOpenFileName(this, tr("Open Image"), "C://", tr("Image (*.jpg;*.jpeg;*.png)"));
		if (loadImg != 0) {
			Mat rImg;
			imgstate = 1;
			string stdstring = loadImg.toStdString();
			myImg = imread(stdstring);
			myImg2 = imread(stdstring);
	
			char t1[50];
			sprintf(&t1[0], "%d * %d Pixel", myImg.cols, myImg.rows);
			string str1(t1);
			QString qstring1 = QString::fromStdString(t1);
			ui.label_11->setText(qstring1);

			cv::resize(myImg, rImg, Size(751, 541));
			ds = 0;
			QImage qimgOutput = convertOpenCVMatToQtQImage(rImg);

			// Display output
			ui.label_4->setPixmap(QPixmap::fromImage(qimgOutput));
			ui.label_3->setText(loadImg);
		}
	}
	else {
		selectType = 0;
		QString errMsg = "Cannot find XML";
		ui.listWidget_3->addItem(QString::QString(errMsg));
		ui.listWidget_3->scrollToBottom();
	}
}

void qt0504::detectImg() {
	if (imgstate == 1) {
		double ticks = getTickFrequency();
		int64 t0;
		Mat myImg3 = myImg2.clone();


		vector<Rect> faces;
		Mat gray;
		//cvtColor(myImg2, gray, COLOR_BGR2GRAY);
		t0 = getTickCount();
		face_cascade.detectMultiScale(myImg3, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(ui.spinBox_3->value(), ui.spinBox_4->value()));
		t0 = getTickCount() - t0;
		//face_cascade.detectMultiScale(gray, faces, 1.1, 4);
		// To draw rectangles around detected faces
		for (unsigned i = 0; i < faces.size(); i++) {
			rectangle(myImg3, faces[i], Scalar(255, 0, 0), 2, 1);
		}
		

		char t2[50];
		sprintf(&t2[0], "Processing Time : %.6f s", (double)t0 / ticks);
		string str2(t2);
		//QString t = QString::number((double)t0 / ticks);

		QString qstring2 = QString::fromStdString(t2);
		//t.prepend("Processing Time : ");
		//t.append("sec");

		QString save_lot = QFileDialog::getSaveFileName(this, tr("Save File"), "C://sample//human//", tr("Image (*.jpg *.png *.jpeg)"));
		if (save_lot != 0) {
			string stdstring = save_lot.toStdString();
			imwrite(stdstring, myImg3);
			s = 2;
		}
		cv::resize(myImg3, myImg3, Size(751, 541));
		ui.listWidget_3->addItem(QString::QString(qstring2));
		ui.listWidget_3->scrollToBottom();


		QImage qimgOutput = convertOpenCVMatToQtQImage(myImg3);
		ui.label_4->setPixmap(QPixmap::fromImage(qimgOutput));
	}
	else {
		QString errMsg = "Image is not Exist.";
		ui.listWidget_3->addItem(QString::QString(errMsg));
		ui.listWidget_3->scrollToBottom();
	}
}

string lvideo;
int st = 0;
int vstate = 0;

void qt0504::startVideo() {
	double ticks = getTickFrequency();
	if (selectType == 1) {
		if (vstate == 1) {
			VideoCapture cap2(lvideo);

			cap1 = cap2;

			int64 t0;

			//동영상 플레이시 크기를  320x240으로 지정  
			//cap1.set(CAP_PROP_FRAME_WIDTH, 320);
			//cap1.set(CAP_PROP_FRAME_HEIGHT, 240);

			Mat frame1;

			int li = 0;

			for (;;)
			{
				//웹캡으로부터 한 프레임을 읽어옴  
				cap1 >> frame1;

				//30ms 정도 대기하도록 해야 동영상이 너무 빨리 재생되지 않음.  
				if (waitKey(20) == 27) break; //ESC키 누르면 종료  

				vector<Rect> faces;
				t0 = getTickCount();
				// Detect faces
				face_cascade.detectMultiScale(frame1, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(ui.spinBox_9->value(), ui.spinBox_10->value()));

				//face_cascade.detectMultiScale(frame1, faces, 1.1, 4);
				t0 = getTickCount() - t0;

				char t2[50];
				sprintf(&t2[0], "Processing Time : %.5f s", (double)t0 / ticks );
				li++;
				string str(t2);
				QString t = QString::number((double)t0 / ticks);

				QString qstring = QString::fromStdString(t2);

				ui.listWidget_8->addItem(QString::QString(qstring));
				ui.listWidget_8->scrollToBottom();
				if (li == 100) {
					ui.listWidget_8->clear();
					li = 0;
				}

				// To draw rectangles around detected faces
				for (unsigned i = 0; i < faces.size(); i++) {
					rectangle(frame1, faces[i], Scalar(255, 0, 0), 2, 1);
				}
				// Convert Image
				//cv::resize(frame1, frame1, Size(751, 541));

				if (frame1.empty() != 0)break;

				QImage qimgOutput = convertOpenCVMatToQtQImage(frame1);

				// Display output
				ui.label_21->setPixmap(QPixmap::fromImage(qimgOutput));
				//ui->listWidget_client1->scrollToBottom();
				if (st == 1) {
					st = 0;
					break;
				}

			}
		}
		else {
			QString errMsg = "The Video file does not exist.";
			ui.listWidget_7->addItem(QString::QString(errMsg));
			ui.listWidget_7->scrollToBottom();
		}
	}
	else {
		QString errMsg = "The xml file does not exist.";
		ui.listWidget_7->addItem(QString::QString(errMsg));
		ui.listWidget_7->scrollToBottom();
	}
}

void qt0504::selectVideo() {
	QString loadVideo = QFileDialog::getOpenFileName(this, tr("Open Image"), "C://", tr("Video (*.avi;*.mp4;*.mov)"));
	lvideo = loadVideo.toStdString();
	ui.label_6->setText(loadVideo);
	vstate = 1;
}

void qt0504::videostop() {
	st = 1;
}