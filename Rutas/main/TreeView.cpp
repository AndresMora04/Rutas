#include "TreeView.h"

TreeView::TreeView(const std::string& username, QWidget* parent)
	: QMainWindow(parent),
	stationTree(new Tree()),
	scene(new QGraphicsScene(this)),
	currentUsername(username)
{
	ui.setupUi(this);

	ui.graphicsTree->setScene(scene);
	ui.graphicsTree->setBackgroundBrush(QBrush(QColor("#2b2b2b")));

	connect(ui.btnSearch, &QPushButton::clicked, this, &TreeView::onActionSearch);
	connect(ui.btnDelete, &QPushButton::clicked, this, &TreeView::onActionDelete);
	connect(ui.btnTraverse, &QPushButton::clicked, this, &TreeView::onActionTraverse);
	connect(ui.btnExport, &QPushButton::clicked, this, &TreeView::onActionExport);
	connect(ui.btnAdd, &QPushButton::clicked, this, &TreeView::onActionInsert);

	ui.cBoxTraversal->addItem("Inorder");
	ui.cBoxTraversal->addItem("Preorder");
	ui.cBoxTraversal->addItem("Postorder");

	loadStationsFromFile();
	ui.txtOutput->clear();
	drawTree();
}

TreeView::~TreeView()
{
	delete stationTree;
	delete scene;
}

void TreeView::onActionInsert()
{
	QString qName = ui.txtStationName->text().trimmed();
	std::string name = qName.toUtf8().constData();

	if (name.empty()) {
		QMessageBox::warning(this, "Error", "Ingrese el nombre de la estación a insertar.");
		return;
	}

	Station* existing = stationTree->searchByName(name);
	if (existing) {
		displayOutput(QString::fromUtf8(("La estacion '" + name + "' ya existe en el arbol.").c_str()));
		return;
	}

	int newId = QRandomGenerator::global()->bounded(50, 101);

	Station* newStation = new Station(newId, name, 0, 0, "TreeNode");

	stationTree->insert(newStation);
	displayOutput(QString::fromUtf8(("Estación insertada en el arbol: " + name + " (ID: " + std::to_string(newId) + ")").c_str()));

	ui.txtOutput->clear();
	drawTree();
}

void TreeView::onActionDelete()
{
	QString qName = ui.txtStationName->text().trimmed();
	string name = qName.toUtf8().constData();

	if (name.empty()) {
		QMessageBox::warning(this, "Error", "Ingrese el nombre de la estacion a eliminar.");
		return;
	}

	bool deleted = stationTree->deleteByName(name);
	if (deleted) {
		displayOutput(QString::fromUtf8(("Estacion eliminada: " + name).c_str()));
		ui.txtOutput->clear();
		drawTree();
	}
	else {
		displayOutput(QString::fromUtf8(("No se encontro la estacion: " + name).c_str()));
	}
}

void TreeView::onActionTraverse()
{
	QString mode = ui.cBoxTraversal->currentText();
	vector<string> result;

	if (mode == "Inorder") result = stationTree->getInOrder();
	else if (mode == "Preorder") result = stationTree->getPreOrder();
	else result = stationTree->getPostOrder();

	ui.txtOutput->append("?? Recorrido (" + mode + "):");
	QString joined;
	for (const string& name : result)
		joined += QString::fromUtf8(name.c_str()) + " ? ";
	ui.txtOutput->append(joined + "\n");
	ui.txtOutput->clear();
	drawTree();
}

void TreeView::onActionExport()
{
	vector<string> inorder = stationTree->getInOrder();
	vector<string> preorder = stationTree->getPreOrder();
	vector<string> postorder = stationTree->getPostOrder();

	Archivos::guardarRecorridosUsuario(currentUsername, inorder, preorder, postorder);

	QMessageBox::information(this, "Exportado",
		"Recorridos exportados correctamente al archivo del usuario.");
}

void TreeView::loadStationsFromFile()
{
	vector<Station*> stations = Archivos::cargarEstacionesUsuario(currentUsername);

	if (stations.empty()) {
		displayOutput("No hay estaciones registradas para este usuario.");
		return;
	}

	for (auto* st : stations)
		stationTree->insert(st);

	displayOutput(QString::fromUtf8(("Se cargaron " + to_string(stations.size()) +
		" estaciones del usuario " + currentUsername).c_str()));
}

void TreeView::drawTree()
{
	scene->clear();

	QString mode = ui.cBoxTraversal->currentText();
	vector<string> names;

	if (mode == "Inorder")
		names = stationTree->getInOrder();
	else if (mode == "Preorder")
		names = stationTree->getPreOrder();
	else
		names = stationTree->getPostOrder();

	QGraphicsTextItem* title = scene->addText(
		"Arbol binario de estaciones (" + mode + ")",
		QFont("Arial", 12, QFont::Bold)
	);
	title->setDefaultTextColor(Qt::yellow);
	title->setPos(20, 10);

	int y = 50;

	vector<Station*> allStations = Archivos::cargarEstacionesUsuario(currentUsername);

	for (const string& n : names) {
		Station* s = nullptr;
		for (auto* st : allStations) {
			if (st->getName() == n) {
				s = st;
				break;
			}
		}

		QColor color = (s && s->getType() == "Stop") ? QColor("#3CB371") : QColor("#1E90FF");

		QGraphicsRectItem* rect = scene->addRect(100, y, 220, 30, QPen(Qt::white), QBrush(color));

		QString label;

		if (s) {
			label = QString("%1 (%2) [ID: %3]")
				.arg(QString::fromUtf8(s->getName().c_str()))
				.arg(QString::fromUtf8(s->getType().c_str()))
				.arg(s->getId());
		}
		else {
			label = QString::fromUtf8(n.c_str());
		}

		QGraphicsTextItem* text = scene->addText(label, QFont("Arial", 10, QFont::Bold));
		text->setDefaultTextColor(Qt::white);
		text->setPos(110, y + 5);

		y += 40;
	}
}

void TreeView::displayOutput(const QString& text)
{
	ui.txtOutput->append(text);
}

void TreeView::onActionSearch()
{
	QString qName = ui.txtStationName->text().trimmed();
	string name = qName.toUtf8().constData();

	if (name.empty()) {
		QMessageBox::warning(this, "Error", "Ingrese el nombre de la estacion a buscar.");
		return;
	}

	Station* found = stationTree->searchByName(name);

	if (found) {
		QString msg = QString("Estacion encontrada:\nNombre: %1\nID: %2")
			.arg(QString::fromUtf8(found->getName().c_str()))
			.arg(found->getId());
		displayOutput(msg);
	}
	else {
		displayOutput(QString::fromUtf8(("No se encontro la estacion: " + name).c_str()));
	}
}

void TreeView::showEvent(QShowEvent* event)
{
	QMainWindow::showEvent(event);

	scene->clear();
	ui.txtOutput->clear();

	delete stationTree;
	stationTree = new Tree();

	loadStationsFromFile();
	drawTree();

	ui.txtOutput->append("Arbol actualizado con las estaciones más recientes del mapa.\n");
}

