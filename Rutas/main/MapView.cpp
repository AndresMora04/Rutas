#include "MapView.h"

MapView::MapView(QWidget* parent)
	: QMainWindow(parent),
	scene(new QGraphicsScene(this)),
	mapItem(nullptr)
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

	loadMapImage("/../Resources/Mapa.png");
	ui.graphicsMap->viewport()->installEventFilter(this);
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
	for (auto* st : stations)
		scene->removeItem(st);
	stations.clear();
	updateStationCount();
	QMessageBox::information(this, "Eliminar", "Se eliminaron todas las paradas.");
}

void MapView::onActionBtnRefresh()
{
	for (auto* line : routes)
		scene->removeItem(line);
	routes.clear();

	loadMapImage("/../Resources/Mapa.png");

	QMessageBox::information(this, "Refrescar", "Mapa actualizado.");
	Archivos::guardarEstaciones(logicalStations);
	Archivos::guardarRutas(logicalRoutes);
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

		if (addingStation && mapItem)
		{
			QPointF scenePos = ui.graphicsMap->mapToScene(mouseEvent->pos());
			if (mapItem->contains(mapItem->mapFromScene(scenePos)))
			{
				QString iconPath = QDir::currentPath() + "/../Resources/Parada.png";
				QPixmap icon(iconPath);

				icon = icon.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
				if (!icon.isNull()) {
					QGraphicsPixmapItem* marker = scene->addPixmap(icon);
					marker->setZValue(1);
					marker->setPos(scenePos - QPointF(icon.width() / 2, icon.height() / 2));
					string type = ui.cBoxType->currentText().toUtf8().constData();
					string name = string("Station ") + to_string(logicalStations.size() + 1);
					Station* st = new Station(logicalStations.size() + 1, name, scenePos.x(), scenePos.y(), type);
					logicalStations.push_back(st);
					stations.push_back(reinterpret_cast<QGraphicsEllipseItem*>(marker));
				}
				updateStationCount();
				addingStation = false;
				ui.cBoxType->setEnabled(false);
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
					double cost = QLineF(p1, p2).length();
					Route* route = new Route(startStation, endStation, cost, false);
					logicalRoutes.push_back(route);

					Archivos::guardarRutas(logicalRoutes);
					qDebug() << "[INFO] Ruta guardada entre:"
						<< QString::fromStdString(startStation->getName())
						<< "y"
						<< QString::fromStdString(endStation->getName());
				}
				else
				{
					qDebug() << "[WARN] No se pudieron asociar estaciones lógicas para la ruta.";
				}

				addingRoute = false;
				firstStation = nullptr;
			}
		}
	}

	return QMainWindow::eventFilter(obj, event);
}