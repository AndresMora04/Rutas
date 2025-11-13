#include "MapView.h"

MapView::MapView(const string& username, QWidget* parent)
	: QMainWindow(parent),
	scene(new QGraphicsScene(this)),
	mapItem(nullptr),
	currentUsername(username)
{
	ui.setupUi(this);

	ui.cBoxType->clear();
	ui.cBoxType->addItem("Station");
	ui.cBoxType->addItem("Stop");
	ui.cBoxType->setCurrentIndex(0);
	ui.cBoxType->setEnabled(false);

	ui.graphicsMap->setScene(scene);
	ui.graphicsMap->setRenderHint(QPainter::Antialiasing);
	ui.graphicsMap->setDragMode(QGraphicsView::ScrollHandDrag);
	ui.graphicsMap->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	ui.graphicsMap->setResizeAnchor(QGraphicsView::AnchorViewCenter);

	connect(ui.btnAddStation, &QPushButton::clicked, this, &MapView::onActionBtnAddStation);
	connect(ui.btnAddRoute, &QPushButton::clicked, this, &MapView::onActionBtnAddRoute);
	connect(ui.btnDelete, &QPushButton::clicked, this, &MapView::onActionBtnDelete);
	connect(ui.btnRefresh, &QPushButton::clicked, this, &MapView::onActionBtnRefresh);
	connect(ui.btnDeleteRoute, &QPushButton::clicked, this, &MapView::onActionBtnDeleteRoute);
	connect(ui.btnCloseRoute, &QPushButton::clicked, this, &MapView::onActionBtnCloseRoute);
	connect(ui.btnOpenRoute, &QPushButton::clicked, this, &MapView::onActionBtnOpenRoute);

	loadMapImage("/../Resources/Mapa.png");
	ui.graphicsMap->viewport()->installEventFilter(this);

	logicalStations = Archivos::cargarEstacionesUsuario(currentUsername);
	logicalRoutes = Archivos::cargarRutasUsuario(currentUsername, logicalStations);

	redrawLoadedStations();
	redrawLoadedRoutes();
}

MapView::~MapView()
{
}

void MapView::loadMapImage(const QString& fileName)
{
	QString path = QDir::currentPath() + "/../Resources/" + fileName;
	QFile file(path);

	if (!file.exists()) {
		QMessageBox::warning(this, "Error", "No se encontro la imagen del mapa:\n" + path);
		return;
	}

	QPixmap mapPixmap(path);
	if (mapPixmap.isNull()) {
		QMessageBox::warning(this, "Error", "No se pudo cargar la imagen del mapa.");
		return;
	}

	mapItem = scene->addPixmap(mapPixmap);
	mapItem->setZValue(-1);
	scene->setSceneRect(mapPixmap.rect());

}

void MapView::onActionBtnAddStation()
{
	addingStation = true;
	ui.cBoxType->setEnabled(true);
	QMessageBox::information(this, "Modo activo", "Haz clic sobre el mapa para agregar una parada.");
}

void MapView::onActionBtnAddRoute()
{
	if (stations.size() < 2) {
		QMessageBox::warning(this, "Error", "Debe haber al menos dos paradas para crear una ruta.");
		return;
	}

	addingRoute = true;
	firstStation = nullptr;
	QMessageBox::information(this, "Modo ruta", "Selecciona dos paradas para conectar.");
}

void MapView::onActionBtnDelete()
{
	if (logicalStations.empty()) {
		QMessageBox::information(this, "Eliminar", "No hay estaciones para eliminar.");
		return;
	}

	deletingStation = true;
	QMessageBox::information(this, "Modo eliminar", "Haz clic sobre una estación para eliminarla.");
}

void MapView::updateStationCount()
{
	ui.lblStationsCount->setText("Total de paradas: " + QString::number(stations.size()));
}

