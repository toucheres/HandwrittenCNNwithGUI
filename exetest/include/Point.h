#pragma once
//#include"Network.h"
#include <QtWidgets/QMainWindow>
#include "ui_Point.h"
#include<qpushbutton.h>
#include<vector>
#include <QLabel>
#include <qspinbox.h>
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

    void DrawPoint(int x, int y);
public Q_SLOTS:
    void Clear();
    void Identification();
    void Train();
    std::vector<int> &getout();

protected: 
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
private:
    Ui::PointClass ui;
    //Network mNet;
    QPixmap mPixMap;
    QPoint mStartPoint,mEndPoint;
    std::vector<int>mMat;
    QLabel mLabel;
    void savebmp_mem();
    void savebmp_file(std::string path);
    int num_each=0;
    int num_now=0;
};
