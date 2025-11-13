#include <iostream>
#include <QApplication>
#include "LogInView.h"
#include "MainView.h"
using namespace std;

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	QFile file("Styles.qss");
	if (file.open(QFile::ReadOnly)) {
		app.setStyleSheet(file.readAll());
	}

	LogInView window;
	window.show();

	return app.exec();
}
