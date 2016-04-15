#include "mypainterwidget.h"


MyPainterWidget::MyPainterWidget(QWidget* parent)
     :QWidget(parent){
	
	YMAX= 0;
	edgeNum = 0;
	pointNum = 0;
	APNum = 0;
	APointY = 0;
	maxPointNum = 100;
	maxEdgeNum = 100;
	qColor = Qt::red;
	ifFinished = false;
	ifFilling = false;
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
}


void MyPainterWidget::initial(){
	
	
	YMAX= 0;
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

	//封闭
	if( ifFinished == true ){

		painter.drawLine(pointArray[pointNum-1],pointArray[0]);

		for( int i = 0; i < APointY; i++ ){
			for( int j = 0; j < xLen[i]; j+=2 ){
				staPoint.setX(xPos[i][j]);
				staPoint.setY(i);
				endPoint.setX(xPos[i][j+1]);
				endPoint.setY(i);
				painter.drawLine(staPoint,endPoint);
			}
		}

		initial();
	
	}


}

void MyPainterWidget::mousePressEvent(QMouseEvent *e){

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


		//结束绘制
		pointNum = 0;

	}

	checkPointArray();
	
	//改变鼠标形状
    //setCursor(Qt::PointingHandCursor);


}

void MyPainterWidget::mouseMoveEvent(QMouseEvent *e){
	if( this->pointNum > 0 ){

		pointArray[pointNum] = e->pos();
		pointNum++;
		
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
			if( YPos[yMin] != NULL ){
				edge* tmp = YPos[yMin];
				while( tmp->next!= NULL ){
					tmp = tmp->next;
				}
				if( check[p1] == false ){
					tmp->next = &edgeTable[p1];
					check[p1] = true;
				}
				if( check[p2] == false ){
					edgeTable[p1].next = &edgeTable[p2];
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

	for(int i = 0; i < YMAX+1; i++ ){
		
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

		//若AEL不为空便绘制
		if( AEL!=NULL ){
			APNum = 0;
			APointY = i;
			tmp = AEL;
			xPos[i] = new int[maxEdgeNum];
			//取出所有横坐标
			while( tmp != NULL ){
				xPos[i][APNum++] = tmp->x;
				tmp = tmp->next;
			}
			xLen[i] = APNum;
			//将所有横坐标排序
			bubbleSort(APNum,xPos[i]);
			
		
			//更新AEL
			edge* p1;
			edge* p2;
			while( AEL!=NULL && AEL->yMax == i ){
				AEL = AEL->next;
			}
			if( AEL != NULL ){
				AEL->x += AEL->dx;
				p1 = AEL;
				p2 = AEL->next;
				while( p2 != NULL ){
					if( p2->yMax == i ){
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

void MyPainterWidget::bubbleSort(int len,int* xPos){

	for( int i = 0; i < len; i++ ){
		for( int j = i+1; j < len; j++ ){
			if( xPos[i]>xPos[j] ){
				swap(xPos[i],xPos[j]);
			}
		}
	}

}