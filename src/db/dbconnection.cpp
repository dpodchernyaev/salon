#include <QHash>
#include <QSqlError>
#include <QSqlRecord>
#include <QtDebug>
#include <QMutexLocker>
#include <QCoreApplication>
#include <QThread>
#include <assert.h>

//#include "core/app_runtime_data.h"
#include "db/dbconnection.h"
#include "db/ping.h"

const int PING_COUNT = 3; //Сколько раз пинговать перед переподключением

/** Конструктор по умолчанию.
  * @param name Имя соединения. */
DBConn::DBConn(const QString &name)
	: m_mutex(QMutex::Recursive)
{
	m_port = 1521;
	m_host = "127.0.0.1";
	m_driver = "QOCI";
	m_connected = false;
	m_name = name;

	inTransaction = false;
}

DBConn::~DBConn()
{
	Q_EMIT closeConnection(m_name);

	qDebug() << "DBConn::~DBConn() m_name=" << m_name;
}

/** Дублирует объект соединения.
  * @param name Имя клона.
  * @return Указатель на новое соединение.
  */
DBConn * DBConn::clone(const QString &name)
{
	DBConn * d = new DBConn(name);
	d->setAccess(m_user, m_password);
	d->setDatabaseName(m_databaseName);
	d->setDriver(m_driver);
	d->setHost(m_host);
	d->setPort(m_port);
	d->setConnectOptions(m_connectOptions);
	return d;
}

/** Выполняет подключение к базе данных
@return Результат подключения
  */
bool DBConn::connect()
{
	QMutexLocker locker(&m_mutex);
	if (m_connected)
	{
		qWarning() << "Double connection detected! " + m_name;
		return true;
	}

	if (QThread::currentThread() != this->thread())
	{
		printThreadWarning(Q_FUNC_INFO);

		qFatal("DB thread ERROR");
	}

	// Для замещения стандартного соединения
	if (m_name == "default")
	{
		m_database = QSqlDatabase::addDatabase(m_driver);
	}
	else
	{
		if (m_name.isEmpty())
		{
			m_name="no_input_db_name";
		}
		m_database = QSqlDatabase::addDatabase(m_driver, m_name);

	}

	m_database.setDatabaseName(m_databaseName);
	m_database.setHostName(m_host);
	m_database.setUserName(m_user);
	m_database.setPassword(m_password);
	m_database.setPort(m_port);
	m_database.setConnectOptions(m_connectOptions);

	bool db_open_flag = m_database.open();
	if (! db_open_flag)
	{
		qDebug() << "Error connecting DB:" << m_database.lastError().text();
	}
	else
	{
		qDebug() << "Connection established:" << m_name;
	}

	// Ставим флаг соединения
	m_connected = db_open_flag;

	if (m_driver.contains("QPSQL", Qt::CaseInsensitive) && m_connected)
	{
		executeQuery("SET bytea_output=escape");
	}

	return db_open_flag;
}

/** Выполняет отключение от базы данных */
void DBConn::disconnect()
{
	qDebug() << "DBConn::disconnect() " << m_name;
	QMutexLocker locker(&m_mutex);

	if (QThread::currentThread() != this->thread())
	{
		printThreadWarning(Q_FUNC_INFO);
		qFatal("DB thread ERROR");
	}

	QString connection;
	connection = m_database.connectionName();
	m_database.close();
	m_database = QSqlDatabase();
	m_database.removeDatabase(connection);
}

/** Начинает транзакцию */
void DBConn::beginTransaction()
{
	QMutexLocker locker(&m_mutex);

	if (QThread::currentThread() != this->thread())
	{
		printThreadWarning(Q_FUNC_INFO);
		qFatal("DB thread ERROR");
	}

	m_database.transaction();
	inTransaction = true;
}

/** Подтверждает транзакцию */
bool DBConn::commit()
{
	QMutexLocker locker(&m_mutex);
	qDebug() << "Commit:" << m_name;

	if (QThread::currentThread() != this->thread())
	{
		printThreadWarning(Q_FUNC_INFO);
		qFatal("DB thread ERROR");
	}

	inTransaction = false;
	return (m_database.commit());
}

/** Отменяет транзакцию */
void DBConn::rollback()
{
	QMutexLocker locker(&m_mutex);
	qDebug() << "Rollback:" << m_name;

	if (QThread::currentThread() != this->thread())
	{
		printThreadWarning(Q_FUNC_INFO);
		qFatal("DB thread ERROR");
	}

	m_database.rollback();
	inTransaction = false;
}


