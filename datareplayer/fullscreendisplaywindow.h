#ifndef FULLSCREENDISPLAYWINDOW_H
#define FULLSCREENDISPLAYWINDOW_H

#include <QWidget>
#include "qlabel.h"
#include <qevent.h>
#include "ui_FullScreenDisplayWindow.h"
#include "LogManager.h"
#include "LogReader.h"
#include "playthread.h"
#include "qslider.h"
#include "qpushbutton.h"
#include "qspinbox.h"
#include "displayimage.h"
#include "ui_playbar.h"

class FullScreenDisplayWindow : public QWidget
{
	Q_OBJECT

public:
	FullScreenDisplayWindow(QWidget *parent=0);
	~FullScreenDisplayWindow();

	void init(LogManager& logMan, LogReader& logReader);
	void setImage();
	void setImage(QImage img);
	//void setImage(LogEvent e);

public slots:
	void play();
	void changeImage();
signals:
	

protected:
	void keyPressEvent(QKeyEvent *k);
	
private:

	int seq;
	int index;
	double time;
	LogManager logMan;
	LogReader logReader;

	QTimer* timer;

	QPushButton* playbtn;
	QSpinBox* spinbox;
	QPushButton* gobtn;
	QLabel* timelabel;
	//QPushButton* pausebtn;
	QLabel* imageLabel;
	QSlider* slider;

	Ui::fullscreenwidget ui;
	Ui::playbar barui;
};

#endif // FULLSCREENDISPLAYWINDOW_H
