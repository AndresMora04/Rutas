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
using namespace std;

class MapView : public QMainWindow
{
	Q_OBJECT

public:
	MapView(QWidget* parent = nullptr);
	~MapView();

private slots:
	void onActionBtnAddStation();
	void onActionBtnAddRoute();
	void onActionBtnDelete();
	void onActionBtnRefresh();

private:
	Ui::MapViewClass ui;
	QGraphicsScene* scene;
	QGraphicsPixmapItem* mapItem;
	QVector<QGraphicsEllipseItem*> stations;
	bool addingStation = false;
	bool addingRoute = false;
	QGraphicsPixmapItem* firstStation = nullptr;
	QVector<QGraphicsLineItem*> routes;

	void loadMapImage(const QString& path);
	void updateStationCount();
	bool eventFilter(QObject* obj, QEvent* event) override;
};

