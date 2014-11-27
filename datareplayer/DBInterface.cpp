#include <Windows.h>
#include "DBInterface.h"
#include <QDebug>
#include <fstream>
#include "util.h"
#include <QString>
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
		//table: mouse_event_$name
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

		//table: mouse_event_screenshot_$name
		sql = "drop table if exists mouse_event_screenshot_" + name;
		stmt->execute(sql.c_str());
		sql = "CREATE TABLE `mouse_event_screenshot_" + name + "` ( \
			  `id` INT NOT NULL, \
			  `screenshot` MEDIUMBLOB NULL, \
			  PRIMARY KEY (`id`));";
		stmt->execute(sql.c_str());

		//table: mouse_event_action_$name
		sql = "drop table if exists mouse_event_action_" + name;
		stmt->execute(sql.c_str());
		sql = "CREATE TABLE `mouse_event_action_" + name + "` ( \
			  `id` INT NOT NULL, \
			  `action_name` MEDIUMTEXT NULL, \
			  `action_type` VARCHAR(45) NULL, \
			  `action_value` MEDIUMTEXT NULL, \
			  `bound_left` INT NULL, \
			  `bound_top` INT NULL, \
			  `bound_right` INT NULL, \
			  `bound_bottom` INT NULL, \
			  PRIMARY KEY (`id`));";
		stmt->execute(sql.c_str());

		//table: key_event_$name
		sql = "drop table if exists key_event_" + name;
		stmt->execute(sql.c_str());
		sql = "CREATE TABLE `key_event_" + name + "` ( \
			  `id` INT NOT NULL AUTO_INCREMENT, \
			  `timestamp` VARCHAR(30) NULL, \
			  `key_input` VARCHAR(10) NULL, \
			  `window_name` VARCHAR(300) NULL, \
			  `process_name` VARCHAR(100) NULL, \
			  `parent_window` VARCHAR(300) NULL, \
			  `pos_x` INT NULL, \
			  `pos_y` INT NULL, \
			  `has_screenshot` INT NULL, \
			  PRIMARY KEY (`id`));";
		stmt->execute(sql.c_str());

		//table: key_event_screenshot_$name
		sql = "drop table if exists key_event_screenshot_" + name;
		stmt->execute(sql.c_str());
		sql = "CREATE TABLE `key_event_screenshot_" + name + "` ( \
			  `id` INT NOT NULL, \
			  `screenshot` MEDIUMBLOB NULL, \
			  PRIMARY KEY (`id`));";
		stmt->execute(sql.c_str());

		//table: copy_event_$name
		sql = "drop table if exists copy_event_" + name;
		stmt->execute(sql.c_str());
		sql = "CREATE TABLE `copy_event_" + name + "` ( \
			  `id` INT NOT NULL AUTO_INCREMENT, \
			  `timestamp` VARCHAR(30) NULL, \
			  `window_name` VARCHAR(300) NULL, \
			  `process_name` VARCHAR(100) NULL, \
			  `parent_window` VARCHAR(300) NULL, \
			  `text` MEDIUMTEXT NULL, \
			  PRIMARY KEY (`id`));";
		stmt->execute(sql.c_str());

		delete stmt;

		return true;
	}
	catch(sql::SQLException &e) 
	{
		printException(e);
		return false;
	}
}

