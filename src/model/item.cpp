
#include <model/item_model.h>

#include "item.h"

QHash<QString, Item*> Item::hashItems;

Item::Item()
{
	model = NULL;
}

Item::~Item()
{
	if (model != NULL)
	{
		model->removeItem(this);
	}

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

int Item::getId() const
{
	int id = 0;
	Item* curr = const_cast<Item*>(this);
	QVariant value = model->data(model->getIndex(curr), KeyRole);
	if (!value.isNull() && value.isValid())
	{
		id = value.toInt();
	}
	return id;
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
