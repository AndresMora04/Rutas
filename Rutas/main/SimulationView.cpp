#include "SimulationView.h"

SimulationView::SimulationView(const string& username, const string& character, QWidget* parent)
	: QMainWindow(parent),
	scene(new QGraphicsScene(this)),
	currentUsername(username),
	character(character)
{
	ui.setupUi(this);

	ui.graphicsSimulation->setScene(scene);
	ui.graphicsSimulation->setRenderHint(QPainter::Antialiasing);
	ui.graphicsSimulation->setBackgroundBrush(QBrush(QColor("#1E1E1E")));

	ui.cBoxAlgorithm->addItems({
		"Breadth-First Search (BFS)",
		"Depth-First Search (DFS)",
		"Dijkstra",
		"Floyd-Warshall",
		"Kruskal",
		"Prim"
		});

	stations = Archivos::cargarEstacionesUsuario(currentUsername);
	routes = Archivos::cargarRutasUsuario(currentUsername, stations);

	for (auto* s : stations) graph.addStation(s);
	for (auto* r : routes) graph.addRoute(r);

	drawGraph();

	connect(ui.btnRunAlgorithm, &QPushButton::clicked, this, &SimulationView::onActionRunAlgorithm);
	connect(ui.btnReset, &QPushButton::clicked, this, &SimulationView::onActionReset);

	Archivos archivos;
}

SimulationView::~SimulationView()
{
}

void SimulationView::onActionReset()
{
	clearSimulation();

	stations = Archivos::cargarEstacionesUsuario(currentUsername);
	routes = Archivos::cargarRutasUsuario(currentUsername, stations);

	Archivos archivos;
	vector<pair<int, int>> cierres = archivos.cargarCierresUsuario(currentUsername);

	for (auto& c : cierres) {
		for (auto* r : routes) {
			if ((r->getStart()->getId() == c.first && r->getEnd()->getId() == c.second) ||
				(r->getStart()->getId() == c.second && r->getEnd()->getId() == c.first)) {
				r->setClosed(true);
			}
		}
	}

	graph = Graph();
	for (auto* s : stations) graph.addStation(s);
	for (auto* r : routes) graph.addRoute(r);

	drawGraph();
	scene->update();

	ui.txtOutput->clear();
	ui.txtOutput->append("Simulacion reiniciada correctamente.\n");
	if (!cierres.empty()) {
		ui.txtOutput->append("Algunas rutas permanecen cerradas segun el mapa.\n");
	}
	else {
		ui.txtOutput->append("Todas las rutas estan abiertas.\n");
	}
}



