/* Copyright (c) 2015 Forward Systems. All rights reserved */

#ifndef DB_SERVICE_H_INCLUDED
#define DB_SERVICE_H_INCLUDED

#include <db/dbconnection.h>

#include <QObject>
#include <QString>

/** @class DBService
 * @brief Предназначен для закрытия и удаления соединений с БД при удалении
 * потока */
class DBService : public QObject
{
	Q_OBJECT
public:
	static DBService* getInstance();
	DBConn* getConnection();
	QSqlQuery executeQuery(const QString &sql);
	/* ВАЖНО. Если в параметрах запроса используются русские символы,
	 * то этот параметр необходибо передавать их через bindValues, иначе запрос
	 * не сработает */
	QSqlQuery executeQuery(const QString &sql, DBConn* conn,
						   const QList<QVariant> &bindValues = QList<QVariant>(),
						   bool* isOk = NULL);
	QSqlQuery executeQuery(const QString &sql, DBConn* conn, bool* isOk);
	QThread* getThread() const;

private:
	DBService();

private Q_SLOTS:
	void threadDestroyed(QObject* obj);

private:
	DBConn* mainConn;
	QThread* dbThread; ///< поток для выполнения запросов
	static DBService* inst; ///< единственный экземпляр класса
	QMap<QObject*, DBConn*> connections; ///< все соединения с БД
};
#endif