bool MapView::eventFilter(QObject* obj, QEvent* event)
{
	if (obj == ui.graphicsMap->viewport() && event->type() == QEvent::MouseButtonPress)
	{
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

		if (deletingStation && mapItem)
		{
			QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
			QPointF scenePos = ui.graphicsMap->mapToScene(mouseEvent->pos());

			QGraphicsPixmapItem* clickedStation = nullptr;
			Station* stationToRemove = nullptr;

			for (size_t i = 0; i < stations.size(); ++i) {
				if (stations[i]->contains(stations[i]->mapFromScene(scenePos))) {
					clickedStation = reinterpret_cast<QGraphicsPixmapItem*>(stations[i]);
					if (i < logicalStations.size())
						stationToRemove = logicalStations[i];
					break;
				}
			}

			if (clickedStation && stationToRemove)
			{
				QVariant textVar = clickedStation->data(0);
				if (textVar.isValid()) {
					QGraphicsTextItem* label = static_cast<QGraphicsTextItem*>(textVar.value<void*>());
					if (label) {
						scene->removeItem(label);
						delete label;
					}
				}

				vector<Route*> updatedRoutes;
				for (auto* route : logicalRoutes)
				{
					if (route->getStart() != stationToRemove && route->getEnd() != stationToRemove)
						updatedRoutes.push_back(route);
					else
						scene->removeItem(scene->addLine(QLineF(
							QPointF(route->getStart()->getX(), route->getStart()->getY()),
							QPointF(route->getEnd()->getX(), route->getEnd()->getY())
						)));
				}
				logicalRoutes = updatedRoutes;

				scene->removeItem(clickedStation);
				stations.erase(remove(stations.begin(), stations.end(), reinterpret_cast<QGraphicsEllipseItem*>(clickedStation)), stations.end());

				logicalStations.erase(remove(logicalStations.begin(), logicalStations.end(), stationToRemove), logicalStations.end());
				delete stationToRemove;

				Archivos::guardarEstacionesUsuario(currentUsername, logicalStations);
				Archivos::guardarRutasUsuario(currentUsername, logicalRoutes);

				updateStationCount();
				QMessageBox::information(this, "Estacion eliminada", "La estación se elimino correctamente.");
			}
			else {
				QMessageBox::warning(this, "Eliminar", "No se selecciono una estación valida.");
			}

			deletingStation = false;
			return true;
		}

		if (deletingRoute && mapItem)
		{
			QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
			QPointF scenePos = ui.graphicsMap->mapToScene(mouseEvent->pos());

			QGraphicsLineItem* clickedRoute = nullptr;
			Route* routeToRemove = nullptr;

			for (size_t i = 0; i < routes.size(); ++i) {
				QLineF line = routes[i]->line();
				if (line.p1().x() - 5 <= scenePos.x() && scenePos.x() <= line.p2().x() + 5 &&
					line.p1().y() - 5 <= scenePos.y() && scenePos.y() <= line.p2().y() + 5) {
					clickedRoute = routes[i];
					if (i < logicalRoutes.size())
						routeToRemove = logicalRoutes[i];
					break;
				}
			}

			if (clickedRoute && routeToRemove) {
				scene->removeItem(clickedRoute);
				routes.erase(remove(routes.begin(), routes.end(), clickedRoute), routes.end());
				logicalRoutes.erase(remove(logicalRoutes.begin(), logicalRoutes.end(), routeToRemove), logicalRoutes.end());
				delete routeToRemove;

				Archivos::guardarRutasUsuario(currentUsername, logicalRoutes);
				QMessageBox::information(this, "Ruta eliminada", "Ruta eliminada correctamente.");
			}
			else {
				QMessageBox::warning(this, "Eliminar", "No se selecciono una ruta valida.");
			}

			deletingRoute = false;
			return true;
		}

		if ((closingRoute || openingRoute) && mapItem)
		{
			QPointF scenePos = ui.graphicsMap->mapToScene(mouseEvent->pos());

			QGraphicsLineItem* clickedRoute = nullptr;
			Route* targetRoute = nullptr;

			for (size_t i = 0; i < routes.size(); ++i) {
				QLineF line = routes[i]->line();
				qreal distance = QLineF(line.p1(), scenePos).length() + QLineF(scenePos, line.p2()).length() - line.length();
				if (fabs(distance) <= 8.0) {
					clickedRoute = routes[i];
					if (i < logicalRoutes.size())
						targetRoute = logicalRoutes[i];
					break;
				}
			}

			if (!clickedRoute || !targetRoute) {
				QMessageBox::warning(this, "Ruta no encontrada", "No se detecto ninguna ruta valida en esa posicion.");
				closingRoute = openingRoute = false;
				return true;
			}

			if (closingRoute) {
				if (targetRoute->isClosed()) {
					QMessageBox::information(this, "Ruta ya cerrada", "Esta ruta ya estaba cerrada.");
				}
				else {
					targetRoute->setClosed(true);
					clickedRoute->setPen(QPen(QColor("#FF5555"), 3, Qt::DashLine));
					Archivos archivos;
					archivos.guardarRutasUsuario(currentUsername, logicalRoutes);
					archivos.guardarCierresUsuario(currentUsername, logicalRoutes);
					QMessageBox::information(this, "Ruta cerrada", "La ruta fue cerrada exitosamente.");
				}
			}
			else if (openingRoute) {
				if (!targetRoute->isClosed()) {
					QMessageBox::information(this, "Ruta abierta", "Esta ruta ya estaba abierta.");
				}
				else {
					targetRoute->setClosed(false);
					clickedRoute->setPen(QPen(QColor("#00FF88"), 3, Qt::SolidLine));
					Archivos archivos;
					archivos.guardarRutasUsuario(currentUsername, logicalRoutes);
					archivos.guardarCierresUsuario(currentUsername, logicalRoutes);
					QMessageBox::information(this, "Ruta abierta", "La ruta fue reabierta exitosamente.");
				}
			}

			closingRoute = openingRoute = false;
			return true;
		}

		if (addingStation && mapItem)
		{
			addingStation = false;
			ui.cBoxType->setEnabled(false);

			QPointF scenePos = ui.graphicsMap->mapToScene(mouseEvent->pos());
			if (mapItem->contains(mapItem->mapFromScene(scenePos)))
			{
				for (auto* existing : logicalStations) {
					if (fabs(existing->getX() - scenePos.x()) < 10 &&
						fabs(existing->getY() - scenePos.y()) < 10) {
						QMessageBox::warning(this, "Duplicado", "Ya existe una estacion cerca de este punto.");
						return true;
					}
				}

				QString iconPath = QDir::currentPath() + "/../Resources/Parada.png";
				QPixmap icon(iconPath);
				icon = icon.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);

				if (!icon.isNull()) {
					QGraphicsPixmapItem* marker = scene->addPixmap(icon);
					marker->setZValue(1);
					marker->setPos(scenePos - QPointF(icon.width() / 2, icon.height() / 2));

					string type = ui.cBoxType->currentText().toUtf8().constData();

					int newId = 1;
					for (auto* existing : logicalStations) {
						if (existing->getId() >= newId)
							newId = existing->getId() + 1;
					}

					string name = "Estacion " + to_string(newId);

					Station* st = new Station(newId, name, scenePos.x(), scenePos.y(), type);
					logicalStations.push_back(st);
					stations.push_back(reinterpret_cast<QGraphicsEllipseItem*>(marker));
					Archivos::guardarEstacionesUsuario(currentUsername, logicalStations);
				}

				updateStationCount();
				return true;
			}
		}
		else if (addingRoute && mapItem)
		{
			QPointF scenePos = ui.graphicsMap->mapToScene(mouseEvent->pos());

			QGraphicsPixmapItem* clickedStation = nullptr;
			for (auto* st : stations)
			{
				if (st->contains(st->mapFromScene(scenePos))) {
					clickedStation = reinterpret_cast<QGraphicsPixmapItem*>(st);
					break;
				}
			}

			if (!clickedStation)
				return QMainWindow::eventFilter(obj, event);

			if (!firstStation)
			{
				firstStation = clickedStation;
			}
			else
			{
				QPointF p1 = firstStation->scenePos() + QPointF(30, 30);
				QPointF p2 = clickedStation->scenePos() + QPointF(30, 30);

				QLineF line(p1, p2);
				QGraphicsLineItem* routeLine = scene->addLine(line, QPen(QColor("#00FF88"), 3, Qt::SolidLine, Qt::RoundCap));
				routeLine->setZValue(0.5);
				routes.push_back(routeLine);

				Station* startStation = nullptr;
				Station* endStation = nullptr;

				for (auto* s : logicalStations)
				{
					if (abs(s->getX() - p1.x()) < 35 && abs(s->getY() - p1.y()) < 35)
						startStation = s;
					else if (abs(s->getX() - p2.x()) < 35 && abs(s->getY() - p2.y()) < 35)
						endStation = s;
				}

				if (startStation && endStation)
				{
					for (auto* existing : logicalRoutes) {
						if ((existing->getStart() == startStation && existing->getEnd() == endStation) ||
							(existing->getStart() == endStation && existing->getEnd() == startStation)) {
							QMessageBox::warning(this, "Ruta duplicada", "Ya existe una ruta entre esas estaciones.");
							addingRoute = false;
							firstStation = nullptr;
							return true;
						}
					}

					double cost = QLineF(p1, p2).length();
					Route* route = new Route(startStation, endStation, cost, false);
					logicalRoutes.push_back(route);

					Archivos::guardarRutasUsuario(currentUsername, logicalRoutes);
				}

				addingRoute = false;
				firstStation = nullptr;
			}
		}
	}

	return QMainWindow::eventFilter(obj, event);
}

