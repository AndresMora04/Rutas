#include "TreeView.h"

TreeView::TreeView(QWidget* parent)
	: QMainWindow(parent),
	stationTree(new Tree()),
	scene(new QGraphicsScene(this))
{
	ui.setupUi(this);

	ui.graphicsTree->setScene(scene);
	ui.graphicsTree->setBackgroundBrush(QBrush(QColor("#2b2b2b")));

	connect(ui.btnSearch, &QPushButton::clicked, this, &TreeView::onActionSearch);
	connect(ui.btnDelete, &QPushButton::clicked, this, &TreeView::onActionDelete);
	connect(ui.btnTraverse, &QPushButton::clicked, this, &TreeView::onActionTraverse);
	connect(ui.btnExport, &QPushButton::clicked, this, &TreeView::onActionExport);

	ui.cBoxTraversal->addItem("Inorder");
	ui.cBoxTraversal->addItem("Preorder");
	ui.cBoxTraversal->addItem("Postorder");

	loadStationsFromFile();
	drawTree();
}

TreeView::~TreeView()
{
	delete stationTree;
	delete scene;
}

void TreeView::loadStationsFromFile()
{
	vector<Station*> stations = Archivos::cargarEstaciones();
	if (stations.empty()) {
		displayOutput("No hay estaciones registradas aun.");
		return;
	}

	for (auto* st : stations)
		stationTree->insert(st);

	displayOutput(QString::fromUtf8(("Se cargaron " + to_string(stations.size()) + " estaciones en el árbol.").c_str()));

}

void TreeView::drawTree()
{
	scene->clear();
	vector<string> names = stationTree->getInOrder();

	int y = 20;
	for (const string& n : names) {
		scene->addText(QString::fromUtf8(n.c_str()), QFont("Arial", 10, QFont::Bold))->setPos(20, y);
		y += 25;
	}
}

void TreeView::displayOutput(const QString& text)
{
	ui.txtOutput->append(text);
}

void TreeView::onActionSearch()
{
	QString text = ui.txtStationName->text().trimmed();
	if (text.isEmpty()) {
		QMessageBox::warning(this, "Error", "Ingrese el ID de la estacion a buscar.");
		return;
	}

	int id = text.toInt();
	Station* found = stationTree->search(id);

	if (found)
		displayOutput(QString("Estacion encontrada: %1 (ID %2)")
			.arg(QString::fromStdString(found->getName()))
			.arg(found->getId()));
	else
		displayOutput("No se encontro la estacion con ese ID.");
}

void TreeView::onActionDelete()
{
	QMessageBox::information(this, "Eliminar", "Funcion de eliminar aun no implementada.");
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
}

void TreeView::onActionExport()
{
	QString filePath = "data/recorridos_rutas.txt";
	QFile file(filePath);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QMessageBox::warning(this, "Error", "No se pudo abrir el archivo para exportar.");
		return;
	}

	QTextStream out(&file);
	vector<string> inorder = stationTree->getInOrder();
	vector<string> preorder = stationTree->getPreOrder();
	vector<string> postorder = stationTree->getPostOrder();

	out << "INORDER:\n";
	for (auto& n : inorder) out << QString::fromUtf8(n.c_str()) << " ? ";
	out << "\n\nPREORDER:\n";
	for (auto& n : preorder) out << QString::fromUtf8(n.c_str()) << " ? ";
	out << "\n\nPOSTORDER:\n";
	for (auto& n : postorder) out << QString::fromUtf8(n.c_str()) << " ? ";

	file.close();
	QMessageBox::information(this, "Exportado",
		"Recorridos exportados a data/recorridos_rutas.txt.");
}

void TreeView::showEvent(QShowEvent* event)
{
    QMainWindow::showEvent(event);

    scene->clear();
    delete stationTree;
    stationTree = new Tree();

    loadStationsFromFile();
    drawTree();
}
