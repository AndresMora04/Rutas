#include "LogInView.h"

LogInView::LogInView(QWidget *parent)
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
{}

void LogInView::onActionBtnLogIn()
{
    MainView* mainView = new MainView();

    mainView->show();

    this->close();
}