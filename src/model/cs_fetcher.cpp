#include <QDebug>

#include <model/client_service_item.h>
#include <db/db_service.h>

#include "cs_fetcher.h"

CsFetcher::CsFetcher()
{
}

#include <unistd.h>

QList<Item*> CsFetcher::fetchSlot(DBConn* conn)
{
	usleep(1000000 * 3);
	QList<Item*> items;
	QString sql =
			"SELECT"
				" id"
				", client_id"
				", date"
				", summ"
				", limit_value"
				", limit_days"
				", limit_type"
				", name"
				", vid_id"
			" FROM client_service"
			" WHERE id <> 0 AND client_id = " + QString::number(id);

	QSqlQuery q = conn->executeQuery(sql);
	while (q.next())
	{
		CsItem* item = new CsItem;
		CsParam p;
		int i = 0;
		p.id = q.value(i++).toInt();
		p.client_id = q.value(i++).toInt();
		p.date = q.value(i++).toDateTime();
		p.summ = q.value(i++).toDouble();
		p.limit_value = q.value(i++).toInt();
		p.limit_days = q.value(i++).toInt();
		p.limit_type = (LimitType)q.value(i++).toInt();
		p.name = q.value(i++).toString();
		p.vid_id = q.value(i++).toInt();
		item->setParam(p);
		items.append(item);
	}
	return items;
}

bool CsFetcher::saveSlot(Item* item, DBConn* conn)
{
//	usleep(1000000 * 3);
	CsItem* cItem = (CsItem*)item;
	CsParam p = cItem->getParam();
	QString sql = "";
	QSqlQuery q(conn->qtDatabase());

	int i = 0;
	if (p.id != 0)
	{
		sql =
			"UPDATE client_service"
				" SET"
					" client_id = ?"
					", date = ?"
					", summ = ?"
					", limit_value = ?"
					", limit_days = ?"
					", limit_type = ?"
					", name = ?"
					", vid_id = ?"
				" WHERE id = ?";
		q.prepare(sql);
		q.bindValue(i++, p.client_id);
		q.bindValue(i++, p.date);
		q.bindValue(i++, p.summ);
		q.bindValue(i++, p.limit_value);
		q.bindValue(i++, p.limit_days);
		q.bindValue(i++, p.limit_type);
		q.bindValue(i++, p.name);
		q.bindValue(i++, p.vid_id);
		q.bindValue(i++, p.id);
	}
	else
	{
		sql = "SELECT nextval('client_service_id_seq')";
		QSqlQuery seq(conn->qtDatabase());
		seq.exec(sql);
		if (seq.next())
		{
			qDebug() << "NEW ID = " << seq.value(0).toInt();
			p.id = seq.value(0).toInt();
			cItem->setParam(p);
		}
		sql =
			"INSERT INTO client_service("
					" id, client_id, date, summ, limit_value, limit_days, limit_type, name, vid_id)"
				" VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?)";
		q.prepare(sql);
		q.bindValue(i++, p.id);
		q.bindValue(i++, p.client_id);
		q.bindValue(i++, p.date);
		q.bindValue(i++, p.summ);
		q.bindValue(i++, p.limit_value);
		q.bindValue(i++, p.limit_days);
		q.bindValue(i++, p.limit_type);
		q.bindValue(i++, p.name);
		q.bindValue(i++, p.vid_id);
	}

	return conn->executeQuery(q);
}

bool CsFetcher::deleteSlot(Item *i, DBConn *conn)
{
	QString sql = "DELETE FROM client_service WHERE id = " +
				  QString::number(i->getId());
	QSqlQuery q = conn->executeQuery(sql);
	return q.isActive();
}