void MapView::redrawLoadedStations()
{
	if (logicalStations.empty()) {
		return;
	}

	for (auto* st : logicalStations)
	{
		QString iconPath = QDir::currentPath() + "/../Resources/Parada.png";
		QPixmap icon(iconPath);
		icon = icon.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);

		if (!icon.isNull()) {
			QGraphicsPixmapItem* marker = scene->addPixmap(icon);
			marker->setZValue(1);
			marker->setPos(QPointF(st->getX(), st->getY()) - QPointF(icon.width() / 2, icon.height() / 2));
			stations.push_back(reinterpret_cast<QGraphicsEllipseItem*>(marker));

			QString labelText = QString::fromUtf8(st->getName().c_str()) +
				" (" + QString::fromUtf8(st->getType().c_str()) + ")";
			QGraphicsTextItem* label = scene->addText(labelText);
			label->setDefaultTextColor(Qt::white);
			label->setZValue(5);
			label->setPos(st->getX() - 30, st->getY() - 50);

			marker->setData(0, QVariant::fromValue<void*>(label));
		}
	}

	updateStationCount();
}

void MapView::redrawLoadedRoutes()
{
	if (logicalRoutes.empty()) return;

	vector<pair<int, int>> cierres = Archivos::cargarCierresUsuario(currentUsername);

	for (auto* route : logicalRoutes)
	{
		Station* start = route->getStart();
		Station* end = route->getEnd();
		if (!start || !end)
			continue;

		bool cerrada = false;
		for (auto& c : cierres) {
			if ((c.first == start->getId() && c.second == end->getId()) ||
				(c.first == end->getId() && c.second == start->getId())) {
				cerrada = true;
				break;
			}
		}

		route->setClosed(cerrada);

		QPen pen;
		if (cerrada)
			pen = QPen(QColor("#FF5555"), 3, Qt::DashLine);
		else
			pen = QPen(QColor("#00FF88"), 3, Qt::SolidLine, Qt::RoundCap);

		QGraphicsLineItem* line = scene->addLine(
			QLineF(QPointF(start->getX(), start->getY()), QPointF(end->getX(), end->getY())),
			pen
		);
		line->setZValue(0.5);
		routes.push_back(line);

		QPointF mid = (QPointF(start->getX(), start->getY()) + QPointF(end->getX(), end->getY())) / 2;
		QString costText = QString::number(route->getCost(), 'f', 1);
		QGraphicsTextItem* costLabel = scene->addText(costText);
		costLabel->setDefaultTextColor(Qt::cyan);
		costLabel->setZValue(5);
		costLabel->setPos(mid.x() - 10, mid.y() - 20);

		line->setData(0, QVariant::fromValue<void*>(costLabel));
	}
}

