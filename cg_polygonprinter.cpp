#include "cg_polygonprinter.h"

CG_PolygonPrinter::CG_PolygonPrinter(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//默认完全填充
	fillingType = 0;

	//居中设置
    QDesktopWidget* desktop = QApplication::desktop();
    int width = desktop->width();
    int height = desktop->height();
    move((width - this->width())/2, (height - this->height())/2);

	
	colorCombox = new ColorCombox(parent);
	myPainterWidget = new MyPainterWidget(this);
	setCentralWidget(myPainterWidget);

	//colorCombox->show();

	//菜单栏设置
	createActions(this);
	createMenus();

	connect(this->colorCombox,&ColorCombox::sigColorChanged,this->myPainterWidget,&MyPainterWidget::changeColor);

}

CG_PolygonPrinter::~CG_PolygonPrinter()
{

}


//工具栏构造函数
void CG_PolygonPrinter::createActions(QWidget *parent){

	fullFillAction = new QAction(QIcon(""),QStringLiteral("&填充"),this);
	fullFillAction->setStatusTip(QStringLiteral("完全填充"));
	connect(fullFillAction, &QAction::triggered,this->myPainterWidget,&MyPainterWidget::changeToFullFilling);
	
	shadowFillAction = new QAction(QIcon(""),QStringLiteral("&阴影"),this);
	shadowFillAction->setStatusTip(QStringLiteral("阴影填充"));
	connect(shadowFillAction, &QAction::triggered,this->myPainterWidget,&MyPainterWidget::changeToShadowFilling);
	
	fileFillAction = new QAction(QIcon(""),QStringLiteral("&文件读入"),this);
	fileFillAction->setStatusTip(QStringLiteral("从文件中读入多边形信息"));
	connect(fileFillAction, &QAction::triggered,this->myPainterWidget,&MyPainterWidget::fileFilling);
	
	fileSave = new QAction(QIcon(""),QStringLiteral("&文件保存"),this);
	fileSave->setStatusTip(QStringLiteral("将多边形信息保存至文件"));
	connect(fileSave, &QAction::triggered,this->myPainterWidget,&MyPainterWidget::fileSave);
	
	changeColor = new QAction(QIcon(""),QStringLiteral("&更换颜色"),this);
	changeColor->setStatusTip(QStringLiteral("更换多边形绘制颜色"));
	connect(changeColor, &QAction::triggered,this,&CG_PolygonPrinter::showColorCombox);

}

void CG_PolygonPrinter::createMenus(){

	//创建一个name为绘图的菜单栏
	paintMenu = ui.menuBar->addMenu(QStringLiteral("&绘图"));
	paintMenu->addAction(fullFillAction);
	paintMenu->addAction(shadowFillAction);
	paintMenu->addAction(fileFillAction);

	//添加一个分割器
	paintMenu->addSeparator();

	//创建一个neme为保存的菜单栏
	saveMenu = ui.menuBar->addMenu(QStringLiteral("&保存"));
	saveMenu->addAction(fileSave);

	colorMenu = ui.menuBar->addMenu(QStringLiteral("&颜色"));
	colorMenu->addAction(changeColor);

}

//显示颜色更换窗口
void CG_PolygonPrinter::showColorCombox(){
	
	colorCombox->show();

}