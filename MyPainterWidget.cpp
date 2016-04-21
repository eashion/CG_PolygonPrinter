#include "mypainterwidget.h"


MyPainterWidget::MyPainterWidget(QWidget* parent)
     :QWidget(parent){


	YMAX = 0;
	YMIN = 1100;
	edgeNum = 0;
	pointNum = 0;
	APNum = 0;
	APointY = 0;
	maxPointNum = 100;
	maxEdgeNum = 100;
	qColor = Qt::red;
	ifFinished = false;
	ifFilling = false;
	fillingType = 0;
	YPos = new edge*[1100];
	xPos = new int*[1100];
	xLen = new int[1100];
	pointArray = new QPoint[maxPointNum];
	edgeTable = new edge[maxEdgeNum];

	//初始化指针列表
	AEL = NULL;
	for( int i = 0; i < 1100; i++ ){
		YPos[i] = NULL;
	}


	//设置大小自适应
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); 

	//打开鼠标跟踪
	setMouseTracking(true);
}

MyPainterWidget::~MyPainterWidget(){
	delete []pointArray;
	delete []edgeTable;
	delete []YPos;
	delete []xPos;
	delete []xLen;
}

//初始化变量及数组
void MyPainterWidget::initial(){
	
	YMAX= 0;
	YMIN = 1100;
	edgeNum = 0;
	pointNum = 0;
	APNum = 0;
	APointY = 0;
	maxPointNum = 100;
	maxEdgeNum = 100;
	qColor = Qt::red;
	ifFinished = false;
	ifFilling = false;

	//初始化指针列表
	AEL = NULL;
	for( int i = 0; i < 1100; i++ ){
		YPos[i] = NULL;
	}

}


void MyPainterWidget::paintEvent(QPaintEvent*p){
    QPainter painter(this);
    QPen pen;									//创建一个画笔
	pen.setColor(qColor);
    pen.setWidth(1);
    painter.setPen(pen);

	for( int i = 0;i < pointNum-1;i++ ){
		painter.drawLine(pointArray[i],pointArray[i+1]);
	}

	//结束画图
	if( ifFinished == true ){

		//封闭
		painter.drawLine(pointArray[pointNum-1],pointArray[0]);

		int flag = 0;
		//绘制
		for( int i = YMIN; i <= YMAX; i++ ){
			for( int j = 0; j < xLen[i]; j+=2 ){
				if( fillingType == 1 ){
					if( (flag/20)%2 == 0 ){
						staPoint.setX(xPos[i][j]);
						staPoint.setY(i);
						if( (i+20) >= YMAX ){
							continue;
						}
						endPoint.setX(xPos[i+20][j+1]);
						endPoint.setY(i+20);
						painter.drawLine(staPoint,endPoint);
					}
					flag++;
				}
				else if( fillingType == 0 ){
					staPoint.setX(xPos[i][j]);
					staPoint.setY(i);
					endPoint.setX(xPos[i][j+1]);
					endPoint.setY(i);
					painter.drawLine(staPoint,endPoint);
				}
			}
		}

		//initial();
	
	}


}

void MyPainterWidget::mousePressEvent(QMouseEvent *e){

	//重新绘制后在鼠标左键点击后初始化
	if( ifFinished == true ){
		initial();
	}
	if(e->button()==Qt::LeftButton){

		pointArray[pointNum] = e->pos();
		pointNum++;

	}
	else if(e->button()==Qt::RightButton){

		ifFinished = true;

		//填充准备
		fillingPolygon();

		//填充
		repaint();


	}

	checkPointArray();
	
	//改变鼠标形状
    //setCursor(Qt::PointingHandCursor);

}

void MyPainterWidget::mouseMoveEvent(QMouseEvent *e){
	if( pointNum>0 && ifFinished==false ){

		pointArray[pointNum] = e->pos();
		pointNum++;
		checkPointArray();
		
		//绘制
		//update();
		repaint();


		//丢弃最后一个点
		pointNum--;

    }
}


void MyPainterWidget::fillingPolygon(){

	//构造边集
	buildEdgeTable();
	//扫描
	scanning();

}