void MapView::onActionBtnRefresh()
{
	scene->clear();
	stations.clear();
	routes.clear();
	logicalStations = Archivos::cargarEstacionesUsuario(currentUsername);
	logicalRoutes = Archivos::cargarRutasUsuario(currentUsername, logicalStations);

	loadMapImage("/../Resources/Mapa.png");

	redrawLoadedStations();
	redrawLoadedRoutes();
	updateStationCount();

	QMessageBox::information(this, "Refrescado", "Mapa actualizado correctamente.");
}

void MapView::onActionBtnDeleteRoute()
{
	if (logicalRoutes.empty()) {
		QMessageBox::information(this, "Eliminar", "No hay rutas para eliminar.");
		return;
	}

	deletingRoute = true;
	QMessageBox::information(this, "Modo eliminar ruta", "Haz clic sobre una ruta para eliminarla.");
}

void MapView::onActionBtnCloseRoute()
{
	if (logicalRoutes.empty()) {
		QMessageBox::information(this, "Cerrar ruta", "No hay rutas disponibles para cerrar.");
		return;
	}

	closingRoute = true;
	openingRoute = false;
	QMessageBox::information(this, "Modo cerrar ruta", "Haz clic sobre la ruta que deseas cerrar.");
}

void MapView::onActionBtnOpenRoute()
{
	if (logicalRoutes.empty()) {
		QMessageBox::information(this, "Abrir ruta", "No hay rutas cerradas actualmente.");
		return;
	}

	openingRoute = true;
	closingRoute = false;
	QMessageBox::information(this, "Modo abrir ruta", "Haz clic sobre la ruta que deseas abrir.");
}