/** Выполняет запрос к базе данных
  @param sql Строка запроса
  @return Объект-запрос
*/
QSqlQuery DBConn::executeQuery(const QString &sql)
{
	QMutexLocker locker(&m_mutex);

	if (!isConnected())
	{
		//Попытка переподключения
		disconnect();
		if (! connect())
		{
			return QSqlQuery();
		}
	}

	if (QThread::currentThread() != this->thread())
	{
		printThreadWarning(Q_FUNC_INFO);
		qFatal("DB thread ERROR");
	}
	qDebug() << "Trying query" << sql
				   << " [conn:" << m_name << "]";

	// Выполняем запрос
	QSqlQuery query(m_database);
	query.exec(sql);

	// Добавляем ошибки в лог
	if (query.lastError().isValid())
	{
		qCritical() << "DBConn (connection '" << m_name << "'):"
					<< query.lastError().databaseText() << "  SQL: "
					<< sql;
	}

	return query;
}

/** Выполняет запрос к базе данных
  @param query Объект-запрос
  @return Результат выполнения запроса
  @note Внимание! Функция выплняется в соединении по умолчанию, поэтому
  при создании QSqlQuery нужно передать указатель на соединение

*/
bool DBConn::executeQuery(QSqlQuery query)
{
	QMutexLocker locker(&m_mutex);

	if (QThread::currentThread() != this->thread())
	{
		printThreadWarning(Q_FUNC_INFO);
		qFatal("DB thread ERROR");
	}

	qDebug() << "Trying query" << query.lastQuery()
				   << " [conn:" << m_name << "]";

	if (!isConnected())
	{
		//Попытка переподключения
		disconnect();
		connect();
		//Не выполняем запрос, не известно в каком он состоянии
		return false;
	}

	query.exec();

	if (! query.isActive())
	{
		qCritical() << "DBConn (connection '" << m_name << "')"
					<< query.lastError().databaseText() << "  SQL: "
					<< query.lastQuery();

		return false;
	}
	return true;
}

/** Возвращает список ошибок */
QStringList DBConn::errors() const
{
	return m_errors;
}

/** Установить соединение
  @param db Объект-база_данных
  */
void DBConn::setQtDatabase(const QSqlDatabase &db)
{
	QMutexLocker locker(&m_mutex);

	m_database = db;
}

/** Возвращает QSqlDatabase */
QSqlDatabase DBConn::qtDatabase() const
{
	return m_database;
}

/** Установка хоста БД*/
void DBConn::setHost(const QString &host)
{
	m_host = host;
}

/** Установка порта
  @param port Номер порта БД
*/
void DBConn::setPort(int port)
{
	m_port = port;
}

/** Установка драйвера БД
  @param driver Драйвер базы данных
*/
void DBConn::setDriver(const QString &driver)
{
	m_driver = driver;
}

/** Установка имени базы данных
  @param name Имя базы данных
*/

void DBConn::setDatabaseName(const QString& name)
{
	m_databaseName = name;
}

/** Установка дополнительных опций соединения
	@param options опции соединения (см. QSqlDatabase::setConnectOptions) */
void
DBConn::setConnectOptions(const QString &options)
{
	m_connectOptions = options;
}

/** Установка имени пользователя и пароля
  @param login Логин
  @param password Пароль
*/
void DBConn::setAccess(const QString &login, const QString &password)
{
	m_user = login;
	m_password = password;
}

/** Проверка наличия таблицы в БД
  @param tableName Имя таблицы
  @return Возвращает в случае наличия таблицы в БД
  */
