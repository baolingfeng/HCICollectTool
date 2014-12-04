#include "logdetailwidget.h"
#include <fstream>

LogDetailWidget::LogDetailWidget(QWidget *parent, QString name)
	: QWidget(parent), logMan(name.toStdString())
{
	ui.setupUi(this);

	this->name = name;
	this->setWindowTitle("Log Information - " + name);

	//db = new DBInterface;
	//vector<LogEvent> events = db->getAllEvents(name.toStdString());
	//logMan.setEvents(events);

	QStandardItemModel *model = new QStandardItemModel(8, 2, this);
	
	pieChart = new PieView(ui.pie_tab);
	pieLayout = new QVBoxLayout(ui.pie_tab);
	pieLayout->addWidget(pieChart);

	ui.include_proc_view->setModel(model);
	pieChart->setModel(model);

	QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
	pieChart->setSelectionModel(selectionModel);
	ui.include_proc_view->setSelectionModel(selectionModel);

	QHeaderView *headerView = ui.include_proc_view->horizontalHeader();
    headerView->setStretchLastSection(true);
	
	setupModel();
	
	markovLayout = new QVBoxLayout(ui.markov_tab); 
	generateMarkov();
	QLabel *label = new QLabel;
	QImage img(name + ".png");
	label->setPixmap(QPixmap::fromImage(img));
	markovLayout->addWidget(label);
	

	seqChart = new DistributedWidget;
	seqChart->initForProcess(logMan);
	seqLayout = new QVBoxLayout(ui.seq_tab);
	seqLayout->addWidget(seqChart);

	ui.aaView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.aaView , SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomMenuRequested(const QPoint &)));

	classifyAction();

	funcKeyInput();

	copyPaste();
}

LogDetailWidget::~LogDetailWidget()
{

}

void LogDetailWidget::setupModel()
{
	QAbstractItemModel *model = pieChart->model();

	model->setHeaderData(0, Qt::Horizontal, tr("Label"));
    model->setHeaderData(1, Qt::Horizontal, tr("Quantity"));

	QString colors[] = {"#ff0000", "#00ff00", "#0000ff","#ffff00", "#ff00ff", "#00ffff", "#000000", "#ffffff", "#777777", "#222222", "#aaaaaa"};

	int i=0;
	for(hash_map<string, double>::iterator it = logMan.process_stat.begin() ; it != logMan.process_stat.end(); it++)
	{
		string process = it->first;
		double duration = it->second;

		model->insertRow(i);
		model->setData(model->index(i, 0), QString::fromStdString(process));
		model->setData(model->index(i, 1), duration);

		model->setData(model->index(i, 0, QModelIndex()),
                           QColor(colors[i]), Qt::DecorationRole);
		i++;
	}

}

string clearProcessName(string proc_name)
{
	int index = proc_name.find(".");
	string name = proc_name;
	if(index >= 0)
	{
		name = proc_name.substr(0,index);
	}
	return name;
}

