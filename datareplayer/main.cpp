//#include "datareplayer.h"
#include <QtWidgets/QApplication>
#include "datatransfertool.h"
#include "logoverview.h"
#include "diff_match_patch.h"
#include "util.h"
#include <fstream>

int diff_test()
{
	//QApplication a(argc, argv);

	diff_match_patch dmp;
    ifstream f1("d:/temp/1.txt");
    ifstream f2("d:/temp/2.txt");

    std::string s1 = readAllFromStream(f1);

    std::string s2 = readAllFromStream(f2);

    QString str1 = QString::fromStdString(s1);//QString("First string in diff");
    QString str2 = QString::fromStdString(s2);//QString("string Test in diff");

    QList<Diff> res =  dmp.diff_main(str2,str1, true);
	dmp.diff_cleanupSemantic(res);
	qDebug()<<dmp.diff_toDelta(res);
    for(int i=0; i<res.size(); i++)
    {	
		
		//if(res[i].operation != Operation::EQUAL)
		//{
		//	qDebug()<<res[i].strOperation(res[i].operation) << res[i].text;
		//}
      
    }

	return 0;
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//diff_test();

	//DataReplayer w;
	//w.show();
	//DataTransferTool w;
	//w.show();
	LogOverview w;
	w.show();
	return a.exec();
}

