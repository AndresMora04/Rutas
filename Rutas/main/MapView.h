#pragma once

#include <QMainWindow>
#include "ui_MapView.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "ui_MapView.h"
#include <QFile>
#include <QMessageBox>
#include <QPainter>
#include <iostream>
#include <QDir>
#include <QString>
#include <QDebug> 
#include <QMouseEvent>
#include <vector>
#include "Route.h"
#include "Station.h"
#include "Utils.h"
#include "Archivos.h"
using namespace std;
using namespace Ui;

class MapView : public QMainWindow
{
	Q_OBJECT

public:
	MapView(const string& username, QWidget* parent = nullptr);
	~MapView();

private slots:
	void onActionBtnAddStation();
	void onActionBtnAddRoute();
	void onActionBtnDelete();
	void onActionBtnRefresh();
	void onActionBtnDeleteRoute();
	void onActionBtnCloseRoute();
	void onActionBtnOpenRoute();
private:
	MapViewClass ui;
	QGraphicsScene* scene;
	QGraphicsPixmapItem* mapItem;
	bool addingStation = false;
	bool addingRoute = false;
	bool deletingStation = false;
	QGraphicsPixmapItem* firstStation = nullptr;
	vector<QGraphicsEllipseItem*> stations;
	vector<QGraphicsLineItem*> routes;
	vector<Station*> logicalStations;
	vector<Route*> logicalRoutes;
	void loadMapImage(const QString& path);
	void updateStationCount();
	bool eventFilter(QObject* obj, QEvent* event) override;
	string currentUsername;
	void redrawLoadedStations();
	void redrawLoadedRoutes();
	bool deletingRoute = false;
	bool closingRoute = false;
	bool openingRoute = false;
};


