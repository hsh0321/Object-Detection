#include "qt0504.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qt0504 w;
	
	w.show();
	return a.exec();
}

