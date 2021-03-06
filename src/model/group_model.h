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
	bool isFull(GroupItem*) const;
	QList<GroupItem*> getItems(const QDate &date, int vidId) const;
	GroupItem* getItem(const QDateTime &dateTime, int hallId) const;

protected:
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	virtual int columnCount(const QModelIndex &parent) const;
};

#endif // GROUPMODEL_H
