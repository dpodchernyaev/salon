#include <QDebug>

#include <model/card_item.h>
#include <db/db_service.h>

#include "card_fetcher.h"

CardFetcher::CardFetcher()
{

}

void CardFetcher::fetchSlot()
{
	QList<Item*> items;
	QString sql =
			"SELECT"
				" card.id"
				", card.name"
				", card.discont"
			" FROM card";
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
		CardItem* item = new CardItem;
		int i = 0;
		item->setId(q.value(i++).toInt());
		item->setName(q.value(i++).toString());
		item->setDiscont(q.value(i++).toDouble());
		items.append(item);
	}
	Q_EMIT fetched(items);
}

void CardFetcher::saveSlot(Item* item)
{
	CardItem* cItem = (CardItem*)item;

	int id = cItem->getId();
	QString name = cItem->getName();
	int disc = cItem->getDiscont();

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
	if (id != 0)
	{
		sql =
			"UPDATE card"
				" SET"
					" name = ?"
					", discont = ?"
				" WHERE id = ?";
		q.prepare(sql);
		q.bindValue(i++, name);
		q.bindValue(i++, disc);
		q.bindValue(i++, id);
	}
	else
	{
		sql = "SELECT nextval('card_id_seq')";
		QSqlQuery seq(conn->qtDatabase());
		seq.exec(sql);
		if (seq.next())
		{
			qDebug() << "NEW ID = " << seq.value(0).toInt();
			id = seq.value(0).toInt();
			cItem->setId(id);
		}
		sql =
			"INSERT INTO card("
					" id, name, discont)"
				" VALUES(?, ?, ?)";
		q.prepare(sql);
		q.bindValue(i++, id);
		q.bindValue(i++, name);
		q.bindValue(i++, disc);
	}

	bool res = conn->executeQuery(q);
	conn->commit();

	Q_EMIT saved(res);
}
