#ifndef GROUPMODEL_H
#define GROUPMODEL_H

#include <model/item_model.h>

class GroupItem;

class GroupModel : public ItemModel
{
public:
	GroupModel();
	virtual ~GroupModel() {}

	GroupItem* getItem(const QDateTime &date, int hallId) const;

	QVariant data(const QModelIndex &index, int role) const;
};

#endif // GROUPMODEL_H
