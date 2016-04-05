
#include <QDebug>

#include <model/group_item.h>
#include <db/db_service.h>

#include "group_fetcher.h"

GroupFetcher::GroupFetcher()
{
	btime = QDateTime::currentDateTime();
	btime.setTime(QTime(0, 0));
	etime = QDateTime::currentDateTime().addMonths(2);
	etime.setTime(QTime(0, 0));
}

GroupFetcher::~GroupFetcher()
{
}

void GroupFetcher::setTimeInterval(const QDateTime &btime, const QDateTime &etime)
{
	this->btime = btime;
	this->etime = etime;
}

void GroupFetcher::addGroup(const GroupParam &param)
{
	// группа уже присутствует
	if (param.id != 0)
	{
		qWarning() << "Группа уже присутствует в БД";
		return;
	}

	GroupItem* i = new GroupItem;
	i->setParam(param);
	save(i);
}

bool GroupFetcher::deleteSlot(Item *i, DBConn *conn)
{
	QString sql = "DELETE FROM vgroup WHERE id = " + QString::number(i->getId());
	QSqlQuery q = conn->executeQuery(sql);
	return q.isActive();
}

bool GroupFetcher::saveSlot(Item* item, DBConn *conn)
{
	GroupItem* cItem = (GroupItem*)item;
	GroupParam p = cItem->getParam();
	QString sql = "";
	QSqlQuery q(conn->qtDatabase());

	int i = 0;
	if (p.id != 0)
	{
		sql =
			"UPDATE vgroup"
				" SET"
					" coach_id = ?"
					", hall_id = ?"
					", bdtime = ?"
					", etime = ?"
					", cnt = ?"
					", vid_id = ?"
				" WHERE id = ?";
		q.prepare(sql);
		q.bindValue(i++, p.coach_id);
		q.bindValue(i++, p.hall_id);
		q.bindValue(i++, p.bdtime);
		q.bindValue(i++, p.etime);
		q.bindValue(i++, p.cnt);
		q.bindValue(i++, p.vid_id);
		q.bindValue(i++, p.id);
	}
	else
	{
		sql = "SELECT nextval('shedule_group_id_seq')";
		QSqlQuery seq(conn->qtDatabase());
		seq.exec(sql);
		if (seq.next())
		{
			qDebug() << "NEW ID = " << seq.value(0).toInt();
			p.id = seq.value(0).toInt();
			cItem->setParam(p);
		}
		sql =
			"INSERT INTO vgroup("
					" id, coach_id, hall_id, bdtime, etime, cnt, vid_id)"
				" VALUES(?, ?, ?, ?, ?, ?, ?)";
		q.prepare(sql);
		q.bindValue(i++, p.id);
		q.bindValue(i++, p.coach_id);
		q.bindValue(i++, p.hall_id);
		q.bindValue(i++, p.bdtime);
		q.bindValue(i++, p.etime);
		q.bindValue(i++, p.cnt);
		q.bindValue(i++, p.vid_id);
	}

	bool res = conn->executeQuery(q);
	return res;
}

void GroupFetcher::fetchSlot()
{
	QList<Item*> items;
	DBConn* conn = DBService::getInstance()->getConnection();
	if (!conn->isConnected())
	{
		qCritical() << Q_FUNC_INFO << "Ошибка подключания к БД";
		Q_EMIT fetched(items);
		return;
	}

	QSqlQuery q(conn->qtDatabase());
	QString sql =
			"SELECT"
				" id"
				", coach_id"
				", hall_id"
				", bdtime"
				", etime"
				", cnt"
				", vid_id"
			" FROM vgroup"
			" WHERE id <> 0"
				" AND (bdtime BETWEEN ? AND ?)";

	qDebug() << "TIMES: " << btime << etime;

	q.prepare(sql);
	int i = 0;
	q.bindValue(i++, btime);
	q.bindValue(i++, etime);

	bool res = conn->executeQuery(q);
	while ( (res == true) && q.next())
	{
		GroupParam param;

		int i = 0;
		param.id = q.value(i++).toInt();
		param.coach_id = q.value(i++).toInt();
		param.hall_id = q.value(i++).toInt();
		param.bdtime = q.value(i++).toDateTime();
		param.etime = q.value(i++).toTime();
		param.cnt = q.value(i++).toInt();
		param.vid_id = q.value(i++).toInt();

		GroupItem* item = new GroupItem;
		item->setParam(param);
		items.append(item);
	}
	Q_EMIT fetched(items);
}