bool DBInterface::insertMouseEvent(string name, LogEvent e, string logDir)
{
	try
	{
		sql::PreparedStatement *prep_stmt;
		sql::Statement *stmt;
		sql::ResultSet *rs;

		string sql = "INSERT INTO mouse_event_" + name + "(timestamp, event_name, pos_x, pox_y, window_name, process_name, parent_window, \
			win_rect_left, win_rect_top, win_rect_right, win_rect_bottom, has_screenshot, has_acc) VALUES (?, ?,?,?, ?, ?,?,?, ?, ?,?,?, ?)";
		prep_stmt = conn->prepareStatement(sql.c_str());
		prep_stmt->setString(1, e.timestamp.c_str());
		prep_stmt->setString(2, e.name.c_str());
		prep_stmt->setInt(3, e.pt.x);
		prep_stmt->setInt(4, e.pt.y);
		//string encodeStr = QString::fromLocal8Bit(e.windowName.c_str()).toStdString();
		//qDebug()<<QString::fromLocal8Bit(e.windowName.c_str());

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

		sql = "select last_insert_id() as auto_id";
		stmt = conn->createStatement();
		rs = stmt->executeQuery(sql.c_str());
		int autoId = -1;
		if(rs->next())
		{
			autoId = rs->getInt(1);
		}

		if(e.isHasSrceenshot)
		{
			sql = "INSERT INTO mouse_event_screenshot_" + name + " (id, screenshot) VALUES (?, ?)";
			prep_stmt = conn->prepareStatement(sql.c_str());
			
			ifstream imgstream(logDir + "screen/" + e.timestamp + ".png", std::ios_base::binary);
			prep_stmt->setInt(1, autoId);
			prep_stmt->setBlob(2, &imgstream);
			prep_stmt->execute();
			delete prep_stmt;
		}


		if(e.isHasAcc)
		{
			sql = "INSERT INTO mouse_event_action_" + name + " (id, action_name, action_type, action_value, bound_left, bound_top, bound_right, bound_bottom) \
				VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
			prep_stmt = conn->prepareStatement(sql.c_str());
			prep_stmt->setInt(1, autoId);
			prep_stmt->setString(2, e.acc.name.c_str());
			prep_stmt->setString(3, e.acc.type.c_str());
			prep_stmt->setString(4, e.acc.value.c_str());
			prep_stmt->setInt(5, e.acc.bounding.left);
			prep_stmt->setInt(6, e.acc.bounding.top);
			prep_stmt->setInt(7, e.acc.bounding.right);
			prep_stmt->setInt(8, e.acc.bounding.bottom);
			prep_stmt->execute();
			delete prep_stmt;
		}

		delete rs;
		delete stmt;
	}
	catch(sql::SQLException &exception)
	{
		printException(exception);
		return false;
	}
}

bool DBInterface::insertKeyEvent(string name, LogEvent e, string logDir)
{
	try
	{
		sql::PreparedStatement *prep_stmt;
		sql::Statement *stmt;
		sql::ResultSet *rs;

		string sql = "INSERT INTO key_event_" + name + " (timestamp, key_input, window_name, process_name, parent_window, pos_x, pos_y, has_screenshot) \
													   VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
		prep_stmt = conn->prepareStatement(sql.c_str());
		prep_stmt->setString(1, e.timestamp.c_str());
		prep_stmt->setString(2, e.name.c_str());
		prep_stmt->setString(3, e.windowName.c_str());
		prep_stmt->setString(4, e.processName.c_str());
		prep_stmt->setString(5, e.parentWindowName.c_str());
		prep_stmt->setInt(6, e.pt.x);
		prep_stmt->setInt(7, e.pt.y);
		prep_stmt->setInt(8, e.isHasSrceenshot);
		prep_stmt->execute();

		delete prep_stmt;

		sql = "select last_insert_id() as auto_id";
		stmt = conn->createStatement();
		rs = stmt->executeQuery(sql.c_str());
		int autoId = -1;
		if(rs->next())
		{
			autoId = rs->getInt(1);
		}

		if(e.isHasSrceenshot)
		{
			sql = "INSERT INTO key_event_screenshot_" + name + " (id, screenshot) VALUES (?, ?)";
			prep_stmt = conn->prepareStatement(sql.c_str());
			
			ifstream imgstream(logDir + "screen/" + e.timestamp + ".png", std::ios_base::binary);
			prep_stmt->setInt(1, autoId);
			prep_stmt->setBlob(2, &imgstream);
			prep_stmt->execute();
			delete prep_stmt;
		}

		return true;
	}
	catch(sql::SQLException &exception)
	{
		printException(exception);
		return false;
	}
}

bool DBInterface::insertCopyEvent(string name, CopyEvent e, string logDir)
{
	try
	{
		sql::PreparedStatement *prep_stmt;
		
		string sql = "INSERT INTO copy_event_" + name + " (timestamp, window_name, process_name, parent_window, text) \
													   VALUES (?, ?, ?, ?, ?)";
		prep_stmt = conn->prepareStatement(sql.c_str());
		prep_stmt->setString(1, e.timestamp.c_str());
		prep_stmt->setString(2, e.windowName.c_str());
		prep_stmt->setString(3, e.processName.c_str());
		prep_stmt->setString(4, e.parentWindowName.c_str());
		prep_stmt->setString(5, e.text.c_str());

		prep_stmt->execute();

		delete prep_stmt;
	}
	catch(sql::SQLException &exception)
	{
		printException(exception);
		return false;
	}
	return true;
}

bool DBInterface::isExistsLog(string name)
{
	try
	{
		sql::PreparedStatement *prep_stmt;
		sql::ResultSet *rs;

		string sql = "select * from logs where log_name = ?";
		prep_stmt = conn->prepareStatement(sql.c_str());
		prep_stmt->setString(1, name.c_str());

		rs = prep_stmt->executeQuery();
		return rs->next();
	}
	catch(sql::SQLException &exception)
	{
		printException(exception);
		//return false;
	}

	return true;
}