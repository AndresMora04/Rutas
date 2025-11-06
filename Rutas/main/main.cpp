#include <iostream>
#include <QApplication>
#include "LogInView.h"
using namespace std;

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	QFile file("ben10style.qss");
	if (file.open(QFile::ReadOnly)) {
		app.setStyleSheet(file.readAll());
	}

	LogInView window;
	window.show();

	return app.exec();
}
