
#include <model/item_model.h>

#include "item.h"

QHash<QString, Item*> Item::hashItems;

Item::Item()
{
	model = NULL;
}

Item::~Item()
{
	Q_FOREACH (QString str, hashItems.keys())
	{
		Item* i = hashItems.value(str);
		if (i == this)
		{
			hashItems.remove(str);
			break;
		}
	}
}

void Item::setModel(ItemModel* model)
{
	this->model = model;
}

void Item::save()
{
	Q_ASSERT (model != NULL);
	model->save(this);
}

Item* Item::getItem(const QString &hash)
{
	return hashItems.value(hash, NULL);
}

Item* Item::addToHash(Item* item)
{
	hashItems.insert(item->hash(), item);
}
