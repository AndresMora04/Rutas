#pragma once
#include <QMainWindow>
#include "ui_ReportView.h"
#include <vector>
#include "Station.h"
#include "Route.h"
#include "Archivos.h"
#include <iostream>
#include <QMessageBox>
#include <QDateTime>
using namespace std;
using namespace Ui;

class ReportView : public QMainWindow
{
    Q_OBJECT

public:
    ReportView(const string& username, const string& character, QWidget* parent = nullptr);
    ~ReportView();

private slots:
    void onActionBtnGenerate();
    void onActionBtnExport();

private:
    ReportViewClass ui;
    vector<Station*> stations;
    vector<Route*> routes;
    QString generatedReport;

    string username;
    string character;

    void loadData();
    void generateStationReport();
    void generateRouteReport();
	void generateClosuresReport();
};