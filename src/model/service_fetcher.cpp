#include <QDebug>

#include <model/service_item.h>
#include <db/db_service.h>

#include "service_fetcher.h"

ServiceFetcher::ServiceFetcher()
{

}

void ServiceFetcher::deleteSlot(int id)
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

	bool res = false;
	conn->beginTransaction();
	QString sql = "DELETE FROM service WHERE id = " + QString::number(id);
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

void ServiceFetcher::fetchSlot()
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
			" FROM service"
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

		item->set(p);
		items.append(item);
	}
	Q_EMIT fetched(items);
}

void ServiceFetcher::saveSlot(Item* item)
{
	ServiceItem* cItem = (ServiceItem*)item;

	ServiceParam p = cItem->get();

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
			"UPDATE service"
				" SET"
					" name = ?"
					", price = ?"
					", limit_type = ?"
					", value = ?"
					", limit_days = ?"
					", used = ?"
				" WHERE id = ?";
		q.prepare(sql);

		q.bindValue(i++, p.name);
		q.bindValue(i++, p.price);
		q.bindValue(i++, p.limitType);
		q.bindValue(i++, p.value);
		q.bindValue(i++, p.limitDays);
		q.bindValue(i++, p.used);
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
					" id, name, price, limit_type, value, limit_days, used)"
				" VALUES(?, ?, ?, ?, ?, ?, ?)";
		q.prepare(sql);

		q.bindValue(i++, p.id);
		q.bindValue(i++, p.name);
		q.bindValue(i++, p.price);
		q.bindValue(i++, p.limitType);
		q.bindValue(i++, p.value);
		q.bindValue(i++, p.limitDays);
		q.bindValue(i++, p.used);
	}

	bool res = conn->executeQuery(q);
	conn->commit();

	Q_EMIT saved(res);
}
