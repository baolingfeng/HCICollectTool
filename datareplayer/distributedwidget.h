#ifndef DISTRIBUTEDLABEL_H
#define DISTRIBUTEDLABEL_H

#include <QLabel>
#include <QPaintEvent>
#include <set>
#include <vector>
#include <hash_map>
#include "LogManager.h"

using namespace std;

class DistributedItem
{
public:
	DistributedItem(string name, double value) 
	{
		this->name = name; 
		this->value = value;
	}

	~DistributedItem() {}

	string getName() {return name;}
	double getValue() {return value;}
	void setName(string name) {this->name = name;}
	void setValue(double value){this->value = value;}

private:
	string name;
	double value;
};

class DistributedWidget : public QWidget
{
	Q_OBJECT

public:
	DistributedWidget(QWidget *parent);
	~DistributedWidget();

	void initForProcess(LogManager& logMan);

	void paintEvent(QPaintEvent* event);

	static QColor USE_COLORS[];
private:
	bool isInit;
	set<string> category;
	vector<DistributedItem> items;
	hash_map<string, QColor> color_map;
};

#endif // DISTRIBUTEDLABEL_H
