#include <Point.h>
#include <QPixmap>
#include <qpainter.h>
#include <qspinbox.h>
#include <qevent.h>
#include <QDebug>
#include <bmp.h>
#include <Matrix.h>
#include <iostream>
#include <dynamic_lib.h>
#include <filesystem>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent),
	  mPixMap(QPixmap(30, 30)),
	  mMat(900),
	  mLabel(this)
{
	ui.setupUi(this);
	ui.select->hide();
	ui.inputready->hide();
	ui.paintready->hide();
	mPixMap.fill(Qt::white);

	connect(ui.recongnise, &QPushButton::clicked, [=]()
			{ this->Identification(); });
	connect(ui.clear, &QPushButton::clicked, [=]()
			{ this->Clear(); });
	connect(ui.tarin, &QPushButton::clicked, [=]()
		{
			ui.select->show();
			ui.inputready->show();
		});
	connect(ui.inputready, &QPushButton::clicked, [=]() {
		if (ui.select->value() >= 30)
		{
			num_each = ui.select->value();
			ui.paintready->show();
		}
		else
		{
			std::wcout << "至少为30" << std::endl;
		}
		});
	connect(ui.paintready, &QPushButton::clicked, [=]() {
		if (num_now < num_each * 10)
		{
			char path[100];
			sprintf(path, "C:/work/vscoderope/HandwrittenCNNwithGUI/Training_set/%d/%d.bmp", num_now / num_each, num_now % num_each);
			savebmp_file(path);
			num_now++;
			if (num_now == num_each * 10)
			{
				num_each = 0;
				num_now = 0;
				ui.select->hide();
				ui.inputready->hide();
				ui.paintready->hide();
			}
		}
		});
	connect(ui.train_start, &QPushButton::clicked, [=]() {
		//std::string cpath = std::filesystem::current_path().string();
		Train();
		});

	mLabel.setText("识别的数字是:");
	mLabel.move(800, 350);
}

void MainWindow::DrawPoint(int x, int y)
{
	QPainter painter(&mPixMap);
	painter.setPen(QPen(Qt::black, 2));
	painter.drawPoint(x, y);
}

void MainWindow::Clear()
{
	std::wcout << "点击了Clear" << std::endl;
	mPixMap.fill(Qt::white);
	std::fill(mMat.begin(), mMat.end(), 0);
	update();
}

void MainWindow::Identification()
{
	std::wcout << "点击了ide" << std::endl;
	savebmp_file("C:/work/vscoderope/HandwrittenCNNwithGUI/test.bmp");
	recongise("C:/work/vscoderope/HandwrittenCNNwithGUI/test.bmp");
	// auto mat = mNet.Eval(mMat);
	//  long double maxNum = 0;
	//  int pos = 0, realPos = 0;

	// for (int i = 0; i < mat.GetCols(); i++)
	// {
	// 	if (mat.At(0, i) > maxNum)
	// 	{
	// 		pos = i;
	// 		maxNum = mat.At(0, i);
	// 	}
	// }
	// mLabel.setText("识别的数字是:" + QString::number(pos));
}

void MainWindow::Train()
{
	int numeach[10] = {0};
	int numall = 0;
	for (size_t i = 0; i < 10; i++)
	{
		for (size_t j = 0; ; j++)
		{
			char path[100];
			sprintf(path, "C:/work/vscoderope/HandwrittenCNNwithGUI/out/build/x64-debug/exetest/Training_set/%d/%d.bmp", i, j+1);
			if (!std::filesystem::exists(path))
			{
				std::wcout << "打开";
				std::cout << path;
				std::wcout << "失败" << std::endl;
				break;
			}
			else
			{
				numeach[i]++;
				numall++;
			}
		}
	}
	train_data_loader(numeach);
	train(20, numall);
}

std::vector<int> &MainWindow::getout()
{
	return this->mMat;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.drawPixmap(256, 200, 295, 295, mPixMap);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
	auto point = (event->pos() - QPoint(256, 200)) / 10;
	mStartPoint = point;
	// qDebug() << point.y() * 30 + point.x();
	//  if (point.x() >= 0 && point.x() < 30 && point.y() >= 0 && point.y() < 30)
	//  {
	//  	mMat[point.y() * 30 + point.x()] = 1;
	//  }
	//QRgb tpARGB;
	//QImage tpimg = mPixMap.toImage();
	//for (int i = 0; i < 30; i++)
	//{
	//	for (int j = 0; j < 30; j++)
	//	{
	//		tpARGB = tpimg.pixel(i, j);
	//		if (tpARGB == Qt::white) // 即0xFFFFFFFF(白色)
	//		{
	//			mMat[i * 30 + j] = 0;
	//		}
	//		else
	//		{
	//			mMat[i * 30 + j] = 1;
	//		}
	//	}
	//}
	// savebmp();
	update();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
	// qDebug() << (event->pos() / 10) - QPoint(256, 200);
	QPainter *painter = new QPainter;

	auto point = (event->pos() - QPoint(256, 200)) / 10;

	painter->begin(&mPixMap);
	// painter->setRenderHint(QPainter::Antialiasing, true);//边缘渐变
	painter->setPen(QPen(Qt::black, 1.3, Qt::CustomDashLine));
	painter->drawLine(mStartPoint, point); // 绘制开始点到移动点的位置
	painter->end();
	mStartPoint = point;
	// qDebug() << point.y() * 30 + point.x();
	//  if (point.x() >= 0 && point.x() < 30 && point.y() >= 0 && point.y() < 30)
	//  {
	//  	mMat[point.y() * 30 + point.x()] = 1;
	//  }

	//savebmp();
	update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
}

void MainWindow::savebmp_mem()
{
	QRgb tpARGB;
	QImage tpimg = mPixMap.toImage();
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			tpARGB = tpimg.pixel(j, i);
			if (tpARGB == 0xFFFFFFFF) // 为白色
			{
				mMat[i * 30 + j] = 0;
			}
			else
			{
				mMat[i * 30 + j] = 1;
			}
		}
	}
}

void MainWindow::savebmp_file(std::string path)
{
	savebmp_mem();
	createBlackWhiteBMP(this->getout(), path.c_str());
}
