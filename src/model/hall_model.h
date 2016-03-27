#ifndef HallModel_H
#define HallModel_H

#include <model/item_model.h>

class HallItem;

class HallModel : public ItemModel
{
	Q_OBJECT
public:
	HallModel();
	virtual ~HallModel();

	QVariant data(const QModelIndex &index, int role) const;
};

#endif // HallModel_H
