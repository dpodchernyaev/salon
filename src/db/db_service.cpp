/* Copyright (c) 2015 Forward Systems. All rights reserved */

#include <QDebug>
#include <QThread>
#include <QTextCodec>

#include <config/config.h>

#include "db_service.h"

#define DB_DRIVER "QPSQL"

DBService* DBService::inst = NULL;

/** конструктор */
DBService::DBService()
{
	mainConn = NULL;
	dbThread = new QThread;
	dbThread->start();
}

/** @return единственный экземпляр класса */
DBService *DBService::getInstance()
{
	if (inst == NULL)
	{
		inst = new DBService;
	}
	return inst;
}

/** @return поток для выполнения запросов */
QThread* DBService::getThread() const
{
	return dbThread;
}

/** устанавливает и возвращает соединения с БД
 * @return соединение с БД */
DBConn *DBService::getConnection()
{
	QThread* thread = QThread::currentThread();
	QString name = QString("0x%1").arg((quintptr)thread,
							QT_POINTER_SIZE * 2, 16, QChar('0'));
	DBConn *res = NULL;
	res = connections.value(thread);


	if (res != NULL)
	{
		if (res->isConnected())
		{
			return res;
		}
		else
		{
			if (!res->connect())
			{
				qWarning() << "[[W]] " << "Failed connect to DB!" << name;
			}
		}
		return res;
	}

	if (mainConn == NULL)
	{
		Config* config = Config::getInstance();

		QString dbName = config->getValue(DB_NAME).toString();
		QString userName = config->getValue(DB_USER_NAME).toString();
		QString passwd = config->getValue(DB_PASSWD).toString();
		QString host = config->getValue(DB_HOST).toString();
		int port = config->getValue(DB_PORT).toInt();

		mainConn = new DBConn("SALON_CONN");
		mainConn->setDatabaseName(dbName);
		mainConn->setAccess(userName, passwd);
		mainConn->setHost(host);
		mainConn->setPort(port);
		mainConn->setDriver(DB_DRIVER);
		res = mainConn;
	}
	else
	{
		res = mainConn->clone(name);
	}

	connect(thread, SIGNAL(destroyed(QObject*)),
			this, SLOT(threadDestroyed(QObject*)));
	connections.insert(thread, res);

	if (!res->connect())
	{
		qWarning() << "[[W]] " << "Failed connect to DB!" << name;
	}

	return res;
}

/** Выполнить запрос
 * @param sql запрос
 * @param conn соединение с БД
 * @param isOk результат выполнения запроса
 * @return результат запроса */
QSqlQuery DBService::executeQuery(const QString &sql, DBConn* conn, bool* isOk)
{
	return executeQuery(sql, conn, QList<QVariant>(), isOk);
}

/** Выполнить запрос
 * @param sql запрос
 * @param conn соединение с БД
 * @param bindValues параметры запроса
 * @param isOk результат выполнения запроса
 * @return результат запроса */
QSqlQuery DBService::executeQuery(const QString &sql, DBConn* conn,
								  const QList<QVariant> &bindValues, bool* isOk)
{
	Q_ASSERT(conn != NULL);

	if (conn->isConnected() == false)
	{
		qCritical() << "[[C]] " << tr("Нет соединения БД при выполнении запроса"
						  " (DBService::executeQuery)");
		return QSqlQuery();
	}

	QSqlQuery res(conn->qtDatabase());
	res.prepare(sql);
	for (int i = 0; i < bindValues.size(); i++)
	{
		QVariant var = bindValues.at(i);
		if (var.type() == QVariant::String)
		{
			var = var.toString();
		}
		res.bindValue(i, var);
	}

	bool value = conn->executeQuery(res);
	if (isOk != NULL)
	{
		*isOk = value;
	}

	return res;
}

/** Выполнить запрос
 * @param sql запрос
 * @return результат запроса */
QSqlQuery DBService::executeQuery(const QString &sql)
{
	DBConn* conn = getConnection();
	QSqlQuery q = executeQuery(sql, conn);
	return q;
}

/** разрывает соединение с БД при условии удаления потока, в котором это
 * соединение было создано
 * @param obj удаляемый поток */
void DBService::threadDestroyed(QObject *obj)
{
	DBConn* conn = connections.take(obj);

	if (conn != NULL)
	{
		qDebug() << "[[D]] " << "close connection for thred";
		conn->disconnect();
		conn->deleteLater();
	}
}
