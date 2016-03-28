
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
	Q_EMIT lock(true);
	forSave = item;
	fetcher->save(forSave);
}

void ItemModel::removeItem(Item* item)
{
	int row = items.indexOf(item);
	if (row != -1)
	{
		beginRemoveRows(QModelIndex(), row, row);
		items.removeAll(item);
		endRemoveRows();
	}
}

QModelIndex ItemModel::getIndex(Item* item) const
{
	int row = items.indexOf(item);
	return index(row);
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

		Q_ASSERT (forSave != NULL);

		if (!items.contains(forSave))
		{
			beginInsertRows(QModelIndex(), items.size(), items.size());
			items.append(forSave);
			endInsertRows();
		}
		else
		{
			QModelIndex ind = index(items.indexOf(forSave));
			Q_EMIT dataChanged(ind, ind);
		}
		Item::addToHash(forSave);
	}
	Q_EMIT lock(false);
}
