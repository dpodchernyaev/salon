
#include <QDebug>
#include <QThread>
#include <QTime>

#include <db/db_service.h>

#include <model/item.h>

#include "fetcher.h"

Fetcher::Fetcher()
{
	DBService* service = DBService::getInstance();
	QThread* th = service->getThread();
	moveToThread(th);

	connect(this, SIGNAL(fetchSignal()), SLOT(fetchPrivate()));
	connect(this, SIGNAL(saveSignal(Item*)), SLOT(savePrivate(Item*)));
	connect(this, SIGNAL(deleteSignal(Item*)), SLOT(deletePrivate(Item*)));
}

Fetcher::~Fetcher()
{

}

void Fetcher::fetch()
{
	Q_EMIT fetchSignal();
}

void Fetcher::save(Item* item)
{
	Q_EMIT saveSignal(item);
}

void Fetcher::deleteItem(Item *item)
{
	Q_EMIT deleteSignal(item);
}

bool Fetcher::refetch(DBConn* conn)
{
	QList<Item*> res = fetchSlot(conn);
	Q_EMIT fetched(res);
}

void Fetcher::fetchPrivate()
{

	DBConn* conn = DBService::getInstance()->getConnection();
	if (!conn->isConnected())
	{
		qCritical() << Q_FUNC_INFO << "Ошибка подключания к БД";
		Q_EMIT fetched(QList<Item*>());
		return;
	}

	QTime t;
	t.start();

	conn->beginTransaction();
	refetch(conn);
	conn->commit();

	if (t.elapsed() > (1000 * 2) )
	{
		qWarning() << metaObject()->className()
				   << "Время выполнения запроса данных больше 2 сек = " << t.elapsed();
	}
}

void Fetcher::deletePrivate(Item* item)
{
	DBConn* conn = DBService::getInstance()->getConnection();
	if (!conn->isConnected())
	{
		qCritical() << Q_FUNC_INFO << "Ошибка подключания к БД";
		Q_EMIT deleted(false);
		return;
	}

	if (item->getId() <= 0)
	{
		qCritical() << Q_FUNC_INFO << "Передан отрицательный идентификатор для удаления из БД";
		Q_EMIT deleted(false);
		return;
	}

	QTime t;
	t.start();

	conn->beginTransaction();

	bool res = deleteSlot(item, conn);

	if (res == true)
	{
		conn->commit();
	}
	else
	{
		conn->rollback();
	}

	if (t.elapsed() > (1000 * 2) )
	{
		qWarning() << metaObject()->className()
				   << "Время удаления объекта больше 2 сек = " << t.elapsed();
	}

	Q_EMIT deleted(res);
}

void Fetcher::savePrivate(Item* item)
{
	DBConn* conn = DBService::getInstance()->getConnection();
	if (!conn->isConnected())
	{
		qCritical() << Q_FUNC_INFO << "Ошибка подключания к БД";
		Q_EMIT saved(item, false);
		return;
	}

	QTime t;
	t.start();

	conn->beginTransaction();

	bool res = saveItem(item, conn);

	if (res == true)
	{
		conn->commit();
	}
	else
	{
		conn->rollback();
	}

	if (t.elapsed() > (1000 * 2) )
	{
		qWarning() << metaObject()->className()
				   << "Время сохранения объекта больше 2 сек = " << t.elapsed();
	}
}

bool Fetcher::saveItem(Item* item, DBConn* conn)
{
	bool res = saveSlot(item, conn);
	Q_EMIT saved(item, res);
	return res;
}
