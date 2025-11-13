#include "ReportView.h"

ReportView::ReportView(const std::string& username, const std::string& character, QWidget* parent)
    : QMainWindow(parent),
      username(username),
      character(character)
{
    ui.setupUi(this);

    ui.cBoxReportType->addItem("Stations Report");
    ui.cBoxReportType->addItem("Routes Report");
    ui.cBoxReportType->addItem("Events Report");

    connect(ui.btnGenerate, &QPushButton::clicked, this, &ReportView::onActionBtnGenerate);
    connect(ui.btnExport, &QPushButton::clicked, this, &ReportView::onActionBtnExport);

    loadData();

    ui.txtOutput->setPlainText("Ready to generate reports.\n");
}

ReportView::~ReportView()
{
}

void ReportView::loadData()
{
    stations = Archivos::cargarEstacionesUsuario(username);
    routes = Archivos::cargarRutasUsuario(username, stations);
}

void ReportView::onActionBtnGenerate()
{
    QString type = ui.cBoxReportType->currentText();
    ui.txtOutput->clear();

    if (type == "Stations Report")
        generateStationReport();
    else if (type == "Routes Report")
        generateRouteReport();
    else
		generateClosuresReport();

    ui.txtOutput->append("\n? Report generated successfully.");
}

void ReportView::generateStationReport()
{
    if (stations.empty()) {
        ui.txtOutput->setText("?? No stations found.");
        return;
    }

    sort(stations.begin(), stations.end(),
        [](Station* a, Station* b) {
            return a->getName() < b->getName();
        });

    QString report = "?? STATIONS REPORT\n";
    report += "Generated: " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "\n\n";

    for (auto* s : stations) {
        report += QString("• %1 (Type: %2 | ID: %3 | X: %4 | Y: %5)\n")
            .arg(QString::fromUtf8(s->getName().c_str()))
            .arg(QString::fromUtf8(s->getType().c_str()))
            .arg(s->getId())
            .arg(s->getX())
            .arg(s->getY());
    }

    report += QString("\nTotal stations: %1\n").arg(stations.size());

    ui.txtOutput->setPlainText(report);
    generatedReport = report;
}

void ReportView::generateRouteReport()
{
    if (routes.empty()) {
        ui.txtOutput->setText("?? No routes found.");
        return;
    }

    QString report = "?? ROUTES REPORT\n";
    report += "Generated: " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "\n\n";

    int closedCount = 0;
    for (auto* r : routes) {
        if (r->isClosed()) closedCount++;
        report += QString("• %1 ? %2 | Cost: %3 | Closed: %4\n")
            .arg(QString::fromUtf8(r->getStart()->getName().c_str()))
            .arg(QString::fromUtf8(r->getEnd()->getName().c_str()))
            .arg(r->getCost())
            .arg(r->isClosed() ? "Yes" : "No");
    }

    report += QString("\nTotal routes: %1\nClosed routes: %2\nOpen routes: %3\n")
        .arg(routes.size())
        .arg(closedCount)
        .arg(routes.size() - closedCount);

    ui.txtOutput->setPlainText(report);
    generatedReport = report;
}

void ReportView::generateClosuresReport()
{
	QString report = "?? CLOSURES REPORT\n\n";
	report += "Generated: " + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + "\n\n";

	Archivos archivos;
	vector<pair<int, int>> cierres = archivos.cargarCierresUsuario(username);

	if (cierres.empty()) {
		report += "(No closed routes found for user: " + QString::fromStdString(username) + ")\n";
		ui.txtOutput->setPlainText(report);
		generatedReport = report;
		return;
	}

	report += "Closed routes currently recorded:\n\n";
	int totalClosed = 0;

	for (auto& c : cierres) {
		int id1 = c.first;
		int id2 = c.second;

		Station* s1 = nullptr;
		Station* s2 = nullptr;

		for (auto* s : stations) {
			if (s->getId() == id1) s1 = s;
			if (s->getId() == id2) s2 = s;
		}

		QString name1 = s1 ? QString::fromUtf8(s1->getName().c_str()) : QString("Unknown(%1)").arg(id1);
		QString name2 = s2 ? QString::fromUtf8(s2->getName().c_str()) : QString("Unknown(%1)").arg(id2);

		report += QString("• %1 ? %2 | Status: CLOSED\n").arg(name1).arg(name2);
		totalClosed++;
	}

	report += QString("\nTotal closed routes: %1\n").arg(totalClosed);

	ui.txtOutput->setPlainText(report);
	generatedReport = report;
}

void ReportView::onActionBtnExport()
{
    if (generatedReport.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please generate a report before exporting.");
        return;
    }

    QString folderPath = "data/usuarios/" + QString::fromStdString(username);
    QDir dir(folderPath);
    if (!dir.exists())
        dir.mkpath(".");

    QString filePath = folderPath + "/reportes.txt";
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        QMessageBox::warning(this, "Error", "Unable to open report file for writing.");
        return;
    }

    QTextStream out(&file);
    QDateTime now = QDateTime::currentDateTime();
    QString dateStr = now.toString("yyyy-MM-dd hh:mm:ss");

    out << "\n============================================\n";
    out << "?? Report generated on: " << dateStr << "\n";
    out << "?? User: " << QString::fromStdString(username) << "\n";
    out << "?? Character: " << QString::fromStdString(character) << "\n";
    out << "============================================\n\n";
    out << generatedReport;
    out << "\n--------------------------------------------\n\n";

    file.close();

    QMessageBox::information(this, "Exported",
        "Report appended successfully to:\n" + filePath);
}

/*void ReportView::onActionBtnExport()
{
    if (generatedReport.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please generate a report before exporting.");
        return;
    }

    string folderPath = "data/usuarios/" + username;
    QDir dir(QString::fromStdString(folderPath));
    if (!dir.exists())
        dir.mkpath(".");

    string filePath = folderPath + "/reportes.txt";
    ofstream file(filePath, ios::trunc);

    if (!file.is_open()) {
        QMessageBox::warning(this, "Error", "Unable to open report file for writing.");
        return;
    }

    QDateTime now = QDateTime::currentDateTime();
    string dateStr = now.toString("yyyy-MM-dd hh:mm:ss").toStdString();

    file << "\n============================================\n";
    file << "?? Report generated on: " << dateStr << "\n";
    file << "?? User: " << username << "\n";
    file << "?? Character: " << character << "\n";
    file << "============================================\n\n";
    file << generatedReport.toStdString();
    file << "\n--------------------------------------------\n\n";

    file.close();

    QString qFilePath = QString::fromStdString(filePath);
    QMessageBox::information(
        this,
        "Exported",
        "Report appended successfully to:\n" + qFilePath
    );
}*/