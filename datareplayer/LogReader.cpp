#include "LogReader.h"
#include <QDebug>

LogReader::LogReader(string folder, bool isKeepUnknown) : logDir(folder), isKeepUnknown(isKeepUnknown)
{
}


LogReader::~LogReader(void)
{
}

EventType getEventType(string name)
{
	if(name == "MOUSE LEFT DOWN")
	{
		return EventType::leftdown;
	}
	else if(name == "MOUSE LEFT UP")
	{
		return EventType::leftup;
	}
	else if(name == "MOUSE RIGHT DOWN")
	{
		return EventType::rightdown;
	}
	else if(name == "MOUSE RIGHT UP")
	{
		return EventType::rightup;
	}
	else if(name == "MOUSE MOVE")
	{
		return EventType::move;
	}
	else if(name == "MOUSE WHEEL")
	{
		return EventType::wheel;
	}
	else
	{
		return EventType::unknown;
	}
}

bool LogReader::getOneMouseEvent(ifstream& mouseFile, LogEvent& logEvent)
{
	if(!mouseFile.is_open() || mouseFile.eof()) return false;

	//LogEvent logEvent;

	string line;

	getline(mouseFile,line);
	trim(line);
	if(line == "") return false;
	logEvent.timestamp = line;

	qDebug()<<QString::fromStdString(line)<<endl;

	getline(mouseFile,line);
	
	if(line == "NOT LOGGED APPLICATION")
	{
		logEvent.name = "unknown";
		logEvent.type = EventType::unknown;
		logEvent.processName = "unknown";
		logEvent.windowName = "unknown";
		return true;
	}

	vector<string> sarray = split(line," ");
	int len = sarray.size();
	logEvent.pt.x = stringToNumber<int>(sarray[len-2]);
	logEvent.pt.y = stringToNumber<int>(sarray[len-1]);
	logEvent.name = join(subvector(sarray, 0, len-2)," ");
	logEvent.type = getEventType(logEvent.name);

	getline(mouseFile,line);
	string::size_type pos1 = line.find_first_of(",");
	//r.winHwnd = line.substr(0,pos1);
	string::size_type pos2 = line.find_last_of("(");
	string::size_type pos3 = line.find_last_of(")");
	string s = line.substr(pos2+1,pos3-pos2-1);
	sarray = split(s," ");

	logEvent.winRect.left = stringToNumber<int>(sarray[0]);
	logEvent.winRect.top = stringToNumber<int>(sarray[1]);
	logEvent.winRect.right = stringToNumber<int>(sarray[2]);
	logEvent.winRect.bottom = stringToNumber<int>(sarray[3]);
	
	logEvent.windowName = line.substr(pos1+1,pos2-pos1-1);
	trim(logEvent.windowName);

	getline(mouseFile,line);
	string::size_type pos = line.find_last_of(" ");
	logEvent.processName = line.substr(0,pos);

	getline(mouseFile,line);
	logEvent.parentWindowName = line;

	if(logEvent.type == EventType::leftdown)
	{
		AccElement e;
		if(getAccElement(logEvent.timestamp, e))
		{
			logEvent.acc = e;
			logEvent.isHasAcc = true;
		}
	}

	if(logEvent.type != EventType::move && isScreenshotExists(logEvent.timestamp))
	{
		logEvent.isHasSrceenshot = true;
	}

	return true;
}

bool LogReader::getAccElement(string timestamp, AccElement& e)
{
	string filename = logDir + "detail/" + timestamp + ".txt";
	ifstream uifile(filename);
	//uifile.imbue(std::locale());
	string line;

	if(uifile.is_open())
	{
		try
		{
			getline(uifile,line); //1st line: time info
			getline(uifile,line); //2nd line: runtime 

			getline(uifile,line); //3rd line: ui type
			e.type = line.substr(6);
		
			getline(uifile,line); //4th line: ui name
			e.name = line.substr(6);

			getline(uifile,line); //5th line: bounding
			line = line.substr(10);
			vector<string> tmpv = split(line," ");
			e.bounding.left = stringToNumber<int>(tmpv[0]);
			e.bounding.top = stringToNumber<int>(tmpv[1]);
			e.bounding.right = stringToNumber<int>(tmpv[2]);
			e.bounding.bottom = stringToNumber<int>(tmpv[3]);

			getline(uifile,line); //6th line: ui value
			e.value = line.substr(6);
		}
		catch(exception e)
		{
			cout<<line<<", error: "<<e.what()<<endl;
		}
		uifile.close();
		return true;
	}
	
	return false;
}
	LogEvent getOneKeyEvent(ifstream& keyFile);

