#pragma once
#ifndef MYPAINTERWIDGET_H
#define MYPAINTERWIDGET_H

#include <QWidget>
#include <QPoint>
#include <vector>
#include <QString>
#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <iostream>
#include "edge.h"

using namespace std;

class MyPainterWidget: public QWidget
{
public:
    MyPainterWidget(QWidget* parent);
    ~MyPainterWidget();

    //继承
    void paintEvent(QPaintEvent* p);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

	//initial
	void initial();
	//获取边
	edge getEdge(QPoint staP,QPoint endP);
	//构造边集
	void buildEdgeTable();
	//填充颜色
	void fillingPolygon();
	//扫描
	void scanning();
	//画线
	void drawLines(int x1,int x2,int y);
	//检测pointNum是否越界
	void checkPointArray();
	//检测edgeNum是否越界
	void checkEdgeTable();
	//冒泡排序
	void bubbleSort(int len,int* xPos);
	
	int YMAX;						//存储点的最大值
	int APNum;						//活跃点数目
	int APointY;					//记录当前扫描纵坐标
	QPoint staPoint;				//起点
	QPoint endPoint;				//终点
	bool ifFinished;				//判断是否结束画边
	bool ifFilling;					//是否填充
	QColor qColor;					//存储当前颜色
	int edgeNum;					//存储边数目
	int pointNum;					//存储顶点数目
	int maxPointNum;				//记录顶点动态数组长度	
	int maxEdgeNum;					//记录边动态数组长度
	int** xPos;						//存储活跃点坐标
	int* xLen;						//存储长度
	QPoint* pointArray;				//存储多边形顶点坐标
	edge* edgeTable;				//存储边集
	edge* AEL;						//存储活跃边
	edge** YPos;					//标记活跃边起始点


};

#endif // MYPAINTERWIDGET_H
