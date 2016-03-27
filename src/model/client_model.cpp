
#include <QMessageBox>

#include <model/client_item.h>
#include <model/client_fetcher.h>
#include <variables.h>


#include "client_model.h"

static QHash<int, ClientItem*> clients;

ClientModel::ClientModel() : ItemModel(new ClientFetcher)
{
}

QVariant ClientModel::data(const QModelIndex &index, int role) const
{
	QVariant res;
	if (!index.isValid())
	{
		return res;
	}
	int row = index.row();
	if (row >= items.size())
	{
		return res;
	}

	ClientItem* item = (ClientItem*)items.value(row);
	ClientParam p = item->get();

	if (role == SearchRole)
	{
		res = p.surname + " " + p.name + " " + p.patronymic
				+ " (" + p.birthday.toString(DATE_FORMAT) + ")";
	}
	else if (role == Qt::DisplayRole)
	{
		res = p.surname + " " + p.name + " " + p.patronymic;
	}
	else if (role == SortRole)
	{
		res = p.surname + " " + p.name + " " + p.patronymic;
	}
	else if (role == KeyRole)
	{
		res = p.id;
	}

	return res;
}

ClientItem* ClientModel::getItem(const QModelIndex &index) const
{
	return (ClientItem*)items.value(index.row(), NULL);
}

QModelIndex ClientModel::getIndex(int id) const
{
	ClientItem* item = getItem(id);
	return index(items.indexOf(item));
}

ClientItem* ClientModel::getItem(int id) const
{
	return clients.value(id, NULL);
}

void ClientModel::fetched(QList<Item*> newItems)
{
	ItemModel::fetched(newItems);
	clients.clear();

	Q_FOREACH (Item* item, items)
	{
		ClientItem* cItem = (ClientItem*)item;
		clients.insert(cItem->getId(), cItem);
	}
}
