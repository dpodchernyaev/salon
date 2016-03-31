
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

void ItemModel::save(Item* item)
{
	Q_EMIT lock(true);
	forSave = item;
	fetcher->save(forSave);
}

void ItemModel::clean()
{
	beginResetModel();
	qDeleteAll(items);
	items.clear();
	forSave = NULL;
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
			fetcher->deleteItem(id);
		}
	}
	delete item;
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
			QModelIndex ind = index(items.indexOf(forSave), 0);
			Q_EMIT dataChanged(ind, ind);
		}
	}
	Q_EMIT lock(false);
}
