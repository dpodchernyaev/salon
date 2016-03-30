#ifndef SHEDULEMODEL_H
#define SHEDULEMODEL_H

#include <model/item_model.h>

class SheduleModel : public ItemModel
{
	Q_OBJECT
public:
	SheduleModel();
	virtual ~SheduleModel();

	QVariant data(const QModelIndex &index, int role) const;
	int columnCount(const QModelIndex &parent) const;

	static QString getDay(int id);
	static QString getHall(int id);
	static QString getCoach(int id);
};

#endif // SHEDULEMODEL_H
