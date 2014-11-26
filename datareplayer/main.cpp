#include "datareplayer.h"
#include <QtWidgets/QApplication>
#include "datatransfertool.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//DataReplayer w;
	//w.show();
	DataTransferTool w;
	w.show();
	return a.exec();
}
