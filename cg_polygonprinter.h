#ifndef CG_POLYGONPRINTER_H
#define CG_POLYGONPRINTER_H
#include <QtWidgets/QMainWindow>
#include <qlayout.h>
#include "ui_cg_polygonprinter.h"
#include "ColorCombox.h"
#include "MyPainterWidget.h"
#include "QDesktopWidget.h"

class CG_PolygonPrinter : public QMainWindow
{
	Q_OBJECT

public:
	CG_PolygonPrinter(QWidget *parent = 0);
	~CG_PolygonPrinter();
	ColorCombox *colorCombox;						//色版
	MyPainterWidget *myPainterWidget;				//绘制窗口
	QGridLayout *qGridLayout;

    //标记填充样式
	int fillingType;

	//创建菜单栏类
	QMenu* paintMenu;
	QMenu* saveMenu;

	//创建paint的下拉菜单
	QAction* fullFillAction;
	QAction* shadowFillAction;
	QAction* fileFillAction;

	//创建save的下拉菜单
	QAction* fileSave;

	//构造函数
	void createMenus();
	void createActions(QWidget *parent = 0);



private:
	Ui::CG_PolygonPrinterClass ui;
};

#endif // CG_POLYGONPRINTER_H
