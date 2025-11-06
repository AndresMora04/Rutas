#pragma once

#include <QMainWindow>
#include "ui_MapView.h"

class MapView : public QMainWindow
{
	Q_OBJECT

public:
	MapView(QWidget *parent = nullptr);
	~MapView();

private:
	Ui::MapViewClass ui;
};

