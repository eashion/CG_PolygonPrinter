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
#include <qmenu.h>
#include <qfiledialog.h>
#include <qtextstream.h> 
#include <QTextStream>
#include <stdio.h>
#include "edge.h"

using namespace std;

class MyPainterWidget: public QWidget
{
public:
    MyPainterWidget(QWidget* parent);
    ~MyPainterWidget();

    //�̳�
    void paintEvent(QPaintEvent* p);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

	//initial
	void initial();
	//��ȡ��
	edge getEdge(QPoint staP,QPoint endP);
	//����߼�
	void buildEdgeTable();
	//�����ɫ
	void fillingPolygon();
	//ɨ��
	void scanning();
	//����
	void drawLines(int x1,int x2,int y);
	//���pointNum�Ƿ�Խ��
	void checkPointArray();
	//���edgeNum�Ƿ�Խ��
	void checkEdgeTable();
	//ð������
	void bubbleSort(int len,int* xPos);
	//���ļ���ȡ��Ϣ���
	void fileFilling();
	//������Ϣ���ļ�
	void fileSave();
	//���ļ��ж�ȡ�������Ϣ
	void readPolygonFromFile(QString str);
	//���������Ϣ���ļ�
	QString savePolygonToFile();

	//intתstr
	QString intToStr(int);
	
	//�޸��������
	void changeToFullFilling();
	void changeToShadowFilling();
	
	int YMAX;						//�洢������ֵ
	int YMIN;						//�洢�����Сֵ
	int APNum;						//��Ծ����Ŀ
	int APointY;					//��¼��ǰɨ��������
	QPoint staPoint;				//���
	QPoint endPoint;				//�յ�
	bool ifFinished;				//�ж��Ƿ��������
	bool ifFilling;					//�Ƿ����
	QColor qColor;					//�洢��ǰ��ɫ
	int edgeNum;					//�洢����Ŀ
	int pointNum;					//�洢������Ŀ
	int maxPointNum;				//��¼���㶯̬���鳤��	
	int maxEdgeNum;					//��¼�߶�̬���鳤��
	int fillingType;				//��¼��������
	int** xPos;						//�洢��Ծ������
	int* xLen;						//�洢����
	QPoint* pointArray;				//�洢����ζ�������
	edge* edgeTable;				//�洢�߼�
	edge* AEL;						//�洢��Ծ��
	edge** YPos;					//��ǻ�Ծ����ʼ��
	

};

#endif // MYPAINTERWIDGET_H
