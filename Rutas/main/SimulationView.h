#pragma once

#include <QMainWindow>
#include "ui_SimulationView.h"

class SimulationView : public QMainWindow
{
	Q_OBJECT

public:
	SimulationView(QWidget *parent = nullptr);
	~SimulationView();

private:
	Ui::SimulationViewClass ui;
};

