
#include <db/dbconnection.h>
#include <db/db_service.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>

#include <model/vid_item.h>

#include "vid_fetcher.h"

VidFetcher::VidFetcher()
{

}

bool VidFetcher::saveSlot(Item* item, DBConn* conn)
{
	VidItem* vi = (VidItem*)item;
	VidParam p = vi->getParam();

	QString sql;
	QSqlQuery q(conn->qtDatabase());

	int i = 0;
	if (p.id != 0)
	{
		sql =
			"UPDATE vid"
				" SET"
					" name = ?"
				" WHERE id = ?";
		q.prepare(sql);
		q.bindValue(i++, p.name);
		q.bindValue(i++, p.id);
	}
	else
	{
		sql = "SELECT nextval('vid_id_seq')";
		QSqlQuery seq(conn->qtDatabase());
		seq.exec(sql);
		if (seq.next())
		{
			qDebug() << "NEW ID = " << seq.value(0).toInt();
			p.id = seq.value(0).toInt();
		}
		sql = "INSERT INTO vid(id, name) VALUES(?, ?)";
		q.prepare(sql);
		q.bindValue(i++, p.id);
		q.bindValue(i++, p.name);
	}

	bool res = conn->executeQuery(q);
	if (res == true)
	{
		vi->setParam(p);
	}

	return res;
}

bool VidFetcher::deleteSlot(Item* item, DBConn* conn)
{
	VidItem* v = (VidItem*)item;
	VidParam p = v->getParam();
	QString sql = "DELETE FROM vid WHERE id = ?";
	QSqlQuery q(conn->qtDatabase());
	q.prepare(sql);
	int i = 0;
	q.bindValue(i++, p.id);
	return conn->executeQuery(q);
}

void VidFetcher::fetchSlot()
{
	QList<Item*> items;
	QString sql =
			"SELECT"
				" vid.id"
				", vid.name"
			" FROM vid"
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
		VidItem* item = new VidItem;
		VidParam p;
		int i = 0;
		p.id = q.value(i++).toInt();
		p.name = q.value(i++).toString();
		item->setParam(p);
		items.append(item);
	}
	Q_EMIT fetched(items);
}