void MyPainterWidget::buildEdgeTable(){

	bool* check = new bool[pointNum];

	for( int i = 0; i < pointNum; i++ ){
		YMAX = max(YMAX,pointArray[i].y());
		YMIN = min(YMIN,pointArray[i].y());
	}

	edgeNum = pointNum;
	for(int i = 0; i < edgeNum; i++ ){
		check[i] = false;
		edgeTable[i] = getEdge(pointArray[i],pointArray[(i+1)%edgeNum]);
	}
	for( int i = 0; i < pointNum; i++ ){
		int y1,y2,y3;
		int p1,p2,p3;
		p1 = i;
		p2 = (i+1)%pointNum;
		p3 = (i+2)%pointNum;
		y1 = pointArray[p1].y();
		y2 = pointArray[p2].y();
		y3 = pointArray[p3].y();
		//极大值
		if( y2>y1 && y2>y3 ){
			edgeTable[p1].yMax -= 1;
			edgeTable[p2].yMax -= 1;
		}
		//极小值
		else if( y2<y1 && y2<y3 ){
			edgeTable[p1].x += edgeTable[p1].dx;
			edgeTable[p2].x += edgeTable[p2].dx;
			int yMin = y2+1;
			edge* tmp = YPos[yMin];
			if( YPos[yMin] != NULL ){
				while( tmp->next!= NULL ){
					tmp = tmp->next;
				}
				if( check[p1] == false ){
					tmp->next = &edgeTable[p1];
					tmp = tmp->next;
					check[p1] = true;
				}
				if( check[p2] == false ){
					tmp->next = &edgeTable[p2];
					check[p2] = true;
				}
			}
			else{
				if( check[p1] == false ){
					YPos[yMin] = &edgeTable[p1];
					check[p1] = true;
				}
				if( check[p2] == false ){
					edgeTable[p1].next = &edgeTable[p2];
					check[p2] = true;
				}
			}
		}
		//自上到下
		else if( y1>y2 && y2>y3 ){
			//自上到下处理上边
			int yMin = y2;
			//单点处理
			edgeTable[p2].yMax--;
			if( YPos[yMin] != NULL ){
				edge* tmp = YPos[yMin];
				while( tmp->next!= NULL ){
					tmp = tmp->next;
				}
				if( check[p1] == false ){
					tmp->next = &edgeTable[p1];
					check[p1] = true;
				}
			}
			else{
				if( check[p1] == false ){
					YPos[yMin] = &edgeTable[p1];
					check[p1] = true;
				}
			}
		}
		//自下到上
		else if( y1<y2 && y2<y3 ){
			//自下到上处理上边
			int yMin = y2;
			//单点处理
			edgeTable[p1].yMax--;
			if( YPos[yMin] != NULL ){
				edge* tmp = YPos[yMin];
				while( tmp->next!= NULL ){
					tmp = tmp->next;
				}
				if( check[p2] == false ){
					tmp->next = &edgeTable[p2];
					check[p2] = true;
				}
			}
			else{
				if( check[p2] == false ){
					YPos[yMin] = &edgeTable[p2];
					check[p2] = true;
				}
			}
		}
		else if( y1==y2 ){
			int yMin = y1;
			if( y3 < y2 ){
				if( check[p2] == false ){
					int yMin = y3;
					if( YPos[yMin] != NULL ){
						edge* tmp = YPos[yMin];
						while( tmp->next!= NULL ){
							tmp = tmp->next;
						}
						tmp->next = &edgeTable[p2];
					}
					else{
						YPos[yMin] = &edgeTable[p2];
					}
					check[p2] = true;
				}

			}
			else if( y3 > y2 ){
				
				if( check[p2] == false ){
					int yMin = y2;
					if( YPos[yMin] != NULL ){
						edge* tmp = YPos[yMin];
						while( tmp->next!= NULL ){
							tmp = tmp->next;
						}
						tmp->next = &edgeTable[p2];
					}
					else{
						YPos[yMin] = &edgeTable[p2];
					}
					check[p2] = true;
				}

			}

		}
		//左勾
		else if( y2==y3 ){
			if( y1 > y2 ){
				
				if( check[p1] == false ){
					int yMin = y2;
					if( YPos[yMin] != NULL ){
						edge* tmp = YPos[yMin];
						while( tmp->next!= NULL ){
							tmp = tmp->next;
						}
						tmp->next = &edgeTable[p1];
					}
					else{
						YPos[yMin] = &edgeTable[p1];
					}
					check[p1] = true;
				}

			}
			else{

				if( check[p1] == false ){
					int yMin = y1;
					if( YPos[yMin] != NULL ){
						edge* tmp = YPos[yMin];
						while( tmp->next!= NULL ){
							tmp = tmp->next;
						}
						tmp->next = &edgeTable[p1];
					}
					else{
						YPos[yMin] = &edgeTable[p1];
					}
					check[p1] = true;
				}
			}
		}
	}
	delete []check;
}

edge MyPainterWidget::getEdge(QPoint staP,QPoint endP){

	if( staP.y() > endP.y() ){
		QPoint tmp;
		tmp = staP;
		staP = endP;
		endP = tmp;
	}
	edge tmp;
	if( (staP.x()-endP.x())==0 || (staP.y()-endP.y())==0 ){
		tmp.dx = 0;
	}
	else{
		tmp.dx = (staP.x()-endP.x())*1.0/(staP.y()-endP.y());
	}
	tmp.x = staP.x();
	tmp.yMax = endP.y();
	tmp.next = NULL;
	return tmp;

}

