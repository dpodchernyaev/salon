#include <QDebug>

#include <model/coach_item.h>
#include <db/db_service.h>

#include "coach_fetcher.h"

CoachFetcher::CoachFetcher()
{

}

void CoachFetcher::fetchSlot()
{
	QList<Item*> items;
	QString sql =
			"SELECT"
				" coach.id"
				", coach.name"
			" FROM coach"
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
		CoachItem* item = new CoachItem;
		int i = 0;
		item->setId(q.value(i++).toInt());
		item->setName(q.value(i++).toString());
		items.append(item);
	}
	Q_EMIT fetched(items);
}

bool CoachFetcher::saveSlot(Item* item, DBConn *conn)
{
	CoachItem* cItem = (CoachItem*)item;

	int id = cItem->getId();
	QString name = cItem->getName();
	QString phone = cItem->getPhone();
	QString sql = "";
	QSqlQuery q(conn->qtDatabase());

	int i = 0;
	if (id != 0)
	{
		sql =
			"UPDATE coach"
				" SET"
					" name = ?"
				" WHERE id = ?";
		q.prepare(sql);
		q.bindValue(i++, name);
		q.bindValue(i++, id);
	}
	else
	{
		sql = "SELECT nextval('coach_id_seq')";
		QSqlQuery seq(conn->qtDatabase());
		seq.exec(sql);
		if (seq.next())
		{
			qDebug() << "NEW ID = " << seq.value(0).toInt();
			id = seq.value(0).toInt();
		}
		sql =
			"INSERT INTO coach("
					" id, name)"
				" VALUES(?, ?)";
		q.prepare(sql);
		q.bindValue(i++, id);
		q.bindValue(i++, name);
	}

	bool res = conn->executeQuery(q);

	if (res == true)
	{
		cItem->setId(id);
	}

	return res;
}

bool CoachFetcher::deleteSlot(Item *i, DBConn *conn)
{
	bool res = false;

	QString sql = "UPDATE shedule SET coach_id = 0"
				  " WHERE coach_id = " + QString::number(i->getId());

	QString sql1 = "UPDATE vgroup SET coach_id = 0"
				  " WHERE coach_id = " + QString::number(i->getId());

	QSqlQuery q = conn->executeQuery(sql);
	if (q.isActive())
	{
		q = conn->executeQuery(sql1);
	}

	if (q.isActive())
	{
		sql = "DELETE FROM coach WHERE id = " + QString::number(i->getId());
		q = conn->executeQuery(sql);
		if (q.isActive())
		{
			res = true;
		}
	}

	return res;
}