bool LogReader::isScreenshotExists(string timestamp)
{
	string filename = logDir + "screen/" + timestamp + ".png";
	ifstream fimg(filename);
	return fimg.is_open();
}

bool LogReader::getOneKeyEvent(ifstream& keyFile, LogEvent& logEvent)
{
	if(!keyFile.is_open() || keyFile.eof()) return false;

	string line;
	//LogEvent logEvent;
	
	getline(keyFile,line);
	trim(line);
	if(line == "") return false;

	logEvent.timestamp = line;

	getline(keyFile,line);
	if(line == "NOT LOGGED APPLICATION")
	{
		logEvent.name = "unknown";
		logEvent.type = EventType::unknown;
		return true;
	}
	
	logEvent.name = line;
	logEvent.type = EventType::key;

	getline(keyFile,line);
	vector<string> sarray = split(line," ");
	int len = sarray.size();
	logEvent.pt.x = stringToNumber<int>(sarray[len-2]);
	logEvent.pt.y = stringToNumber<int>(sarray[len-1]);

	getline(keyFile,line);
	logEvent.windowName = line;

	//process name
	getline(keyFile,line);
	logEvent.processName = line;

	//parent window
	getline(keyFile,line);
	logEvent.parentWindowName = line;

	if(logEvent.type != EventType::move && isScreenshotExists(logEvent.timestamp))
	{
		logEvent.isHasSrceenshot = true;
	}

	return true;
}

vector<LogEvent> LogReader::read(bool isKeepUnknown)
{
	ifstream mouseFile(logDir + "mouse.txt");
	ifstream keyFile(logDir + "keyboard.txt");

	vector<LogEvent> events;
	
	bool temp = keyFile.is_open();

	string mline, kline;
	while(true)
	{
		LogEvent mevent;
		LogEvent kevent;

		bool mflag = getOneMouseEvent(mouseFile, mevent);
		bool kflag = getOneKeyEvent(keyFile, kevent);

		if(!mflag && !kflag) //both log file is finished or error happen
		{
			break;
		}
		else if(!mflag && kflag)
		{
			events.push_back(kevent);
		}
		else if(mflag && !kflag)
		{
			events.push_back(mevent);
		}
		else
		{
			while(true)
			{
				if(!mflag || !kflag)
				{
					break;
				}

				double interval = GetTimeDifference(toSystemTime(mevent.timestamp),toSystemTime(kevent.timestamp));
				if(interval>0)
				{
					events.push_back(mevent);
					mevent = LogEvent();
					mflag = getOneMouseEvent(mouseFile, mevent);
				}
				else
				{
					events.push_back(kevent);
					kevent = LogEvent();
					kflag = getOneKeyEvent(keyFile, kevent);
				}
			}
			
		}
	}

	return events;
}

vector<LogEvent> LogReader::readMouseEvents()
{
	ifstream mouseFile(logDir + "mouse.txt");

	vector<LogEvent> events;
	while(true)
	{
		LogEvent mevent;
		if(!getOneMouseEvent(mouseFile, mevent)) break;

		events.push_back(mevent);
	}
	return events;
}

vector<LogEvent> LogReader::readKeyInputEvents()
{
	ifstream keyFile(logDir + "keyboard.txt");

	vector<LogEvent> events;
	while(true)
	{
		LogEvent kevent;
		if(!getOneKeyEvent(keyFile, kevent)) break;

		events.push_back(kevent);
	}
	return events;
}

vector<CopyEvent> LogReader::readCopyEvents()
{
	ifstream cpfile(logDir + "copypaste.txt");

	vector<CopyEvent> events;
	while(cpfile.is_open() && !cpfile.eof())
	{
		string line;
		getline(cpfile, line);

		CopyEvent e;
		e.timestamp = line;
		
		getline(cpfile, line);
		e.windowName = line;

		getline(cpfile, line);
		e.processName = line;

		getline(cpfile, line);
		e.parentWindowName = line;

		getline(cpfile, line);
		e.text = line;

		getline(cpfile, line);
		events.push_back(e);
	}

	cpfile.close();
	return events;
}

cv::Mat LogReader::readScreenshotByCV(string timestamp)
{
	string path = logDir + "screen/" + timestamp + ".png";
	return cv::imread(path);
}

QImage LogReader::readScreenshotByQT(string timestamp)
{
	string path = logDir + "screen/" + timestamp + ".png";
	QImage qimg(path.c_str());
	return qimg;
}