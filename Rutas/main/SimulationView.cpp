#include "SimulationView.h"

SimulationView::SimulationView(User* user, QWidget* parent)
	: QMainWindow(parent),
	scene(new QGraphicsScene(this)),
	currentUser(user)
{
	ui.setupUi(this);

	ui.graphicsSimulation->setScene(scene);
	ui.graphicsSimulation->setRenderHint(QPainter::Antialiasing);
	ui.graphicsSimulation->setBackgroundBrush(QBrush(QColor("#1E1E1E")));

	ui.cBoxAlgorithm->addItem("Breadth-First Search (BFS)");
	ui.cBoxAlgorithm->addItem("Depth-First Search (DFS)");
	ui.cBoxAlgorithm->addItem("Dijkstra");
	ui.cBoxAlgorithm->addItem("Floyd-Warshall");
	ui.cBoxAlgorithm->addItem("Kruskal");
	ui.cBoxAlgorithm->addItem("Prim");

	loadGraphFromFiles();

	drawGraph();

	connect(ui.btnRunAlgorithm, &QPushButton::clicked, this, &SimulationView::onActionRunAlgorithm);
	connect(ui.btnReset, &QPushButton::clicked, this, &SimulationView::onActionReset);
}

SimulationView::~SimulationView()
{
}

void SimulationView::loadGraphFromFiles()
{
	stations = Archivos::cargarEstaciones();
	routes = Archivos::cargarRutas(stations);

	for (auto* s : stations) graph.addStation(s);
	for (auto* r : routes) graph.addRoute(r);

	ui.cBoxStartStation->clear();
	ui.cBoxEndStation->clear();

	for (auto* s : stations) {
		QString name = QString::fromUtf8(s->getName().c_str());
		ui.cBoxStartStation->addItem(name);
		ui.cBoxEndStation->addItem(name);
	}
}

void SimulationView::drawGraph()
{
	scene->clear();

	QString path = QDir::currentPath() + "/../Resources/Mapa.png";
	QPixmap mapPixmap(path);
	if (!mapPixmap.isNull()) {
		QGraphicsPixmapItem* mapItem = scene->addPixmap(mapPixmap);
		mapItem->setZValue(-1);
	}

	for (auto* s : stations) {
		QPointF pos(s->getX(), s->getY());
		QString iconPath = QDir::currentPath() + "/../Resources/Parada.png";
		QPixmap icon(iconPath);
		icon = icon.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		QGraphicsPixmapItem* marker = scene->addPixmap(icon);
		marker->setZValue(1);
		marker->setPos(pos - QPointF(icon.width() / 2, icon.height() / 2));
		marker->setToolTip(QString::fromUtf8(s->getName().c_str()));
	}

	for (auto* r : routes) {
		QPointF p1(r->getStart()->getX(), r->getStart()->getY());
		QPointF p2(r->getEnd()->getX(), r->getEnd()->getY());
		QGraphicsLineItem* line = scene->addLine(QLineF(p1, p2),
			QPen(QColor("#00FF88"), 2));
		line->setZValue(0);
	}
}

void SimulationView::onActionReset()
{
	clearSimulation();
	drawGraph();
	ui.txtOutput->clear();
	ui.txtOutput->append("Simulacion reiniciada correctamente.");
}

void SimulationView::clearSimulation()
{
	if (animationTimer && animationTimer->isActive()) {
		animationTimer->stop();
	}

	if (vehicle) {
		scene->removeItem(vehicle);
		delete vehicle;
		vehicle = nullptr;
	}

	for (auto* e : events)
		delete e;
	events.clear();

	scene->clear();

	currentPath.clear();
	currentIndex = 0;

	QString mapPath = QDir::currentPath() + "/../Resources/Mapa.png";
	QPixmap mapPixmap(mapPath);
	if (!mapPixmap.isNull()) {
		QGraphicsPixmapItem* mapItem = scene->addPixmap(mapPixmap);
		mapItem->setZValue(-1);
	}
}

