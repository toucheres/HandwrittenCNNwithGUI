#include <Point.h>
#include <QPixmap>
#include<qpainter.h>
#include<qevent.h>
#include<QDebug>
#include <bmp.h>
#include <Matrix.h>
#include <dynamic_lib.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
	mPixMap(QPixmap(30,30)),
	mMat(900),
	mLabel(this)
{

	mPixMap.fill(Qt::white);
	ui.setupUi(this);
	connect(ui.recongnise, &QPushButton::clicked, [=]()
			{ this->Identification(); });
	connect(ui.clear, &QPushButton::clicked, [=]()
			{ this->Clear(); });
	//mNet.Load("model.json");
	mLabel.setText("识别的数字是:");
	mLabel.move(800,350);
}
    
	
	

void MainWindow::DrawPoint(int x, int y)
{
	QPainter painter(&mPixMap);
	painter.setPen(QPen(Qt::black, 2));
	painter.drawPoint(x,y);
}

void MainWindow::Clear()
{
	qDebug() << "点击了clear";
	mPixMap.fill(Qt::white);
	std::fill(mMat.begin(),mMat.end(),0);
	update();
}

void MainWindow::Identification()
{
	qDebug() << "点击了ide";
	createBlackWhiteBMP(this->getout(), "C:/work/vscoderope/HandwrittenCNNwithGUI/test.bmp");
	recongise("C:/work/vscoderope/HandwrittenCNNwithGUI/test.bmp");
	//auto mat = mNet.Eval(mMat);
	// long double maxNum = 0;
	// int pos = 0, realPos = 0;

	// for (int i = 0; i < mat.GetCols(); i++)
	// {
	// 	if (mat.At(0, i) > maxNum)
	// 	{
	// 		pos = i;
	// 		maxNum = mat.At(0, i);
	// 	}
	// }
	//mLabel.setText("识别的数字是:" + QString::number(pos));
}

std::vector<int> &MainWindow::getout()
{
	return this->mMat;
}

void MainWindow::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	painter.drawPixmap(256, 200, 295 ,295, mPixMap);
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
	auto point = (event->pos() - QPoint(256, 200)) / 10;
	mStartPoint = point;
	//qDebug() << point.y() * 30 + point.x();
	// if (point.x() >= 0 && point.x() < 30 && point.y() >= 0 && point.y() < 30)
	// {
	// 	mMat[point.y() * 30 + point.x()] = 1;
	// }
	QRgb tpARGB;
	QImage tpimg = mPixMap.toImage();
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			tpARGB = tpimg.pixel(i, j);
			if (tpARGB == Qt::white) // 即0xFFFFFFFF(白色)
			{
				mMat[i * 30 + j] = 0;
			}
			else
			{
				mMat[i * 30 + j] = 1;
			}
		}
	}
	update();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
	//qDebug() << (event->pos() / 10) - QPoint(256, 200);
	QPainter* painter = new QPainter;
	
	auto point = (event->pos() - QPoint(256, 200)) / 10;

	painter->begin(&mPixMap);
	//painter->setRenderHint(QPainter::Antialiasing, true);//边缘渐变
	painter->setPen(QPen(Qt::black, 1.3, Qt::CustomDashLine));
	painter->drawLine(mStartPoint, point); //绘制开始点到移动点的位置
	painter->end();
	mStartPoint = point;
	//qDebug() << point.y() * 30 + point.x();
	// if (point.x() >= 0 && point.x() < 30 && point.y() >= 0 && point.y() < 30)
	// {
	// 	mMat[point.y() * 30 + point.x()] = 1;
	// }

	QRgb tpARGB;
	QImage tpimg = mPixMap.toImage();
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			tpARGB = tpimg.pixel(j, i);
			if (tpARGB == 0xFFFFFFFF)//为白色
			{
				mMat[i * 30 + j] = 0;
			}
			else
			{
				mMat[i * 30 + j] = 1;
			}
		}
		
	}
	update();
	
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
	
}
