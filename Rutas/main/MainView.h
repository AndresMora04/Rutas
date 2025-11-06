#pragma once

#include <QMainWindow>
#include "ui_MainView.h"
#include "MainView.h"
#include <iostream>
#include <QMessageBox>
#include <iostream>
#include "LogInView.h"
#include "MapView.h"
#include "TreeView.h"
#include "SimulationView.h"
#include "ReportView.h"
using namespace std;

class MainView : public QMainWindow
{
	Q_OBJECT

public:
	MainView(QWidget* parent = nullptr);
	~MainView();
private slots:
	void onActionBtnMap();
	void onActionBtnTree();
	void onActionBtnSimulation();
	void onActionBtnReports();
	void onActionBtnLogOut();
private:
	Ui::MainViewClass ui;
	MapView* mapView;
	TreeView* treeView;
	SimulationView* simulationView;
	ReportView* reportView;
};

