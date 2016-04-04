#ifndef SHEDULEMODEL_H
#define SHEDULEMODEL_H

#include <model/item_model.h>

class SheduleItem;

class SheduleModel : public ItemModel
{
	Q_OBJECT
public:
	SheduleModel();
	virtual ~SheduleModel();

	QVariant data(const QModelIndex &index, int role) const;
	int columnCount(const QModelIndex &parent) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;

	bool contains(const QDate &date) const;

	bool isValid(SheduleItem* item) const;
};

#endif // SHEDULEMODEL_H
