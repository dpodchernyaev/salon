#include <QDebug>

#include <model/client_service_item.h>
#include <db/db_service.h>

#include "cs_fetcher.h"

CsFetcher::CsFetcher()
{
	lastId = 0;
}

//#include <unistd.h>

void CsFetcher::fetchSlot()
{
	mtx.lock();
	if (queue.isEmpty())
	{
		mtx.unlock();
		return;
	}

	lastId = queue.takeLast();
	queue.clear();
	mtx.unlock();

//	usleep(1000000);
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
			" WHERE id <> 0 AND client_id = " + QString::number(lastId);
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
	Q_EMIT fetched(items);
}

bool CsFetcher::saveSlot(Item* item, DBConn* conn)
{
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

void CsFetcher::fetchClient(int clientId)
{
	mtx.lock();
	queue.append(clientId);
	mtx.unlock();

	fetch();
}

bool CsFetcher::deleteSlot(Item *i, DBConn *conn)
{
	QString sql = "DELETE FROM client_service WHERE id = " +
				  QString::number(i->getId());
	QSqlQuery q = conn->executeQuery(sql);
	return q.isActive();
}
