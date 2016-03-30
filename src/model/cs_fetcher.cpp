#include <QDebug>

#include <model/client_service_item.h>
#include <db/db_service.h>

#include "cs_fetcher.h"

CsFetcher::CsFetcher()
{
	clientId = 0;
}

//#include <unistd.h>

void CsFetcher::fetchSlot()
{
//	usleep(1000000);
	QList<Item*> items;
	QString sql =
			"SELECT"
				" id"
				", client_id"
				", service_id"
				", date"
			" FROM client_service"
			" WHERE id <> 0 AND client_id = " + QString::number(clientId);
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
		p.service_id = q.value(i++).toInt();
		p.date = q.value(i++).toDateTime();
		item->setParam(p);
		items.append(item);
	}
	Q_EMIT fetched(items);
}

void CsFetcher::saveSlot(Item* item)
{
	CsItem* cItem = (CsItem*)item;
	CsParam p = cItem->getParam();

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
			"UPDATE client_service"
				" SET"
					"client_id = ?"
					", service_id = ?"
					", date = ?"
				" WHERE id = ?";
		q.prepare(sql);
		q.bindValue(i++, p.client_id);
		q.bindValue(i++, p.service_id);
		q.bindValue(i++, p.date);
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
					" id, client_id, service_id, date)"
				" VALUES(?, ?, ?, ?)";
		q.prepare(sql);
		q.bindValue(i++, p.id);
		q.bindValue(i++, p.client_id);
		q.bindValue(i++, p.service_id);
		q.bindValue(i++, p.date);
	}

	bool res = conn->executeQuery(q);
	conn->commit();

	Q_EMIT saved(res);
}

void CsFetcher::fetchClient(int clientId)
{
	this->clientId = clientId;
	fetch();
}

void CsFetcher::deleteSlot(int id)
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

	conn->beginTransaction();
	QString sql = "UPDATE visit SET client_service_id = 0"
				  " WHERE client_service_id = " + QString::number(id);


	bool res = false;
	QSqlQuery q = conn->executeQuery(sql);
	if (q.isActive())
	{
		sql = "DELETE FROM client_service WHERE id = " + QString::number(id);
		q = conn->executeQuery(sql);
		if (q.isActive())
		{
			res = true;
		}
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
