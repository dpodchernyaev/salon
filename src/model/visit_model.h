#ifndef VISITMODEL_H
#define VISITMODEL_H

#include <model/item_model.h>

class VisitItem;

class VisitModel : public ItemModel
{
public:
	VisitModel();
	virtual ~VisitModel() {}

	void fetch(int id);

	VisitItem* getItemInGroup(int groupId) const;

	QVariant data(const QModelIndex &index, int role) const;
	int columnCount(const QModelIndex &parent) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // VISITMODEL_H
