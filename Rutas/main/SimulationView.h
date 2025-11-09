#pragma once

#include <QMainWindow>
#include "ui_SimulationView.h"
#include <QGraphicsScene>
#include <vector>
#include "Station.h"
#include "Route.h"
#include "Graph.h"
#include <QTimer>
#include "Event.h"
#include <iostream>
#include "Archivos.h"
#include <QGraphicsEllipseItem>
#include <QDir>
#include <QRandomGenerator>
#include "User.h"
using namespace std;

class SimulationView : public QMainWindow
{
	Q_OBJECT

public:
	SimulationView(User* user = nullptr, QWidget* parent = nullptr);
	~SimulationView();
private slots:
	void onActionRunAlgorithm();
	void onActionReset();
private:
	Ui::SimulationViewClass ui;
	void loadGraphFromFiles();
	void drawGraph();
	void clearSimulation();
	void startAnimation(vector<Station*> path);
	void moveVehicleStep();
	void triggerRandomEvent();
	void resolveEvents();
	QGraphicsScene* scene;
	vector<Station*> stations;
	vector<Route*> routes;
	Graph graph;
	QGraphicsPixmapItem* vehicle = nullptr;
	QTimer* animationTimer = nullptr;
	vector<Station*> currentPath;
	int currentIndex = 0;
	vector<Event*> events;
	User* currentUser = nullptr;
};

