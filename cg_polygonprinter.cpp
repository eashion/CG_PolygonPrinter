#include "cg_polygonprinter.h"

CG_PolygonPrinter::CG_PolygonPrinter(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//Ĭ����ȫ���
	fillingType = 0;

	//��������
    QDesktopWidget* desktop = QApplication::desktop();
    int width = desktop->width();
    int height = desktop->height();
    move((width - this->width())/2, (height - this->height())/2);

	
	colorCombox = new ColorCombox(parent);
	myPainterWidget = new MyPainterWidget(this);
	setCentralWidget(myPainterWidget);

	//colorCombox->show();

	//�˵�������
	createActions(this);
	createMenus();

	connect(this->colorCombox,&ColorCombox::sigColorChanged,this->myPainterWidget,&MyPainterWidget::changeColor);

}

CG_PolygonPrinter::~CG_PolygonPrinter()
{

}


//���������캯��
void CG_PolygonPrinter::createActions(QWidget *parent){

	fullFillAction = new QAction(QIcon(""),QStringLiteral("&���"),this);
	fullFillAction->setStatusTip(QStringLiteral("��ȫ���"));
	connect(fullFillAction, &QAction::triggered,this->myPainterWidget,&MyPainterWidget::changeToFullFilling);
	
	shadowFillAction = new QAction(QIcon(""),QStringLiteral("&��Ӱ"),this);
	shadowFillAction->setStatusTip(QStringLiteral("��Ӱ���"));
	connect(shadowFillAction, &QAction::triggered,this->myPainterWidget,&MyPainterWidget::changeToShadowFilling);
	
	fileFillAction = new QAction(QIcon(""),QStringLiteral("&�ļ�����"),this);
	fileFillAction->setStatusTip(QStringLiteral("���ļ��ж���������Ϣ"));
	connect(fileFillAction, &QAction::triggered,this->myPainterWidget,&MyPainterWidget::fileFilling);
	
	fileSave = new QAction(QIcon(""),QStringLiteral("&�ļ�����"),this);
	fileSave->setStatusTip(QStringLiteral("���������Ϣ�������ļ�"));
	connect(fileSave, &QAction::triggered,this->myPainterWidget,&MyPainterWidget::fileSave);
	
	changeColor = new QAction(QIcon(""),QStringLiteral("&������ɫ"),this);
	changeColor->setStatusTip(QStringLiteral("��������λ�����ɫ"));
	connect(changeColor, &QAction::triggered,this,&CG_PolygonPrinter::showColorCombox);

}

void CG_PolygonPrinter::createMenus(){

	//����һ��nameΪ��ͼ�Ĳ˵���
	paintMenu = ui.menuBar->addMenu(QStringLiteral("&��ͼ"));
	paintMenu->addAction(fullFillAction);
	paintMenu->addAction(shadowFillAction);
	paintMenu->addAction(fileFillAction);

	//���һ���ָ���
	paintMenu->addSeparator();

	//����һ��nemeΪ����Ĳ˵���
	saveMenu = ui.menuBar->addMenu(QStringLiteral("&����"));
	saveMenu->addAction(fileSave);

	colorMenu = ui.menuBar->addMenu(QStringLiteral("&��ɫ"));
	colorMenu->addAction(changeColor);

}

//��ʾ��ɫ��������
void CG_PolygonPrinter::showColorCombox(){
	
	colorCombox->show();

}