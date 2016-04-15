#include <QDebug>

#include <model/card_item.h>
#include <db/db_service.h>

#include "card_fetcher.h"

CardFetcher::CardFetcher()
{

}

QList<Item*> CardFetcher::fetchSlot(DBConn* conn)
{
	QList<Item*> items;
	QString sql =
			"SELECT"
				" card.id"
				", card.name"
				", card.discont"
			" FROM card";

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
	return items;
}

bool CardFetcher::deleteSlot(Item *i, DBConn *conn)
{
	QString sql = "DELETE FROM card WHERE id = " + QString::number(i->getId());
	QSqlQuery q = conn->executeQuery(sql);
	return q.isActive();
}

bool CardFetcher::saveSlot(Item* item, DBConn *conn)
{
	CardItem* cItem = (CardItem*)item;

	int id = cItem->getId();
	QString name = cItem->getName();
	int disc = cItem->getDiscont();
	QString sql = "";
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

	return conn->executeQuery(q);
}