void MyPainterWidget::scanning(){

	for(int i = YMIN; i < YMAX+1; i++ ){
		
		edge* tmp;
		if( YPos[i] != NULL ){
			if( AEL == NULL ){
				AEL = YPos[i];
			}
			else{
				tmp = AEL;
				while( tmp->next != NULL ){
					tmp = tmp->next;
				}
				tmp->next = YPos[i];
			}
		}

		//若AEL不为空便取出坐标
		if( AEL!=NULL ){
			APNum = 0;
			APointY = i;
			tmp = AEL;
			xPos[i] = new int[maxEdgeNum];
			//取出所有横坐标
			while( tmp != NULL ){
				if( tmp->yMax >= i ){
					xPos[i][APNum++] = tmp->x;
				}
				tmp = tmp->next;
			}
			xLen[i] = APNum;
			//将所有横坐标排序
			bubbleSort(APNum,xPos[i]);
			
		
			//更新AEL
			edge* p1;
			edge* p2;
			while( AEL!=NULL && AEL->yMax <= i ){
				AEL = AEL->next;
			}
			if( AEL != NULL ){
				AEL->x += AEL->dx;
				p1 = AEL;
				p2 = AEL->next;
				while( p2 != NULL ){
					if( p2->yMax <= i ){
						p1->next = p2->next;
					}
					else{
						p2->x += p2->dx;
						p1 = p1->next;
					}
					p2 = p1->next;
				}
			}
		}
		else{
			xLen[i] = 0;
		}
	}

}

//检查点集大小
void MyPainterWidget::checkPointArray(){

	if( pointNum+1 < maxPointNum )
		return ;
	QPoint* tmp = new QPoint[2*maxPointNum];
	for( int i = 0; i < pointNum; i++ ){
		tmp[i] = pointArray[i];
	}
	delete []pointArray;
	pointArray = tmp;
	maxPointNum*=2;

}
//检查边集大小
void MyPainterWidget::checkEdgeTable(){

	if( edgeNum+1 < maxEdgeNum )
		return ;
	edge* tmp = new edge[2*maxEdgeNum];
	for( int i = 0; i < edgeNum; i++ ){
		tmp[i] = edgeTable[i];
	}
	delete []edgeTable;
	edgeTable = tmp;
	maxEdgeNum*=2;

}
//排序算法
void MyPainterWidget::bubbleSort(int len,int* xPos){

	for( int i = 0; i < len; i++ ){
		for( int j = i+1; j < len; j++ ){
			if( xPos[i]>xPos[j] ){
				swap(xPos[i],xPos[j]);
			}
		}
	}

}

//文件绘制多边形
void MyPainterWidget::fileFilling(){
	
	QString path = QFileDialog::getOpenFileName(this,
                                                tr("Open File"),
                                                "./file",
                                                tr("Text Files(*.txt)"));//"."表示程序运行目录
    if(!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Read File"),
                                 tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream in(&file);
		QString str = in.readAll();

		readPolygonFromFile(str);

        file.close();
    } else {
        QMessageBox::warning(this, tr("Path"),
                             tr("You did not select any file."));
    }

}

//保存多边形信息到文件
void MyPainterWidget::fileSave(){
	
	QString path = QFileDialog::getSaveFileName(this,
                                                tr("Open File"),
                                                "./file",
                                                tr("Text Files(*.txt)"));
    if(!path.isEmpty()) {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, tr("Write File"),
                                       tr("Cannot open file:\n%1").arg(path));
            return;
        }
        QTextStream out(&file);
		QString str;
        str = savePolygonToFile();
		out << str;
        file.close();
    } else {
        QMessageBox::warning(this, tr("Path"),
                             tr("You did not select any file."));
    }

}

//从文件中读取多边形信息
void MyPainterWidget::readPolygonFromFile(QString str){

	int pos = 0;
	//n
	pointNum = str.section('\n',pos,pos).toInt();
	pos++;
	//type
	fillingType = str.section('\n',pos,pos).toInt();
	pos++;
	//color
	int r = str.section('\n',pos,pos).toInt();
	pos++;
	int g = str.section('\n',pos,pos).toInt();
	pos++;
	int b = str.section('\n',pos,pos).toInt();
	pos++;
	qColor.setRed(r);
	qColor.setGreen(g);
	qColor.setBlue(b);

	QPoint qPoint;
	for( int i = 0; i < pointNum; i++ ){
		int x = str.section('\n',pos,pos).toInt();
		pos++;
		int y = str.section('\n',pos,pos).toInt();
		pos++;
		qPoint.setX(x);
		qPoint.setY(y);
		pointArray[i] = qPoint;
		checkPointArray();
	}
	//绘制
	ifFinished = true;
	fillingPolygon();
	repaint();
}

//读多边形信息到文件
QString MyPainterWidget::savePolygonToFile(){

	QString str = "";
	str += intToStr(pointNum)+'\n';
	str += intToStr(fillingType)+'\n';
	str += intToStr(qColor.red())+'\n';
	str += intToStr(qColor.green())+'\n';
	str += intToStr(qColor.blue())+'\n';
	for( int i = 0; i < pointNum; i++ ){
	
		str += intToStr(pointArray[i].x())+'\n';
		str += intToStr(pointArray[i].y())+'\n';
	
	}
	return str;

}

//int转str
QString MyPainterWidget::intToStr(int tmp){
	
	QString str = QString::number(tmp);
	return str;

}

//修改为完全填充模式
void MyPainterWidget::changeToFullFilling(){
	fillingType = 0;
}

//修改为阴影填充模式
void MyPainterWidget::changeToShadowFilling(){
	fillingType = 1;
}