void SimulationView::onActionRunAlgorithm()
{
	QString algorithm = ui.cBoxAlgorithm->currentText();
	QString startStationName = ui.cBoxStartStation->currentText();
	QString endStationName = ui.cBoxEndStation->currentText();

	Station* startStation = nullptr;
	Station* endStation = nullptr;

	for (auto* r : routes)
		r->setClosed(false);

	Archivos archivos;
	vector<pair<int, int>> cierres = archivos.cargarCierresUsuario(currentUsername);

	if (!cierres.empty()) {
		ui.txtOutput->append("Rutas cerradas cargadas desde el mapa:\n");
		for (auto& c : cierres) {
			for (auto* r : routes) {
				if ((r->getStart()->getId() == c.first && r->getEnd()->getId() == c.second) ||
					(r->getStart()->getId() == c.second && r->getEnd()->getId() == c.first)) {
					r->setClosed(true);
					ui.txtOutput->append(" - " +
						QString::fromUtf8(r->getStart()->getName().c_str()) + " ? " +
						QString::fromUtf8(r->getEnd()->getName().c_str()));
				}
			}
		}
	}
	else {
		ui.txtOutput->append("No hay rutas cerradas registradas.\n");
	}

	graph = Graph();
	for (auto* s : stations) graph.addStation(s);
	for (auto* r : routes) graph.addRoute(r);
	drawGraph();
	scene->update();

	for (auto* s : stations) {
		QString stationName = QString::fromUtf8(s->getName().c_str());
		if (stationName == startStationName)
			startStation = s;
		if (stationName == endStationName)
			endStation = s;
	}

	if (startStation == endStation) {
		ui.txtOutput->setText("La estacion de inicio y fin son la misma. No hay recorrido que realizar.");
		return;
	}

	if (!startStation || !endStation) {
		ui.txtOutput->setText("Error: estacion de inicio o fin no válida.");
		return;
	}

	ui.txtOutput->clear();
	ui.txtOutput->append("?? Ejecutando " + algorithm + " desde " +
		startStationName + " hasta " + endStationName + "...\n");

	vector<Station*> result;

	if (algorithm.contains("Breadth-First")) {
		result = graph.bfsAvoidingBlocked(startStation, endStation);
	}
	else if (algorithm.contains("Depth-First")) {
		result = graph.dfsAvoidingBlocked(startStation, endStation);
	}
	else if (algorithm.contains("Dijkstra")) {
		result = graph.dijkstraAvoidingBlocked(startStation, endStation);
	}
	else if (algorithm.contains("Floyd-Warshall")) {
		result = graph.floydWarshallAvoidingBlocked(startStation, endStation);
	}
	else if (algorithm.contains("Kruskal")) {
		vector<Route*> mst = graph.kruskalAvoidingBlocked();
		ui.txtOutput->append("Árbol mínimo generado con " + QString::number(mst.size()) + " rutas.");
		return;
	}
	else if (algorithm.contains("Prim")) {
		vector<Route*> mst = graph.primAvoidingBlocked();
		ui.txtOutput->append("Árbol mínimo generado con " + QString::number(mst.size()) + " rutas.");
		return;
	}

	if (result.size() < 2) {
		ui.txtOutput->append("No hay recorrido posible o no se pudo calcular una ruta valida.");
		return;
	}

	QPen highlightPen(Qt::yellow, 3, Qt::DashLine);
	for (size_t i = 0; i < result.size() - 1; ++i) {
		QPointF p1(result[i]->getX(), result[i]->getY());
		QPointF p2(result[i + 1]->getX(), result[i + 1]->getY());
		scene->addLine(QLineF(p1, p2), highlightPen);
	}
	scene->update();

	QString pathText = "??? Recorrido: ";
	for (auto* s : result)
		pathText += QString::fromUtf8(s->getName().c_str()) + " ? ";
	ui.txtOutput->append(pathText + "\n? Algoritmo completado.");
	ui.txtOutput->append("\n");
	ui.txtOutput->append("\n");
	ui.txtOutput->append("\nConsulta: Estaciones en la ruta seleccionada:\n");
	for (auto* s : result) {
		ui.txtOutput->append(" - " + QString::fromUtf8(s->getName().c_str()) +
			" (" + QString::fromUtf8(s->getType().c_str()) + ")");
	}

	currentPath = result;

	startAnimation(result);
}

void SimulationView::startAnimation(vector<Station*> path)
{
	if (path.empty()) return;

	currentPath = path;
	currentIndex = 0;

	if (!vehicle) {
		QString busFolder = QDir::currentPath() + "/../Buses/";
		QString busPath = busFolder + QString::fromUtf8(character.c_str()) + ".png";

		QPixmap busIcon(busPath);
		if (busIcon.isNull()) {
			busIcon.load(busFolder + "Default.png");
		}

		busIcon = busIcon.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		vehicle = scene->addPixmap(busIcon);
		vehicle->setZValue(20);
	}

	currentPos = QPointF(path.front()->getX(), path.front()->getY());
	vehicle->setPos(currentPos);

	if (path.size() > 1)
		targetPos = QPointF(path[1]->getX(), path[1]->getY());

	if (!animationTimer) {
		animationTimer = new QTimer(this);
		connect(animationTimer, &QTimer::timeout, this, &SimulationView::updateBusPosition);
	}

	isMoving = true;
	animationTimer->start(16);
}

