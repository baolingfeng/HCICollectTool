#ifndef PLAYTHREAD_H
#define PLAYTHREAD_H

#include <QThread>
#include <vector>
#include "LogManager.h"

using namespace std;

class PlayThread : public QThread
{
	Q_OBJECT

public:
	PlayThread(QObject *parent);
	~PlayThread();

	void setTimes(LogManager& logMan);
	void stop();
	void run();
public slots:
	void setInterval(double i);

signals:
	void nextEvent(int);

private:
	vector<double> events_duration;
	int current_index;
	//double interval;
	bool is_stop;
};

#endif // PLAYTHREAD_H
