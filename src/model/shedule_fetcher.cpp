#include <QDebug>

//#include <unistd.h>

#include <db/db_service.h>
#include <model/shedule_item.h>

#include "shedule_fetcher.h"

SheduleFetcher::SheduleFetcher()
{

}

QList<Item*> SheduleFetcher::fetchSlot(DBConn* conn)
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
				", vid_id"
			" FROM shedule"
			" WHERE id <> 0";
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
		p.vid_id = q.value(i++).toInt();
		item->setParam(p);
		items.append(item);
	}
	return items;
}

bool SheduleFetcher::saveSlot(Item* item, DBConn* conn)
{
	SheduleItem* cItem = (SheduleItem*)item;
	SheduleParam p = cItem->getParam();
	QString sql = "";
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
					", vid_id = ?"
				" WHERE id = ?";
		q.prepare(sql);
		q.bindValue(i++, p.coach_id);
		q.bindValue(i++, p.hall_id);
		q.bindValue(i++, p.day);
		q.bindValue(i++, p.bTime);
		q.bindValue(i++, p.eTime);
		q.bindValue(i++, p.vid_id);
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
					" id, coach_id, hall_id, day, begin_time, end_time, vid_id)"
				" VALUES(?, ?, ?, ?, ?, ?, ?)";
		q.prepare(sql);
		q.bindValue(i++, p.id);
		q.bindValue(i++, p.coach_id);
		q.bindValue(i++, p.hall_id);
		q.bindValue(i++, p.day);
		q.bindValue(i++, p.bTime);
		q.bindValue(i++, p.eTime);
		q.bindValue(i++, p.vid_id);
	}

	return conn->executeQuery(q);
}

bool SheduleFetcher::deleteSlot(Item *i, DBConn *conn)
{
	QString sql = "DELETE FROM shedule WHERE id = " +
					QString::number(i->getId());
	QSqlQuery q = conn->executeQuery(sql);
	return q.isActive();
}

