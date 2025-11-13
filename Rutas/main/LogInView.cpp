#include "LogInView.h"

LogInView::LogInView(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.btnLogIn, &QPushButton::clicked, this, &LogInView::onActionBtnLogIn);

	QString folderPath = QDir::currentPath() + "/../Buses";
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
		ui.cBoxCharacters->addItem("No se encontraron buses");
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

	vector<User*> users = Archivos::cargarUsuarios();

	User* existingUser = nullptr;
	for (auto* u : users) {
		if (u->getUsername() == username) {
			existingUser = u;
			break;
		}
	}

	if (!existingUser) {
		if (qChar.isEmpty() || qChar == "Seleccione un bus..." || qChar == "No se encontraron buses") {
			QMessageBox::warning(this, "Error", "Seleccione un bus valido antes de continuar.");
			return;
		}

		User* newUser = new User(username, selectedCharacter);
		Archivos::guardarUsuario(newUser);

		QString msg = QString("Usuario '%1' creado correctamente con el bus '%2'.")
			.arg(QString::fromUtf8(username))
			.arg(QString::fromUtf8(selectedCharacter));
		QMessageBox::information(this, "Registro exitoso", msg);
	}
	else {
		selectedCharacter = existingUser->getSelectedCharacter();
		ui.cBoxCharacters->setCurrentText(QString::fromUtf8(selectedCharacter.c_str()));
		ui.cBoxCharacters->setEnabled(false);

		QString msg = QString("Bienvenido de nuevo, %1!\nTu bus asignado es: %2.")
			.arg(QString::fromUtf8(username))
			.arg(QString::fromUtf8(selectedCharacter.c_str()));
		QMessageBox::information(this, "Bienvenido", msg);
	}
	cout << "Inicio de sesion exitoso." << endl;
	MainView* mainView = new MainView(
		QString::fromUtf8(username),
		QString::fromUtf8(selectedCharacter)
	);
	mainView->show();

	this->close();
}