bool DBConn::hasTable(const QString& tableName)
{
	QString sql;

	if (m_driver.contains("QOCI", Qt::CaseInsensitive))
	{
		sql =	QString("SELECT COUNT(table_name) FROM user_tables "
						"WHERE table_name='%1'").arg(tableName);
	}
	else if (m_driver.contains("QPSQL", Qt::CaseInsensitive))
	{
		QString schemaName = "public";
		QString table;

		if (tableName.contains(".")) //есть схема
		{
			schemaName = tableName.mid(0, tableName.indexOf(".")).toLower();
			table = QString(tableName).remove(
						QString(schemaName + "."), Qt::CaseInsensitive);
		}
		else
		{
			table = tableName;
		}

		sql =	QString("SELECT COUNT(*) FROM pg_tables "
						"WHERE tablename = '%1' AND schemaname='%2' ")
				.arg(table).arg(schemaName);
	}
	else if (m_driver.contains("QODBC3", Qt::CaseInsensitive) ||
			m_driver.contains("QODBC-4.6.2", Qt::CaseInsensitive))
	{
		sql = QString("SELECT COUNT(table_name) FROM iitables "
						"WHERE table_name='%1'").arg(tableName);
	}
	else
	{
		qFatal("%s", QString("Unable to check table for database: " +
					m_driver).toUtf8().data());
		return false;
	}

	QSqlQuery query = executeQuery(sql);
	if (!query.isActive())
	{
		qDebug() << "ERROR" << query.lastError().text();
	}

	query.first();
	if (query.value(0).toInt() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/** Получает версию БД
	@param dbPropTable Имя таблицы свойств
	@param versRowName Столбец имени свойства
	@param versValRowName Имя строки с версией
	@param versInDbName Значение для версии
	@return Версию БД
*/
QString DBConn::getDBVersionFromProp(const QString &dbPropTable,
									 const QString &versRowName,
									 const QString &versValRowName,
									 const QString &versInDbName)
{
	QString sql = QString("SELECT %1 FROM %2 "
						  "WHERE %3='%4'").arg(versValRowName)
			.arg(dbPropTable)
			.arg(versRowName).arg(versInDbName);

	QSqlQuery query = executeQuery(sql);
	if (!query.isActive())
	{
		qCritical() << "ERROR" << query.lastError().text();
		return "";
	}

	query.first();
	return query.value(0).toString();
}

/** Получает версию БД
	@param dbPropTable Имя таблицы версий
	@param versColName Столбец версии
	@param idColName Столбец ID
	@return Версию БД
 */
QString DBConn::getDBVersion(const QString &dbPropTable,
							 const QString &versColName,
							 const QString &idColName)
{

	QString sql =
			QString(
				"SELECT %1 FROM %2  "
				"WHERE %3 = (SELECT MAX(%3) FROM %2)"
				)
			.arg(versColName)
			.arg(dbPropTable)
			.arg(idColName);

	QSqlQuery query = executeQuery(sql);
	if (!query.isActive())
	{
		qCritical() << "ERROR" << query.lastError().text();
		return "";
	}

	query.first();
	return query.value(0).toString();
}

/** Возвращает хост */
QString DBConn::host() const
{
	return m_host;
}

/** Возвращает имя пользователя */
QString DBConn::user() const
{
	return m_user;
}

/** Возвращает пароль */
QString DBConn::password() const
{
	return m_password;
}

/** Возвращает драйвер */
QString DBConn::driver() const
{
	return m_driver;
}

/** Возвращает имя баз данынх */
QString DBConn::databaseName() const
{
	return m_databaseName;
}

/** Возвращает опции соединения */
QString
DBConn::connectOptions() const
{
	return m_connectOptions;
}

/** Возвращает порт */
int DBConn::port() const
{
	return m_port;
}

/** Проверка подключения */
bool DBConn::isConnected()
{
	QMutexLocker locker(&m_mutex);

	if (m_driver.contains("QPSQL", Qt::CaseInsensitive))
	{
		m_connected = m_database.isOpen();
	}
	else
	{
		if (pingHost(m_host))
		{
			m_connected = m_database.isOpen();
		}
		else
		{
			m_connected = false;
		}
	}
	return m_connected;
}

/** Проверка соединения с БД для Oracle.
	Запускает простой SQL запрос, и если он завершается с ошибкой то
	соединения нет*/
bool DBConn::checkDBConnect()
{
	QString sql;

	if (m_driver.contains("QOCI", Qt::CaseInsensitive))
	{
		sql =	"SELECT * FROM dual";
	}
	else
	{
		if (m_driver.contains("QPSQL", Qt::CaseInsensitive))
		{
			sql = "SELECT 1";
		}
		else
		{
			qFatal("%s", QString("Unable to check table for database: ")
				   .append(m_driver).toUtf8().data());
			return false;
		}
	}

	QSqlQuery query = executeQuery(sql);

	if (query.lastError().isValid())
	{
		qCritical() << "ERROR" << query.lastError().text();

		qCritical() << "DBConn checkDBConnect (connection '" << m_name << "'):"
					<< query.lastError().databaseText() << "  SQL: "
					<< sql;

		return false;
	}

	return true;
}

/** Проверяет доступность сервера
	@param host Хост
*/
bool DBConn::pingHost(const QString &host)
{
	Ping ping;
	for (int i = 0; i < PING_COUNT; i++)
	{
		if (ping.doPing(host))
		{
			return true;
		}
	}
	return false;
}

/** Идет ли сейчас транзакция БД */
bool DBConn::isInTransaction()
{
	return inTransaction;
}

/**
 * @brief DBConn::printThreadWarning
 * Вывести в отладку предупреждение об использовании соединения в другом потоке
 * @param func из какой функции вызвали
 */
void
DBConn::printThreadWarning(const QString &func)
{
	qCritical() << "DBConn: Trying to" << func << "from a different thread!";
	QThread *current = QThread::currentThread();

	QString connMsg;
	QString currentMsg;
	if (qApp != NULL)
	{
		if (qApp->thread() == this->thread())
		{
			connMsg = "[Main thread]";
		}

		if (qApp->thread() == current)
		{
			currentMsg = "[Main thread]";
		}
	}

	qDebug() << "Connection thread:" << this->thread() << qPrintable(connMsg);
	qDebug() << "Current thread:" << current << qPrintable(currentMsg);
}

