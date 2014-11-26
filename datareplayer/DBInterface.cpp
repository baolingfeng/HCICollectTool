#include <Windows.h>
#include "DBInterface.h"
#include <QDebug>

void printException(sql::SQLException &e)
{
	qDebug() << "# ERR: SQLException in " << __FILE__;
	qDebug() << "(" << __FUNCTION__ << ") on line " 
		<< __LINE__ << endl;
	qDebug() << "# ERR: " << e.what();
	qDebug() << " (MySQL error code: " << e.getErrorCode();
	qDebug() << ", SQLState: " << QString::fromStdString(e.getSQLState()) << " )" << endl;
}

DBInterface::DBInterface(void)
{
	try
	{
		sql::SQLString DB_HOST("tcp://127.0.0.1:3306");
		sql::SQLString DB_USER("root");
		sql::SQLString DB_PWD("123456");
		
		driver = get_driver_instance();
		//conn = driver->connect("tcp://127.0.0.1:3306", "root", "123456");
		conn = driver->connect(DB_HOST, DB_USER, DB_PWD);
		conn->setSchema("datacollect");
		
	}
	catch(sql::SQLException &e)
	{
		printException(e);
	}
	//conn->setSchema(DB_SCHEMA);
}


DBInterface::~DBInterface(void)
{
	if(conn != NULL)
	{
		delete conn;
	}
}



void DBInterface::close()
{
	conn->close();
	delete conn;
}

bool DBInterface::initTable(string name)
{
	try {
		sql::PreparedStatement *prep_stmt;

		prep_stmt = conn->prepareStatement("INSERT INTO logs(log_name, create_time) VALUES (?, now())");
		prep_stmt->setString(1, sql::SQLString(name.c_str()));
		prep_stmt->execute();
		
		delete prep_stmt;

		sql::Statement *stmt;
		stmt = conn->createStatement();
		stmt->execute(sql::SQLString(("drop table if exists mouse_event_" + name).c_str()));
		string sql = "CREATE TABLE `mouse_event_" + name + "` ( \
			`id` INT NOT NULL AUTO_INCREMENT, \
			`timestamp` VARCHAR(30) NOT NULL, \
			`event_name` VARCHAR(45) NOT NULL, \
			`pos_x` INT NULL, \
			`pox_y` INT NULL, \
			`window_name` VARCHAR(300) NULL, \
			`process_name` VARCHAR(100) NULL, \
			`parent_window` VARCHAR(300) NULL, \
			`win_rect_left` INT NULL, \
			`win_rect_top` INT NULL, \
			`win_rect_right` INT NULL, \
			`win_rect_bottom` INT NULL, \
			`has_screenshot` INT NULL, \
			`has_acc` INT NULL, \
			PRIMARY KEY (`id`));";

		stmt->execute(sql::SQLString(sql.c_str()));

		delete stmt;

		return true;
	}
	catch(sql::SQLException &e) 
	{
		printException(e);
		return false;
	}
}

bool DBInterface::insertMouseEvent(string name, LogEvent e)
{
	try
	{
		sql::PreparedStatement *prep_stmt;

		string sql = "INSERT INTO mouse_event_" + name + "(timestamp, event_name, pos_x, pox_y, window_name, process_name, parent_window, \
			win_rect_left, win_rect_top, win_rect_right, win_rect_bottom, has_screenshot, has_acc) VALUES (?, ?,?,?, ?, ?,?,?, ?, ?,?,?, ?)";
		prep_stmt = conn->prepareStatement(sql.c_str());
		prep_stmt->setString(1, e.timestamp.c_str());
		prep_stmt->setString(2, e.name.c_str());
		prep_stmt->setInt(3, e.pt.x);
		prep_stmt->setInt(4, e.pt.y);
		prep_stmt->setString(5, e.windowName.c_str());
		prep_stmt->setString(6, e.processName.c_str());
		prep_stmt->setString(7, e.parentWindowName.c_str());
		prep_stmt->setInt(8, e.winRect.left);
		prep_stmt->setInt(9, e.winRect.top);
		prep_stmt->setInt(10, e.winRect.right);
		prep_stmt->setInt(11, e.winRect.bottom);
		prep_stmt->setInt(12, e.isHasSrceenshot);
		prep_stmt->setInt(13, e.isHasAcc);

		prep_stmt->execute();

		delete prep_stmt;

	}
	catch(sql::SQLException &e)
	{
		printException(e);
		return false;
	}
}