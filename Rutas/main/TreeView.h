#pragma once

#include <QMainWindow>
#include "ui_TreeView.h"
#include "Tree.h"
#include <iostream>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include "Archivos.h"
#include <QString>
#include <QGraphicsTextItem>
#include "Utils.h"
using namespace std;
using namespace Qt;
using namespace Ui;

class TreeView : public QMainWindow
{
	Q_OBJECT

public:
	TreeView(QWidget* parent = nullptr);
	~TreeView();
private slots:
	void onActionSearch();
	void onActionDelete();
	void onActionTraverse();
	void onActionExport();
private:
	TreeViewClass ui;
	Tree* stationTree;
	QGraphicsScene* scene;
	void loadStationsFromFile();
	void drawTree();
	void displayOutput(const QString& text);
protected:
	void showEvent(QShowEvent* event) override;
};