void SimulationView::onActionRunAlgorithm()
{
	QString algorithm = ui.cBoxAlgorithm->currentText();
	QString startStationName = ui.cBoxStartStation->currentText();
	QString endStationName = ui.cBoxEndStation->currentText();

	Station* startStation = nullptr;
	Station* endStation = nullptr;

	for (auto* s : stations) {
		QString stationName = QString::fromUtf8(s->getName().c_str());
		if (stationName == startStationName)
			startStation = s;
		if (stationName == endStationName)
			endStation = s;
	}

	if (!startStation || !endStation) {
		ui.txtOutput->setText("Error: estación de inicio o fin no válida.");
		return;
	}

	ui.txtOutput->clear();
	ui.txtOutput->append("?? Ejecutando " + algorithm + " desde " +
		startStationName + " hasta " + endStationName + "...\n");

	vector<Station*> result;

	if (algorithm.contains("Breadth-First")) {
		result = graph.bfs(startStation);
	}
	else if (algorithm.contains("Depth-First")) {
		result = graph.dfs(startStation);
	}
	else if (algorithm.contains("Dijkstra")) {
		result = graph.dijkstra(startStation, endStation);
	}
	else {
		ui.txtOutput->append("?? Algoritmo aún no implementado.");
		return;
	}

	QPen highlightPen(Qt::yellow, 3, Qt::DashLine);
	for (size_t i = 0; i < result.size() - 1; ++i) {
		QPointF p1(result[i]->getX(), result[i]->getY());
		QPointF p2(result[i + 1]->getX(), result[i + 1]->getY());
		scene->addLine(QLineF(p1, p2), highlightPen);
	}

	QString pathText = "??? Recorrido: ";
	for (auto* s : result)
		pathText += QString::fromUtf8(s->getName().c_str()) + " ? ";
	ui.txtOutput->append(pathText + "\n? Algoritmo completado.");

	currentPath = result;

	startAnimation(result);

	triggerRandomEvent();
}

void SimulationView::startAnimation(vector<Station*> path)
{
	if (path.empty()) return;

	if (!vehicle) {
		QString busPath = QDir::currentPath() + "/../Buses/" +
			QString::fromUtf8(currentUser->getSelectedCharacter().c_str()) + ".png";

		QPixmap busIcon(busPath);
		busIcon = busIcon.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		vehicle = scene->addPixmap(busIcon);
		vehicle->setZValue(5);
		vehicle->setPos(QPointF(path.front()->getX(), path.front()->getY()));
	}

	currentIndex = 0;

	if (animationTimer) delete animationTimer;
	animationTimer = new QTimer(this);
	connect(animationTimer, &QTimer::timeout, this, &SimulationView::moveVehicleStep);
	animationTimer->start(400);
}

void SimulationView::moveVehicleStep()
{
	if (currentPath.empty() || currentIndex >= currentPath.size()) {
		animationTimer->stop();
		ui.txtOutput->append("?? Simulación finalizada.");
		return;
	}

	Station* currentStation = currentPath[currentIndex];
	vehicle->setPos(currentStation->getX(), currentStation->getY());
	ui.txtOutput->append("?? Visitando: " + QString::fromUtf8(currentStation->getName().c_str()));

	currentIndex++;
}

void SimulationView::triggerRandomEvent()
{
	if (stations.empty()) return;

	int randomIndex = QRandomGenerator::global()->bounded(static_cast<int>(stations.size()));
	Station* target = stations[randomIndex];

	QStringList eventTypes = { "Accident", "Traffic", "Maintenance" };
	QString eventType = eventTypes.at(QRandomGenerator::global()->bounded(eventTypes.size()));

	QString desc = "?? Event: " + eventType + " at " + QString::fromUtf8(target->getName().c_str());
	ui.txtOutput->append(desc);

	std::string typeStr = eventType.toUtf8().constData();
	std::string descStr = desc.toUtf8().constData();

	Event* e = new Event(typeStr, descStr, target);
	events.push_back(e);

	QGraphicsEllipseItem* alert = scene->addEllipse(target->getX(), target->getY(), 25, 25,
		QPen(Qt::red, 3, Qt::DashLine), QBrush(Qt::transparent));
	alert->setZValue(4);
}

void SimulationView::resolveEvents()
{
	for (auto* e : events)
		e->resolve();
	events.clear();
	ui.txtOutput->append("Todos los eventos fueron resueltos.");
}