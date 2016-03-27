
#pragma once

#include <model/item_model.h>

class ServiceItem;

class ServiceModel : public ItemModel
{
public:
	ServiceModel();
	virtual ~ServiceModel();

	QVariant data(const QModelIndex &index, int role) const;
};
