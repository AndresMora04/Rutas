#pragma once

#include <QMainWindow>
#include "ui_LogInView.h"
#include "LogInView.h"
#include <QDir>
#include <QFileInfoList>
#include <QMessageBox>
#include <fstream>
#include <sstream>
#include <string>
#include "mainView.h"
#include <iostream>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

class LogInView : public QMainWindow
{
	Q_OBJECT

public:
	LogInView(QWidget *parent = nullptr);
	~LogInView();
private slots:
	void onActionBtnLogIn();

private:
	Ui::LogInViewClass ui;
};

