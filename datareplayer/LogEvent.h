#pragma once
#include<iostream>
#include<Windows.h>
using namespace std;

enum EventType {leftdown,leftup, rightdown,rightup, wheel, move, key, unknown};

struct AccElement
{
	string type;
	string name;
	string value;
	string parent_name;
	string parent_type;
	RECT bounding;
};

class LogEvent
{
public:
	LogEvent() {isHasAcc=false; isHasSrceenshot=false;}
	~LogEvent();
public:
	//int index;
	string timestamp;
	string name;
	string eventType;
	EventType type;
	
	POINT pt;
	string windowName;
	string processName;
	string parentWindowName;

	double duration;
	RECT winRect;
	bool isHasAcc;
	bool isHasSrceenshot;
	AccElement acc;
};

class CopyEvent
{
public:
	CopyEvent() {}
	~CopyEvent() {}
public:
	string timestamp;
	string windowName;
	string processName;
	string parentWindowName;
	string text;
};

class PasteEvent
{
public:
	PasteEvent() {}
	~PasteEvent() {}
public:
	string timestamp;
	string windowName;
	string processName;
	string parentWindowName;
	string method;
};

class EventProcess
{
public:
	EventProcess() {name="";}
	EventProcess(string name) : name(name){}
	~EventProcess(){}

public:
	string name;
	int from;
	int to;
};

class EventWindow
{
public:
	EventWindow() {name="";}
	EventWindow(string name) : name(name){}
	~EventWindow(){}

public:
	string name;
	int from;
	int to;
};