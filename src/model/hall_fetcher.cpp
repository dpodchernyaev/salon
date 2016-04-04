#include <QDebug>

#include <model/hall_item.h>
#include <db/db_service.h>

#include "hall_fetcher.h"

HallFetcher::HallFetcher()
{

}

void HallFetcher::fetchSlot()
{
	QList<Item*> items;
	QString sql =
			"SELECT"
				" hall.id"
				", hall.name"
				", hall.cnt"
			" FROM hall"
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
		HallItem* item = new HallItem;
		int i = 0;
		item->setId(q.value(i++).toInt());
		item->setName(q.value(i++).toString());
		item->setCnt(q.value(i++).toInt());
		items.append(item);
	}
	Q_EMIT fetched(items);
}

bool HallFetcher::deleteSlot(Item *i, DBConn *conn)
{
	QString sql = "UPDATE shedule SET hall_id = 0"
				  " WHERE hall_id = " + QString::number(i->getId());
	QString sql1 = "UPDATE vgroup SET hall_id = 0"
				  " WHERE hall_id = " + QString::number(i->getId());


	QSqlQuery q = conn->executeQuery(sql);
	bool res = q.isActive();

	if (res == true)
	{
		q = conn->executeQuery(sql1);
		res = q.isActive();
	}

	if (res == true)
	{
		sql = "DELETE FROM hall WHERE id = " + QString::number(i->getId());
		q = conn->executeQuery(sql);
		res = q.isActive();
	}
	return res;
}

bool HallFetcher::saveSlot(Item* item, DBConn *conn)
{
	HallItem* cItem = (HallItem*)item;

	int id = cItem->getId();
	QString name = cItem->getName();
	int cnt = cItem->getCnt();
	QString sql = "";
	QSqlQuery q(conn->qtDatabase());

	int i = 0;
	if (id != 0)
	{
		sql =
			"UPDATE hall"
				" SET"
					" name = ?"
					", cnt = ?"
				" WHERE id = ?";
		q.prepare(sql);
		q.bindValue(i++, name);
		q.bindValue(i++, cnt);
		q.bindValue(i++, id);
	}
	else
	{
		sql = "SELECT nextval('hall_id_seq')";
		QSqlQuery seq(conn->qtDatabase());
		seq.exec(sql);
		if (seq.next())
		{
			qDebug() << "NEW ID = " << seq.value(0).toInt();
			id = seq.value(0).toInt();
			cItem->setId(id);
		}
		sql =
			"INSERT INTO hall("
					" id, name, cnt)"
				" VALUES(?, ?, ?)";
		q.prepare(sql);
		q.bindValue(i++, id);
		q.bindValue(i++, name);
		q.bindValue(i++, cnt);
	}

	return conn->executeQuery(q);
}
