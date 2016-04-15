
#include <QDebug>

#include <unistd.h>
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

bool GroupFetcher::savePrivateGroup(GroupItem* gi, DBConn* conn)
{
	bool res = true;
	int i = 0;
	int cnt = 0;

	QString sql = "SELECT count(*) FROM private_group WHERE vgroup_id = ?";

	// имеется ли запись с указанным идентификатором
	QSqlQuery q(conn->qtDatabase());
	q.prepare(sql);
	q.bindValue(i++, gi->getId());
	res = conn->executeQuery(q);
	if ( (res == true) && q.next())
	{
		PrivateGroupParam p = gi->getPrivateParam();
		cnt = q.value(0).toInt();
		i = 0;

		if (cnt == 0)
		{
			sql = "INSERT INTO private_group(vgroup_id, summ)"
					" VALUES(?, ?)";
			q.prepare(sql);
			q.bindValue(i++, gi->getId());
			q.bindValue(i++, p.summ);
		}
		else
		{
			sql = "UPDATE private_group"
					" SET vgroup_id = ?, summ = ?"
					" WHERE vgroup_id = ?";
			q.prepare(sql);
			q.bindValue(i++, gi->getId());
			q.bindValue(i++, p.summ);
			q.bindValue(i++, gi->getId());
		}
		res = conn->executeQuery(q);
	}
	return res;
}

bool GroupFetcher::saveSlot(Item* item, DBConn *conn)
{
//	usleep(1000000 * 4);
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

	if ( (res == true) && (cItem->isPrivate()) )
	{
		res = savePrivateGroup(cItem, conn);
	}
	return res;
}

QList<Item*> GroupFetcher::fetchSlot(DBConn* conn)
{
	QList<Item*> items;

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
				", summ"
			" FROM vgroup LEFT JOIN"
				" private_group ON vgroup.id = private_group.vgroup_id"
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
		PrivateGroupParam pp;

		int i = 0;
		param.id = q.value(i++).toInt();
		param.coach_id = q.value(i++).toInt();
		param.hall_id = q.value(i++).toInt();
		param.bdtime = q.value(i++).toDateTime();
		param.etime = q.value(i++).toTime();
		param.cnt = q.value(i++).toInt();
		param.vid_id = q.value(i++).toInt();
		QVariant summVar = q.value(i++);


		if (!summVar.isNull())
		{
			pp.used = true;
			pp.summ = summVar.toDouble();
		}

		GroupItem* item = new GroupItem;
		item->setParam(param);
		item->setPrivateParam(pp);
		items.append(item);
	}
	return items;
}
