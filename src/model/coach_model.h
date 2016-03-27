#ifndef COACHMODEL_H
#define COACHMODEL_H

#include <model/item_model.h>

class CoachItem;

class CoachModel : public ItemModel
{
	Q_OBJECT
public:
	CoachModel();
	virtual ~CoachModel();

	QVariant data(const QModelIndex &index, int role) const;
};

#endif // COACHMODEL_H
