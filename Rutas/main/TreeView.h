#pragma once

#include <QMainWindow>
#include "ui_TreeView.h"

class TreeView : public QMainWindow
{
	Q_OBJECT

public:
	TreeView(QWidget *parent = nullptr);
	~TreeView();

private:
	Ui::TreeViewClass ui;
};

