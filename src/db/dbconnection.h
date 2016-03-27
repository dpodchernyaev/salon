/** @addtogroup DB */
/*@{*/
#ifndef DBConn_H
#define DBConn_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QStringList>
#include <QVariantList>
#include <QHash>
#include <QMutex>
#include <QScopedPointer>

#define TO_DB_BOOL(val) ((short) (val))

/** @class DBConn
	@brief Cоединение с базой данных.

	Соединения с базами данных создаются при запуске приложения
	и доступны из фасада соединений через вызов FacadeDB::getConnection().

	@note По причине того, что Qt не поддерживает использование
	соединения с базой данных из потока, отличного от создавшего
	это соединение, все потенциально опасные обращения к сущностям QtSql
	защищены мьютексом. Конкурентный доступ к одному соединению возможен,
	но будет выполнен в порядке очереди накопившихся запросов.

	Для корректного использования соединений в потоках, каждый поток должен
	создавать свою копию соединения с помощью вызова DBConn::clone() на
	экземпляре, полученном из фасада:
	@code
	void MyThread::run()
	{
		DBConn *lockedDb = FacadeDB::instance()->getConnection(SPIDER_ODB);
		DBConn *db = lockedDb->clone("UniqueName");
		db->connect();
		//...
		db->disconnect();
		delete db;
	}
	@endcode

	При выполнении запросов с параметрами рекомендуется пользоваться
	переменными sql, и связывать их с помощью конструкций
	QSqlQuery::bindValue():
	@code
	void MyController::selectForId(const ID &id)
	{
		DBConn *db = getConnection();
		db->beginTransaction();

		QString sql =
				"SELECT * FROM table_name "
				"WHERE to_char(id) = :seed_id";

		QSqlQuery query(db->qtDatabase());
		query.prepare(sql);
		query.bindValue(0, toString(Id));
		if (db->executeQuery(query))
		{
			//...
		}
		else
		{
			qWarning() << "Failed to fetch data for id" << toString(id);
		}

		db->commit();
	}
	@endcode

	@note В случаях выборки кортежей с полями численных типов, потенциально
	непредставимых 32-битными числами, следует явно преобразовывать
	эти значения к строковому представлению средствами sql:
	@code
		SELECT to_char(object_id) AS id FROM entities
		WHERE lower(name) like lower(:pattern)
	@endcode

	@see FacadeDB
*/
class DBConn : public QObject
{
	Q_OBJECT

public:
	DBConn(const QString &name);
	~DBConn();

	DBConn * clone(const QString &name);

	bool connect();
	void disconnect();
//	void close();

	QString host() const;
	QString user() const;
	QString password() const;
	QString driver() const;
	QString databaseName() const;
	QString connectOptions() const;
	int port() const;

	void setAccess( const QString &login, const QString &password);
	void setHost( const QString &host );
	void setPort(int port);
	void setDriver( const QString &driver );
	void setDatabaseName( const QString &name );
	void setConnectOptions(const QString &options);

	QSqlQuery executeQuery( const QString &sql );
	bool executeQuery(QSqlQuery query);
	QStringList errors() const;
	QSqlDatabase qtDatabase() const;
	void setQtDatabase( const QSqlDatabase &db  );
	bool hasTable( const QString& tableName );
	QString getDBVersionFromProp(const QString &dbPropTable,
						 const QString &versRowName,
						 const QString &versValRowName,
						 const QString &versInDbName);

	QString getDBVersion(const QString &dbPropTable,
						 const QString &versColName,
						 const QString &idColName);

	bool isConnected();
	void beginTransaction();
	bool commit();
	bool isInTransaction();
	void rollback();
	bool pingHost(const QString &host);

	bool checkDBConnect();

Q_SIGNALS:
	void closeConnection(const QString& name);

private:
	void printThreadWarning(const QString &func);

	QString m_name;
	QString m_host;
	QString m_user;
	QString m_password;
	QString m_driver;
	QString m_databaseName;
	QString m_connectOptions;
	int m_port;

	QSqlDatabase m_database;
	QStringList m_errors;
	volatile bool m_connected;
	bool inTransaction;

	QMutex m_mutex;
};

/** @struct DBConnScopedPtrDeleter
	@brief Вспомогательный класс для удаления QScopedPointer с DBConn */
struct DBConnScopedPtrDeleter
{
	/** @brief Отключения соединения с БД и удаление объекта соединения
		@param conn соединение */
	static inline void cleanup(DBConn *conn)
	{
		if (conn != NULL)
		{
			conn->disconnect();
			delete conn;
		}
	}
};

/** @typedef DBConnScopedPtr
	@brief QScopedPointer для объектов DBConn
	При выходе из области видимости соединение отключается и удаляется */
typedef QScopedPointer<DBConn, DBConnScopedPtrDeleter> DBConnScopedPtr;

#endif // DBConn_H
/*@}*/

