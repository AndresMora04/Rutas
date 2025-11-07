#include "LogInView.h"

LogInView::LogInView(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.btnLogIn, &QPushButton::clicked, this, &LogInView::onActionBtnLogIn);

	QString folderPath = QDir::currentPath() + "/../Transformations";
	QDir directory(folderPath);

	QStringList filters;
	filters << "*.png";
	directory.setNameFilters(filters);

	QFileInfoList fileList = directory.entryInfoList();

	for (int i = 0; i < fileList.size(); i++) {
		QString fileName = fileList.at(i).baseName();
		ui.cBoxCharacters->addItem(fileName);
	}

	if (ui.cBoxCharacters->count() == 0) {
		ui.cBoxCharacters->addItem("No se encontraron personajes");
	}
}

LogInView::~LogInView()
{
}

void LogInView::onActionBtnLogIn()
{
	QString qName = ui.txtName->text().trimmed();
	QString qChar = ui.cBoxCharacters->currentText().trimmed();

	string username = qName.toUtf8().constData();
	string selectedCharacter = qChar.toUtf8().constData();

	if (username.empty()) {
		QMessageBox::warning(this, "Error", "Por favor, ingrese un nombre de usuario.");
		return;
	}

	if (qChar.isEmpty() || qChar == "Seleccione una skin..." || qChar == "No se encontraron skins") {
		QMessageBox::warning(this, "Error", "Seleccione una skin valida.");
		return;
	}

	vector<User*> users = Archivos::cargarUsuarios();

	bool exists = false;
	for (auto* u : users) {
		if (u->getUsername() == username) {
			exists = true;
			break;
		}
	}

	if (!exists) {
		User* newUser = new User(username, selectedCharacter);
		Archivos::guardarUsuario(newUser);

		QString msg = QString("Usuario '%1' creado correctamente.").arg(QString::fromUtf8(username));
		QMessageBox::information(this, "Registro exitoso", msg);
	}
	else {
		QString msg = QString("Bienvenido de nuevo, %1!").arg(QString::fromUtf8(username));
		QMessageBox::information(this, "Bienvenido", msg);
	}

	MainView* mainView = new MainView(
		QString::fromUtf8(username),
		QString::fromUtf8(selectedCharacter)
	);
	mainView->show();

	this->close();
}