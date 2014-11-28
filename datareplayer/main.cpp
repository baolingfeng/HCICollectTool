#include "datareplayer.h"
#include <QtWidgets/QApplication>
#include "datatransfertool.h"
#include "logoverview.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//DataReplayer w;
	//w.show();
	//DataTransferTool w;
	//w.show();
	LogOverview w;
	w.show();
	return a.exec();
}
