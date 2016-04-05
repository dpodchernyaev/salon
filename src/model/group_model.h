#ifndef GROUPMODEL_H
#define GROUPMODEL_H

#include <model/item_model.h>

class GroupItem;

class GroupModel : public ItemModel
{
public:
	GroupModel();
	virtual ~GroupModel() {}

	int getCount(const QDate &date, int vidId) const;
	QList<GroupItem*> getActive(const QDate &date, int vidId) const;
	bool isActive(GroupItem*) const;
	QList<GroupItem*> getItems(const QDate &date, int vidId) const;
	GroupItem* getItem(const QDateTime &dateTime, int hallId) const;

	QVariant data(const QModelIndex &index, int role) const;
};

#endif // GROUPMODEL_H
