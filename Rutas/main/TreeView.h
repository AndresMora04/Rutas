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
#include <vector>
#include <string>
#include <qrandom.h>
using namespace std;
using namespace Qt;
using namespace Ui;

class TreeView : public QMainWindow
{
	Q_OBJECT

public:
	TreeView(const std::string& username, QWidget* parent = nullptr);
	~TreeView();
private slots:
	void onActionSearch();
	void onActionDelete();
	void onActionTraverse();
	void onActionExport();
	void onActionInsert();
private:
	string currentUsername;
	TreeViewClass ui;
	Tree* stationTree;
	QGraphicsScene* scene;
	void loadStationsFromFile();
	void drawTree();
	void displayOutput(const QString& text);
protected:
	void showEvent(QShowEvent* event) override;
};

