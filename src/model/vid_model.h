
#pragma once

#include <model/item_model.h>

class VidModel : public ItemModel
{
public:
	VidModel();
	virtual ~VidModel();

	QVariant data(const QModelIndex &index, int role) const;
};
