
#include <QMessageBox>

#include <model/fetcher.h>
#include <model/item.h>

#include "item_model.h"

ItemModel::ItemModel(Fetcher* fetcher) : fetcher(fetcher)
{
	connect(fetcher, SIGNAL(fetched(QList<Item*>)),
			this, SLOT(fetched(QList<Item*>)));
	connect(fetcher, SIGNAL(saved(Item*, bool)),
			this, SLOT(saved(Item*, bool)));
	connect(fetcher, SIGNAL(deleted(bool)),
			this, SLOT(deleted(bool)));
}


int ItemModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return items.size();
}

int ItemModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return 1;
}

Fetcher* ItemModel::getFetcher() const
{
	return fetcher;
}

Item* ItemModel::getItem(int id) const
{
	Item* res = NULL;
	int row = indexOf(id);
	res = items.value(row);
	return res;
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

int ItemModel::indexOf(Item* item) const
{
	int res = items.indexOf(item);
	return res;
}

int ItemModel::indexOf(int id) const
{
	int res = -1;
	for (int i = 0; i < rowCount(QModelIndex()); i++)
	{
		int key = index(i, 0).data(KeyRole).toInt();
		if (key == id)
		{
			res = i;
			break;
		}
	}
	return res;
}

void ItemModel::add(Item* item)
{
	beginInsertRows(QModelIndex(), items.size(), items.size());
	items.append(item);
	item->setModel(this);
	endInsertRows();
}

void ItemModel::save(Item* item)
{
	Q_EMIT lock(true);
	fetcher->save(item);
}

void ItemModel::clean()
{
	beginResetModel();
	qDeleteAll(items);
	items.clear();
	endResetModel();
}

void ItemModel::deleteItem(Item* item)
{
	int row = items.indexOf(item);

	if (row != -1)
	{
		int id = item->getId();
		beginRemoveRows(QModelIndex(), row, row);
		items.removeAll(item);
		endRemoveRows();

		if (id > 0)
		{
			Q_EMIT lock(true);
			fetcher->deleteItem(item);
		}
		else
		{
			delete item;
		}
	}
	else
	{
		delete item;
	}
}

QModelIndex ItemModel::getIndex(Item* item) const
{
	int row = items.indexOf(item);
	return index(row, 0);
}

void ItemModel::fetched(QList<Item*> newItems)
{
	clean();

	beginResetModel();
	items = newItems;
	endResetModel();

	Q_FOREACH (Item* i, items)
	{
		i->setModel(this);
	}

	Q_EMIT lock(false);
	Q_EMIT modelRestored();
}

void ItemModel::deleted(bool f)
{
	if (f == false)
	{
		QMessageBox::critical(NULL, "Ошибка", "Ошибка удаления");
	}
	else
	{
		QMessageBox::information(NULL, "Удалено",
							"Удаление выполнено успешно", QMessageBox::Ok);
	}
	Q_EMIT lock(false);
}

void ItemModel::saved(Item* item, bool f)
{
	if (f == false)
	{
		QMessageBox::critical(NULL, "Ошибка", "Ошибка сохранения");
	}
	else
	{
		Q_ASSERT (item != NULL);

		if (!items.contains(item))
		{
			beginInsertRows(QModelIndex(), items.size(), items.size());
			item->setModel(this);
			items.append(item);
			endInsertRows();
		}
		else
		{
			QModelIndex ind = index(items.indexOf(item), 0);
			Q_EMIT dataChanged(ind, ind);
		}
	}
	Q_EMIT lock(false);
}
