
#include <QMessageBox>

#include <model/fetcher.h>
#include <model/item.h>

#include "item_model.h"

ItemModel::ItemModel(Fetcher* fetcher) : fetcher(fetcher)
{
	connect(fetcher, SIGNAL(fetched(QList<Item*>)),
			this, SLOT(fetched(QList<Item*>)));
	connect(fetcher, SIGNAL(saved(bool)),
			this, SLOT(saved(bool)));
}


int ItemModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return items.size();
}

Item* ItemModel::getItem(const QModelIndex &ind) const
{
	int r = ind.row();
	return items.value(r);
}


void ItemModel::fetch()
{
	Q_EMIT lock(true);
	fetcher->fetch();
}

int ItemModel::indexOf(int id) const
{
	int res = -1;
	for (int i = 0; i < rowCount(QModelIndex()); i++)
	{
		int key = index(i).data(KeyRole).toInt();
		if (key == id)
		{
			res = i;
			break;
		}
	}
	return res;
}

void ItemModel::save(Item* item)
{
	if (!items.contains(item))
	{
		beginInsertRows(QModelIndex(), items.size(), items.size());
		items.append(item);
		endInsertRows();
	}
	else
	{
		QModelIndex ind = index(items.indexOf(item));
		Q_EMIT dataChanged(ind, ind);
	}

	Q_EMIT lock(true);
	Item::addToHash(item);
	fetcher->save(item);
}

void ItemModel::fetched(QList<Item*> newItems)
{
	beginResetModel();
	qDeleteAll(items);
	items.clear();
	items = newItems;
	endResetModel();

	Q_FOREACH (Item* i, items)
	{
		Item::addToHash(i);
		i->setModel(this);
	}

	Q_EMIT lock(false);
	Q_EMIT modelRestored();
}

void ItemModel::saved(bool f)
{
	if (f == false)
	{
		QMessageBox::critical(NULL, "Ошибка", "Ошибка сохранения");
	}
	else
	{
		QMessageBox::information(NULL, "Сохранено",
							"Сохранение выполнено успешно", QMessageBox::Ok);
	}
	Q_EMIT lock(false);
}
