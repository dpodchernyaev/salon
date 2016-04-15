#include <QDebug>

#include <model/service_item.h>
#include <db/db_service.h>

#include "service_fetcher.h"

ServiceFetcher::ServiceFetcher()
{

}

bool ServiceFetcher::deleteSlot(Item *i, DBConn *conn)
{
	if (i->getId() <= 0)
	{
		qCritical() << Q_FUNC_INFO << "Неверный идентификатор";
		return false;
	}

	QString sql = "DELETE FROM service WHERE id = "
				  + QString::number(i->getId());
	QSqlQuery q = conn->executeQuery(sql);
	return q.isActive();
}

QList<Item*> ServiceFetcher::fetchSlot(DBConn* conn)
{
	QList<Item*> items;
	QString sql =
			"SELECT"
				" service.id"
				", service.name"
				", service.price"
				", service.limit_type"
				", service.value"
				", service.limit_days"
				", service.used"
				", service.vid_id"
			" FROM service"
			" WHERE id <> 0";

	QSqlQuery q = conn->executeQuery(sql);
	while (q.next())
	{
		ServiceItem* item = new ServiceItem;
		ServiceParam p = item->get();

		int i = 0;
		p.id = q.value(i++).toInt();
		p. name = q.value(i++).toString();
		p.price = q.value(i++).toDouble();
		p.limitType = (LimitType)q.value(i++).toInt();
		p.value = q.value(i++).toInt();
		p.limitDays = q.value(i++).toInt();
		p.used = q.value(i++).toBool();
		p.vid_id = q.value(i++).toInt();

		item->set(p);
		items.append(item);
	}
	return items;
}

bool ServiceFetcher::saveSlot(Item* item, DBConn *conn)
{
	ServiceItem* cItem = (ServiceItem*)item;
	ServiceParam p = cItem->get();
	QString sql = "";
	QSqlQuery q(conn->qtDatabase());

	int i = 0;
	if (p.id != 0)
	{
		sql =
			"UPDATE service"
				" SET"
					" name = ?"
					", price = ?"
					", limit_type = ?"
					", value = ?"
					", limit_days = ?"
					", used = ?"
					", vid_id = ?"
				" WHERE id = ?";
		q.prepare(sql);

		q.bindValue(i++, p.name);
		q.bindValue(i++, p.price);
		q.bindValue(i++, p.limitType);
		q.bindValue(i++, p.value);
		q.bindValue(i++, p.limitDays);
		q.bindValue(i++, p.used);
		q.bindValue(i++, p.vid_id);
		q.bindValue(i++, p.id);
	}
	else
	{
		sql = "SELECT nextval('service_id_seq')";
		QSqlQuery seq(conn->qtDatabase());
		seq.exec(sql);
		if (seq.next())
		{
			qDebug() << "NEW ID = " << seq.value(0).toInt();
			p.id = seq.value(0).toInt();
			cItem->set(p);
		}
		sql =
			"INSERT INTO service("
					" id, name, price, limit_type, value, limit_days, used, vid_id)"
				" VALUES(?, ?, ?, ?, ?, ?, ?, ?)";
		q.prepare(sql);

		q.bindValue(i++, p.id);
		q.bindValue(i++, p.name);
		q.bindValue(i++, p.price);
		q.bindValue(i++, p.limitType);
		q.bindValue(i++, p.value);
		q.bindValue(i++, p.limitDays);
		q.bindValue(i++, p.used);
		q.bindValue(i++, p.vid_id);
	}

	return conn->executeQuery(q);
}
