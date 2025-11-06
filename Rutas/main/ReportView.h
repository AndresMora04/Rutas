#pragma once

#include <QMainWindow>
#include "ui_ReportView.h"

class ReportView : public QMainWindow
{
	Q_OBJECT

public:
	ReportView(QWidget *parent = nullptr);
	~ReportView();

private:
	Ui::ReportViewClass ui;
};

