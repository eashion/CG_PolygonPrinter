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
	ColorCombox *colorCombox;
	MyPainterWidget *myPainterWidget;
	QGridLayout *qGridLayout;

private:
	Ui::CG_PolygonPrinterClass ui;
};

#endif // CG_POLYGONPRINTER_H
