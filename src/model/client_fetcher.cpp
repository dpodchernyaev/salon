
#include <QThread>
#include <QDebug>

#include <model/client_item.h>
#include <db/db_service.h>

#include "client_fetcher.h"

ClientFetcher::~ClientFetcher()
{

}



bool ClientFetcher::checkClientService(int client_id, DBConn* conn)
{
	if (client_id == 0)
	{
		return false;
	}

	QString sql = "SELECT count(*) FROM client_card WHERE client_id = ?";
	bool res = false;
	QSqlQuery q(conn->qtDatabase());
	q.prepare(sql);
	q.bindValue(0, client_id);

	if (conn->executeQuery(q) && q.isActive() && q.next())
	{
		res = q.value(0).toInt() > 0;
	}
	return res;
}


bool ClientFetcher::saveCard(Item* item, DBConn* conn)
{
	bool res = false;

	ClientItem* cItem = (ClientItem*)item;

	ClientParam p = cItem->get();

	if (p.card_id == 0)
	{
		return true;
	}

	QString sql = "";
	int i = 0;

	bool oldCard = checkClientService(p.id, conn);

	QSqlQuery q(conn->qtDatabase());
	if (oldCard == true)
	{
		sql =
			"UPDATE client_card"
				" SET"
					" number = ?"
					", card_id = ?"
				" WHERE client_id = ?";
		q.prepare(sql);
		q.bindValue(i++, p.cardNumber);
		q.bindValue(i++, p.card_id);
		q.bindValue(i++, p.id);
	}
	else
	{
		sql = "INSERT INTO client_card("
					"client_id, card_id, number)"
				" VALUES(?, ?, ?)";
		q.prepare(sql);
		q.bindValue(i++, p.id);
		q.bindValue(i++, p.card_id);
		q.bindValue(i++, p.cardNumber);
	}

	res = conn->executeQuery(q);
	return res;
}

void ClientFetcher::deleteSlot(int id)
{
	DBService* service = DBService::getInstance();
	DBConn* conn = service->getConnection();
	if (!conn->isConnected())
	{
		qCritical() << Q_FUNC_INFO << "Ошибка подключания к БД";
		Q_EMIT deleted(false);
		return;
	}

	if (id <= 0)
	{
		qCritical() << Q_FUNC_INFO << "Неверный идентификатор клиента";
		Q_EMIT deleted(false);
		return;
	}

	bool res = true;
	conn->beginTransaction();
	QString sql = "UPDATE client_service SET"
				  " client_id = 0"
				  " WHERE client_id = " + QString::number(id);
	QSqlQuery q = conn->executeQuery(sql);
	if (!q.isActive())
	{
		res = false;
	}

	if (res == true)
	{
		QString sql = "DELETE FROM client WHERE id = " + QString::number(id);
		q = conn->executeQuery(sql);

		if (!q.isActive())
		{
			res = false;
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

void ClientFetcher::saveSlot(Item* item)
{
	ClientItem* cItem = (ClientItem*)item;

	ClientParam p = cItem->get();


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
			"UPDATE client"
				" SET"
					" name = ?"
					", surname = ?"
					", patronymic = ?"
					", email = ?"
					", birthday = ?"
					", phone = ?"
					", photo = ?"
					", info = ?"
					", notify = ?"
				" WHERE id = ?";
		q.prepare(sql);
		q.bindValue(i++, p.name);
		q.bindValue(i++, p.surname);
		q.bindValue(i++, p.patronymic);
		q.bindValue(i++, p.email);
		q.bindValue(i++, p.birthday);
		q.bindValue(i++, p.phone);
		q.bindValue(i++, p.photo);
		q.bindValue(i++, p.info);
		q.bindValue(i++, p.notify);
		q.bindValue(i++, p.id);
	}
	else
	{
		sql = "SELECT nextval('client_id_seq')";
		QSqlQuery seq(conn->qtDatabase());
		seq.exec(sql);
		if (seq.next())
		{
			qDebug() << "NEW ID = " << seq.value(0).toInt();
			p.id = seq.value(0).toInt();
			cItem->set(p);
		}
		sql =
			"INSERT INTO client("
					" id, name, surname, patronymic, email, birthday, phone"
					", photo, info, notify)"
				" VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
		q.prepare(sql);
		q.bindValue(i++, p.id);
		q.bindValue(i++, p.name);
		q.bindValue(i++, p.surname);
		q.bindValue(i++, p.patronymic);
		q.bindValue(i++, p.email);
		q.bindValue(i++, p.birthday);
		q.bindValue(i++, p.phone);
		q.bindValue(i++, p.photo);
		q.bindValue(i++, p.info);
		q.bindValue(i++, p.notify);
	}

	bool res = conn->executeQuery(q);

	if (res == true)
	{
		res = saveCard(item, conn);
	}

	if (res == true)
	{
		conn->commit();
	}
	else
	{
		conn->rollback();
	}

	Q_EMIT saved(res);
}

void ClientFetcher::fetchSlot()
{
	QList<Item*> items;
	QString sql =
			"SELECT"
				" client.id"
				", client.name"
				", client.surname"
				", client.patronymic"
				", client.email"
				", client.birthday"
				", client.phone"
				", client.photo"
				", client.info"
				", client.notify"
				", client_card.number"
				", client_card.card_id"
			" FROM client"
				" LEFT JOIN client_card"
				" ON client.id = client_card.client_id"
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
		ClientParam param;

		int i = 0;
		param.id = q.value(i++).toInt();
		param.name = q.value(i++).toString();
		param.surname = q.value(i++).toString();
		param.patronymic = q.value(i++).toString();
		param.email = q.value(i++).toString();
		param.birthday = q.value(i++).toDateTime();
		param.phone = q.value(i++).toString();
		param.photo = q.value(i++).toString();
		param.info = q.value(i++).toString();
		param.notify = q.value(i++).toBool();
		param.cardNumber = q.value(i++).toInt();
		param.card_id = q.value(i++).toInt();

		ClientItem* item = new ClientItem;
		item->set(param);
		items.append(item);
	}
	Q_EMIT fetched(items);
}

