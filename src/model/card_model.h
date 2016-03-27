#ifndef CardModel_H
#define CardModel_H

#include <model/item_model.h>

class CardItem;

class CardModel : public ItemModel
{
	Q_OBJECT
public:
	CardModel();
	virtual ~CardModel();

	QVariant data(const QModelIndex &index, int role) const;
};

#endif // CardModel_H
