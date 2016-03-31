#include <QDebug>

//#include <unistd.h>

#include <db/db_service.h>
#include <model/shedule_item.h>

#include "shedule_fetcher.h"

SheduleFetcher::SheduleFetcher()
{

}

void SheduleFetcher::fetchSlot()
{
//	usleep(1000000);
	QList<Item*> items;
	QString sql =
			"SELECT"
				" id"
				", coach_id"
				", hall_id"
				", day"
				", begin_time"
				", end_time"
			" FROM shedule"
			" WHERE id <> 0";
	DBConn* conn = DBService::getInstance()->getConnection();
	if (!conn->isConnected())
	{
		qCritical() << Q_FUNC_INFO << "Ошибка подключания к БД";
		Q_EMIT fetched(items);
		return;
	}

	QSqlQuery q = conn->executeQuery(sql);
	while (q.next())
	{
		SheduleItem* item = new SheduleItem;
		SheduleParam p;
		int i = 0;
		p.id = q.value(i++).toInt();
		p.coach_id = q.value(i++).toInt();
		p.hall_id = q.value(i++).toInt();
		p.day = q.value(i++).toInt();
		p.bTime = q.value(i++).toTime();
		p.eTime = q.value(i++).toTime();
		item->setParam(p);
		items.append(item);
	}
	Q_EMIT fetched(items);
}

void SheduleFetcher::saveSlot(Item* item)
{
	SheduleItem* cItem = (SheduleItem*)item;
	SheduleParam p = cItem->getParam();

	QString sql = "";

	DBConn* conn = DBService::getInstance()->getConnection();
	if (!conn->isConnected())
	{
		qCritical() << Q_FUNC_INFO << "Ошибка подключания к БД";
		Q_EMIT saved(false);
		return;
	}

	conn->beginTransaction();
	QSqlQuery q(conn->qtDatabase());

	int i = 0;
	if (p.id != 0)
	{
		sql =
			"UPDATE shedule"
				" SET"
					" coach_id = ?"
					", hall_id = ?"
					", day = ?"
					", begin_time = ?"
					", end_time = ?"
				" WHERE id = ?";
		q.prepare(sql);
		q.bindValue(i++, p.coach_id);
		q.bindValue(i++, p.hall_id);
		q.bindValue(i++, p.day);
		q.bindValue(i++, p.bTime);
		q.bindValue(i++, p.eTime);
		q.bindValue(i++, p.id);
	}
	else
	{
		sql = "SELECT nextval('shedule_id_seq')";
		QSqlQuery seq(conn->qtDatabase());
		seq.exec(sql);
		if (seq.next())
		{
			qDebug() << "NEW ID = " << seq.value(0).toInt();
			p.id = seq.value(0).toInt();
			cItem->setParam(p);
		}
		sql =
			"INSERT INTO shedule("
					" id, coach_id, hall_id, day, begin_time, end_time)"
				" VALUES(?, ?, ?, ?, ?, ?)";
		q.prepare(sql);
		q.bindValue(i++, p.id);
		q.bindValue(i++, p.coach_id);
		q.bindValue(i++, p.hall_id);
		q.bindValue(i++, p.day);
		q.bindValue(i++, p.bTime);
		q.bindValue(i++, p.eTime);
	}

	bool res = conn->executeQuery(q);
	conn->commit();

	Q_EMIT saved(res);
}

void SheduleFetcher::deleteSlot(int id)
{
	DBConn* conn = DBService::getInstance()->getConnection();
	if (!conn->isConnected())
	{
		qCritical() << Q_FUNC_INFO << "Ошибка подключания к БД";
		Q_EMIT deleted(false);
		return;
	}

	if (id <= 0)
	{
		qCritical() << Q_FUNC_INFO << "Неверный идентификатор";
		Q_EMIT deleted(false);
		return;
	}

	conn->beginTransaction();
	bool res = false;
	QString sql = "DELETE FROM shedule WHERE id = " + QString::number(id);
	QSqlQuery q = conn->executeQuery(sql);
	if (q.isActive())
	{
		res = true;
	}

	if (res == true)
	{
		conn->commit();
	}
	else
	{
		conn->rollback();
	}

	Q_EMIT deleted(res);
}
