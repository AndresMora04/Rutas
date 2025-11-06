#include <iostream>
#include <QApplication>
#include "LogInView.h"
using namespace std;

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	LogInView window;
	window.show();

	return app.exec();
}
