#include "MapView.h"

MapView::MapView(QWidget* parent)
	: QMainWindow(parent),
	scene(new QGraphicsScene(this)),
	mapItem(nullptr)
{
	ui.setupUi(this);

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
	scene->clear();
	mapItem = nullptr;
	loadMapImage("/../Resources/Mapa.png");
	for (auto* st : stations)
		scene->addItem(st);
	QMessageBox::information(this, "Refrescar", "Mapa actualizado.");
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
					stations.append(reinterpret_cast<QGraphicsEllipseItem*>(marker));
				}
				updateStationCount();
				addingStation = false;
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

				routes.append(routeLine);

				string name1 = "Parada A";
				string name2 = "Parada B";
				Station* s1 = new Station(0, name1, p1.x(), p1.y());
				Station* s2 = new Station(0, name2, p2.x(), p2.y());
				Route* ruta = Utils::crearRuta(s1, s2);

				addingRoute = false;
				firstStation = nullptr;
			}
		}
	}

	return QMainWindow::eventFilter(obj, event);
}