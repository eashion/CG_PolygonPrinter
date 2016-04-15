#include "cg_polygonprinter.h"
#include <QtWidgets/QApplication>
#include "MyPainterWidget.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CG_PolygonPrinter w;
	w.show();
	return a.exec();
}