void SimulationView::moveVehicleStep()
{
	if (currentPath.empty() || currentIndex >= currentPath.size()) {
		animationTimer->stop();
		ui.txtOutput->append("?? Simulacion finalizada.");
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

	string typeStr = eventType.toUtf8().constData();
	string descStr = desc.toUtf8().constData();

	Event* e = new Event(typeStr, descStr, target);
	events.push_back(e);

	if (eventType == "Accident") {
		for (auto* r : routes) {
			if (r->getStart() == target || r->getEnd() == target) {
				double newCost = r->getCost() * 1.25;
				r->setCost(newCost);
				ui.txtOutput->append("?? Ruta afectada por accidente: " +
					QString::fromUtf8(r->getStart()->getName().c_str()) + " ? " +
					QString::fromUtf8(r->getEnd()->getName().c_str()) +
					" | Nuevo tiempo estimado: " + QString::number(newCost, 'f', 2));
			}
		}
	}

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

void SimulationView::showEvent(QShowEvent* event)
{
	QMainWindow::showEvent(event);

	scene->clear();
	graph = Graph();

	loadGraphFromFiles();
	drawGraph();

	ui.txtOutput->append("Grafo actualizado con los ultimos cambios del mapa.");
}

void SimulationView::updateBusPosition()
{
	if (!isMoving || currentPath.empty() || currentIndex >= currentPath.size() - 1)
		return;

	QPointF direction = targetPos - currentPos;
	double distance = std::sqrt(direction.x() * direction.x() + direction.y() * direction.y());

	if (distance < moveSpeed)
	{
		currentPos = targetPos;
		vehicle->setPos(currentPos);

		if (currentIndex + 1 >= currentPath.size()) {
			return;
		}

		Station* arrived = currentPath[currentIndex + 1];
		if (!arrived) {
			return;
		}

		std::string name = arrived->getName();
		if (name.empty()) {
			return;
		}

		ui.txtOutput->append("Llego a: " + QString::fromUtf8(name.c_str()));

		currentIndex++;

		if (currentIndex >= currentPath.size() - 1)
		{
			ui.txtOutput->append("Ruta completada.");
			animationTimer->stop();
			isMoving = false;
			return;
		}

		if (currentIndex + 1 < currentPath.size() && currentPath[currentIndex + 1])
			targetPos = QPointF(currentPath[currentIndex + 1]->getX(),
				currentPath[currentIndex + 1]->getY());
		else
		{
			animationTimer->stop();
			isMoving = false;
		}

		return;
	}

	QPointF step = (direction / distance) * moveSpeed;
	currentPos += step;
	vehicle->setPos(currentPos);
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
	drawGraph();

	QString mapPath = QDir::currentPath() + "/../Resources/Mapa.png";
	QPixmap mapPixmap(mapPath);
	if (!mapPixmap.isNull()) {
		QGraphicsPixmapItem* mapItem = scene->addPixmap(mapPixmap);
		mapItem->setZValue(-1);
	}
}

void SimulationView::loadGraphFromFiles()
{
	stations = Archivos::cargarEstacionesUsuario(currentUsername);
	routes = Archivos::cargarRutasUsuario(currentUsername, stations);

	graph = Graph();
	for (auto* s : stations) graph.addStation(s);
	for (auto* r : routes)  graph.addRoute(r);

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

	for (auto* r : routes) {
		QPointF p1(r->getStart()->getX(), r->getStart()->getY());
		QPointF p2(r->getEnd()->getX(), r->getEnd()->getY());
		QPen pen;
		if (r->isClosed())
			pen = QPen(QColor("#FF5555"), 3, Qt::DashLine);
		else
			pen = QPen(QColor("#00FF88"), 2, Qt::SolidLine);

		QGraphicsLineItem* line = scene->addLine(QLineF(p1, p2), pen);
		line->setZValue(0);

		QPointF mid = (p1 + p2) / 2;
		QGraphicsTextItem* costText = scene->addText(r->getDisplayText());
		costText->setDefaultTextColor(Qt::white);
		costText->setPos(mid.x() - 10, mid.y() - 20);
		costText->setZValue(5);
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

		QGraphicsTextItem* label = scene->addText(s->getDisplayText());
		label->setDefaultTextColor(Qt::white);
		label->setZValue(5);
		label->setPos(pos.x() - 30, pos.y() - 50);
	}

	scene->update();
}