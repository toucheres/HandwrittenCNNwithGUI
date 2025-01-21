#include "mainWindow.h"

mainWindow::mainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	int num_hight = 30;
	int num_weight = 30;
	for (size_t i = 0; i < num_hight*num_weight; i++)
	{
		
	}
	
}

mainWindow::~mainWindow()
{}
