#include "MainView.h"

MainView::MainView(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    inicializarVistas();
    configurarBotones();

    ui.stackedViews->setCurrentWidget(mapView);
}

MainView::MainView(QString username, QString character, QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    inicializarVistas();
    configurarBotones();
    ui.stackedViews->setCurrentWidget(mapView);

    ui.lbelUser->setText("Usuario: " + username);

    QString imagePath = QDir::currentPath() + "/../Transformations/" + character + ".png";
    QPixmap pix(imagePath);

    if (!pix.isNull()) {
        ui.lbelCharacter->setPixmap(pix.scaled(80, 80, KeepAspectRatio, SmoothTransformation));
    } else {
        ui.lbelCharacter->setText("(Sin imagen)");
    }
}

void MainView::inicializarVistas()
{
    while (ui.stackedViews->count() > 0) {
        QWidget* page = ui.stackedViews->widget(0);
        ui.stackedViews->removeWidget(page);
        delete page;
    }

    mapView = new MapView();
    treeView = new TreeView();
    simulationView = new SimulationView();
    reportView = new ReportView();

    ui.stackedViews->addWidget(mapView);
    ui.stackedViews->addWidget(treeView);
    ui.stackedViews->addWidget(simulationView);
    ui.stackedViews->addWidget(reportView);
}

void MainView::configurarBotones()
{
    connect(ui.btnMap, &QPushButton::clicked, this, &MainView::onActionBtnMap);
    connect(ui.btnTree, &QPushButton::clicked, this, &MainView::onActionBtnTree);
    connect(ui.btnSimulation, &QPushButton::clicked, this, &MainView::onActionBtnSimulation);
    connect(ui.btnReports, &QPushButton::clicked, this, &MainView::onActionBtnReports);
    connect(ui.btnLogOut, &QPushButton::clicked, this, &MainView::onActionBtnLogOut);
}

MainView::~MainView()
{
}

void MainView::onActionBtnMap()
{
	ui.stackedViews->setCurrentWidget(mapView);
}

void MainView::onActionBtnTree()
{
	ui.stackedViews->setCurrentWidget(treeView);
}

void MainView::onActionBtnSimulation()
{
	ui.stackedViews->setCurrentWidget(simulationView);
}

void MainView::onActionBtnReports()
{
	ui.stackedViews->setCurrentWidget(reportView);
}

void MainView::onActionBtnLogOut()
{
	LogInView* login = new LogInView();
	login->show();
	this->close();
}