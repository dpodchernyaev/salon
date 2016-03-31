
#include <model/item_model.h>

#include "item.h"

Item::Item()
{
	model = NULL;
}

Item::~Item()
{
}

QVariant Item::data(int role)
{
	QVariant res;
	if (model != NULL)
	{
		QModelIndex ind = model->getIndex(this);
		res = model->data(ind, role);
	}
	return res;
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
