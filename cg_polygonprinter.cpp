#include "cg_polygonprinter.h"

CG_PolygonPrinter::CG_PolygonPrinter(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//¾ÓÖÐÉèÖÃ
    QDesktopWidget* desktop = QApplication::desktop();
    int width = desktop->width();
    int height = desktop->height();
    move((width - this->width())/2, (height - this->height())/2);

	
	colorCombox = new ColorCombox(parent);
	myPainterWidget = new MyPainterWidget(this);
	setCentralWidget(myPainterWidget);

}

CG_PolygonPrinter::~CG_PolygonPrinter()
{

}