void LogDetailWidget::generateMarkov()
{
	hash_map<string, int> proc_out;
	hash_map<string, int> proc_trans;

	for(int i=0; i<logMan.processes.size()-1; i++)
	{
		EventProcess p1 = logMan.processes[i];
		EventProcess p2 = logMan.processes[i+1];

		string key = p1.name + "->" + p2.name;
		if(proc_trans.find(key) != proc_trans.end())
		{
			proc_trans[key] += 1;
		}
		else
		{
			proc_trans[key] = 1;
		}

		if(proc_out.find(p1.name) != proc_out.end())
		{
			proc_out[p1.name] += 1;
		}
		else
		{
			proc_out[p1.name] = 1;
		}
	}

	ofstream ogv("d:/temp/" + name.toStdString() + ".gv");

	stringstream strbuf;
	strbuf<< "digraph G { "  <<endl << "rankdir=LR; " <<endl; 
	strbuf<<" node [shape=circle]; ";
	for(set<string>::iterator it=logMan.process_set.begin(); it != logMan.process_set.end(); it++)
	{
		strbuf<<clearProcessName((*it))<<" ";
	}
	strbuf<<";"<<endl;

	for(hash_map<string, int>::iterator it = proc_trans.begin(); it != proc_trans.end(); it++)
	{
		string key = it->first;
		int count = it->second;
		int index = key.find("->");
		string outproc = key.substr(0, index);
		string toproc = key.substr(index+2);
		int total = proc_out[outproc];

		strbuf<<clearProcessName(outproc)<<" -> "<<clearProcessName(toproc)<<" [label=\""<<(double)count/total<<"\"];"<<endl;
	}

	strbuf<<"}";
	 
	ogv << strbuf.str();

	ogv.close();

	string cmd = "\"c:/Program Files (x86)/Graphviz2.38/bin/dot.exe\" -Tpng -o " + name.toStdString() + ".png d:/temp/" + name.toStdString() + ".gv";
	//int res = WinExec(cmd.c_str(), SW_HIDE);
	//int ret = system(cmd.c_str());

	//hide cmd window and wait the cmd 
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	wstring wcmd = from_string(cmd).c_str();
	if (CreateProcessW(NULL, (LPWSTR)wcmd.c_str(), NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

void LogDetailWidget::classifyAction()
{
	QAbstractItemModel *model = new QStandardItemModel(0, 6, this);
	ui.aaView->setModel(model);

	int k = 0;
	model->setHeaderData(k++, Qt::Horizontal, tr("Timestamp"));
	model->setHeaderData(k++, Qt::Horizontal, tr("Application"));
	model->setHeaderData(k++, Qt::Horizontal, tr("GUI Component"));
	model->setHeaderData(k++, Qt::Horizontal, tr("Control Name"));
	model->setHeaderData(k++, Qt::Horizontal, tr("Control Type"));
	model->setHeaderData(k++, Qt::Horizontal, tr("Control Value"));

	string eclipseStatus = "normal"; //normal edit; run or debug configuration; debug model; 
	int temp;
	int num = 0;
	for(int i=0; i<logMan.events.size(); i++)
	{
		LogEvent e = logMan.events[i];
		if(!e.isHasAcc) continue;

		model->insertRow(num);

		k = 0;
		model->setData(model->index(num,k++), QString::fromStdString(e.timestamp));
		model->setData(model->index(num,k++), QString::fromStdString(e.processName));
		
		if(e.acc.type == "menu item" || e.acc.type == "tab item" 
			|| e.acc.parent_name == "Desktop" || e.acc.parent_name == "%trimmedwindow.label.eclipseSDK")
		{
			model->setData(model->index(num,k++), QString::fromStdString(e.acc.name));
		}
		else
		{
			model->setData(model->index(num,k++), QString::fromStdString(e.acc.parent_name));
		}
		
		model->setData(model->index(num,k++), QString::fromStdString(e.acc.name));
		model->setData(model->index(num,k++), QString::fromStdString(e.acc.type));
		model->setData(model->index(num,k++), QString::fromStdString(e.acc.value));
		num++;

	}
}

void LogDetailWidget::funcKeyInput()
{
	QAbstractItemModel *model = new QStandardItemModel(0, 4, this);
	ui.funcKeyView->setModel(model);

	int k = 0;
	model->setHeaderData(k++, Qt::Horizontal, tr("Timestamp"));
	model->setHeaderData(k++, Qt::Horizontal, tr("Keys"));
	model->setHeaderData(k++, Qt::Horizontal, tr("Window Name"));
	model->setHeaderData(k++, Qt::Horizontal, tr("Process Name"));

	for(int i=0 ;i<logMan.keyEvents.size(); i++)
	{
		string keystr = logMan.keysToString(logMan.keyEvents[i]);

		model->insertRow(i);
		k=0;
		model->setData(model->index(i,k++), QString::fromStdString(logMan.keyEvents[i][0].timestamp));
		model->setData(model->index(i,k++), QString::fromStdString(keystr));
		model->setData(model->index(i,k++), QString::fromStdString(logMan.keyEvents[i][0].windowName));
		model->setData(model->index(i,k++), QString::fromStdString(logMan.keyEvents[i][0].processName));
	}
}

void LogDetailWidget::copyPaste()
{
	QAbstractItemModel *model = new QStandardItemModel(0, 7, this);
	ui.cpView->setModel(model);

	int k = 0;
	model->setHeaderData(k++, Qt::Horizontal, tr("Timestamp"));
	model->setHeaderData(k++, Qt::Horizontal, tr("Copy"));
	model->setHeaderData(k++, Qt::Horizontal, tr("Window Name"));
	model->setHeaderData(k++, Qt::Horizontal, tr("Process Name"));
	model->setHeaderData(k++, Qt::Horizontal, tr("Paste"));
	model->setHeaderData(k++, Qt::Horizontal, tr("Window Name"));
	model->setHeaderData(k++, Qt::Horizontal, tr("Process Name"));

	int i, j;
	i = j = 0;
	int num = 0;
	while(i<logMan.copy_events.size() || j < logMan.paste_events.size())
	{
		string type = "";
		string timestamp = "";
		string copyText = "";
		string cwindow = "";
		string cprocess = "";
		string pasteText = "";
		string pwindow = "";
		string pprocess = "";
		if(i>=logMan.copy_events.size())
		{
			timestamp = logMan.paste_events[j].timestamp;
			pasteText = logMan.paste_events[j].method;
			pwindow = logMan.paste_events[j].windowName;
			pprocess= logMan.paste_events[j].processName;
			j++;
		}
		else if(j>=logMan.paste_events.size())
		{
			timestamp = logMan.copy_events[i].timestamp;
			copyText = logMan.copy_events[i].text;
			cwindow = logMan.copy_events[i].windowName;
			cprocess= logMan.copy_events[i].processName;
			i++;
		}
		else
		{
			string t1 = logMan.copy_events[i].timestamp;
			string t2 = logMan.paste_events[j].timestamp;
			double interval = GetTimeDifference(toSystemTime(t1), toSystemTime(t2));
			if(interval<0)
			{
				timestamp = logMan.paste_events[j].timestamp;
				pasteText = logMan.paste_events[j].method;
				pwindow = logMan.paste_events[j].windowName;
				pprocess= logMan.paste_events[j].processName;
				j++;	
			}
			else
			{
				timestamp = logMan.copy_events[i].timestamp;
				copyText = logMan.copy_events[i].text;
				cwindow = logMan.copy_events[i].windowName;
				cprocess= logMan.copy_events[i].processName;
				i++;
			}
		}
		k=0;
		model->insertRow(num);
		model->setData(model->index(num,k++), QString::fromStdString(timestamp));
		model->setData(model->index(num,k++), QString::fromStdString(copyText));
		model->setData(model->index(num,k++), QString::fromStdString(cwindow));
		model->setData(model->index(num,k++), QString::fromStdString(cprocess));
		model->setData(model->index(num,k++), QString::fromStdString(pasteText));
		model->setData(model->index(num,k++), QString::fromStdString(pwindow));
		model->setData(model->index(num,k++), QString::fromStdString(pprocess));
		num++;
	}
}

void LogDetailWidget::onCustomMenuRequested(const QPoint &pos)
{
	QModelIndex index = ui.aaView->currentIndex();
	if(index.row() <0) return;

	QAbstractItemModel* model = ui.aaView->model();
	QMenu *menu=new QMenu(this);

	QString timestamp = model->data(model->index(index.row(), 0), Qt::DisplayRole).toString();
	QString compName = model->data(model->index(index.row(), 2), Qt::DisplayRole).toString();
	QString type = model->data(model->index(index.row(), 4), Qt::DisplayRole).toString();
	QString value = model->data(model->index(index.row(), 5), Qt::DisplayRole).toString();
	int i;
	if((i=compName.indexOf(".java"))>=0 && type == "edit" && value != "")
	{
		QAction *editHistory = new QAction("See Edit History", this);
		menu->addAction(editHistory);
		connect(editHistory, SIGNAL(triggered()), this, SLOT(showEditHistory()));
	}

	QAction *showact = new QAction("Show Screenshot", this);
	
	menu->addAction(showact);
    
    menu->popup(ui.aaView->viewport()->mapToGlobal(pos));

	connect(showact, SIGNAL(triggered()), this, SLOT(showScreenshot()));

}

void LogDetailWidget::showScreenshot()
{
	QModelIndex index = ui.aaView->currentIndex();
	QAbstractItemModel* model = ui.aaView->model();

	QString timestamp = model->data(model->index(index.row(), 0), Qt::DisplayRole).toString();

	cv::Mat screenshot = logMan.getScreenshot(timestamp.toStdString(), "mouse");
	LogEvent e = logMan.getMouseEvent(timestamp.toStdString());

	cv::Mat screen2 = addImageOnFront(cv::Point(e.pt.x, e.pt.y), screenshot, MOUSEICON, MOUSEMASK);

	cv::rectangle(screen2, fromWindowRECT(e.acc.bounding), cv::Scalar(0,0,255), 2);

	cv::imshow("Click Action", screen2);
	cv::waitKey(0);
}

void LogDetailWidget::showEditHistory()
{
	QModelIndex index = ui.aaView->currentIndex();
	if(index.row() <0) return;

	QAbstractItemModel* model = ui.aaView->model();

	QString timestamp = model->data(model->index(index.row(), 0), Qt::DisplayRole).toString();
	QString compName = model->data(model->index(index.row(), 2), Qt::DisplayRole).toString();
	QString type = model->data(model->index(index.row(), 4), Qt::DisplayRole).toString();
	QString value = model->data(model->index(index.row(), 5), Qt::DisplayRole).toString();

	QWidget *histWidget = new QWidget;
	histWidget->resize(QSize(800,600));
	QVBoxLayout *boxlayout = new QVBoxLayout;
	QScrollArea* area = new QScrollArea;
	QGridLayout *gridLayout = new QGridLayout;
	QLabel* fileLabel = new QLabel;
	fileLabel->setText("Source Code File: " + compName);
	gridLayout->addWidget(fileLabel, 0, 0, 1, -1);
	histWidget->setLayout(boxlayout);
	boxlayout->addWidget(area);
	area->setLayout(gridLayout);
	
	LogEvent pre_e;
	bool isFirst = true;
	int row = 1;
	for(int i=0; i<logMan.events.size(); i++)
	{
		LogEvent e = logMan.events[i];
		if(!e.isHasAcc) continue;
		
		if(e.acc.parent_name == compName.toStdString() && e.acc.type == type.toStdString())
		{
			if(isFirst)
			{
				QLabel* timeLabel = new QLabel;
				timeLabel->setText(QString::fromStdString(e.timestamp));
				QLabel* edit = new QLabel;
				edit->setText("First Time Open");
				gridLayout->addWidget(timeLabel, row, 0, 1, 1);
				gridLayout->addWidget(edit, row, 1, 1, 1);
				isFirst = false;
			}
			else
			{
				QLabel* timeLabel = new QLabel;
				timeLabel->setText(QString::fromStdString(e.timestamp));
				gridLayout->addWidget(timeLabel, row, 0, 1, 1);

				string s1 = pre_e.acc.value;
				replaceAll(s1, "\\n", "\n");
				replaceAll(s1, "\\t", "\t");

				string s2 = e.acc.value;
				replaceAll(s2, "\\n", "\n");
				replaceAll(s2, "\\t", "\t");

				QString str1 = QString::fromStdString(s1);
				QString str2 = QString::fromStdString(s2);

				diff_match_patch dmp;
				QList<Diff> res =  dmp.diff_main(str1,str2, true);
				dmp.diff_cleanupSemantic(res);
				if(res.size() == 1 && res[0].operation == Operation::EQUAL)
				{
					QLabel* edit = new QLabel;
					edit->setText("No Difference");
					gridLayout->addWidget(edit, row, 1, 1, 1);
				}
				else
				{
					QTextEdit* edit = new QTextEdit;
					edit->setText(dmp.diff_toString(res));
					gridLayout->addWidget(edit, row, 1, 1, 1);
				}			
				
			}

			pre_e = e;
			row++;
		}
	}

	histWidget->show();
}