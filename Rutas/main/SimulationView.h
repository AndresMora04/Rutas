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
using namespace Ui;

class SimulationView : public QMainWindow
{
	Q_OBJECT

public:
	SimulationView(const string& username, const string& character, QWidget* parent = nullptr);
	~SimulationView();
private slots:
	void onActionRunAlgorithm();
	void onActionReset();
private:
	SimulationViewClass ui;
	void loadGraphFromFiles();
	void drawGraph();
	void clearSimulation();
	void startAnimation(vector<Station*> path);
	void moveVehicleStep();
	void triggerRandomEvent();
	void resolveEvents();
	void updateBusPosition();
	void showEvent(QShowEvent* event) override;
	int currentIndex = 0;
	double moveSpeed = 2.0;
	bool isMoving = false;
	string character;
	string currentUsername;
	vector<Station*> stations;
	vector<Route*> routes;
	vector<Station*> currentPath;
	vector<Event*> events;
	Graph graph;
	QGraphicsScene* scene;
	QGraphicsPixmapItem* vehicle = nullptr;
	QTimer* animationTimer = nullptr;
	QPointF currentPos;
	QPointF targetPos;
};

