#include "datareplayer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DataReplayer w;
	w.show();
	return a.exec();
}
