
#include <model/card_item.h>
#include <model/card_fetcher.h>
#include "card_model.h"

CardModel::CardModel() : ItemModel(new CardFetcher)
{

}

CardModel::~CardModel()
{

}

QVariant CardModel::data(const QModelIndex &index, int role) const
{
	QVariant res;
	if (!index.isValid())
	{
		return res;
	}

	CardItem* item = (CardItem*)items.value(index.row());
	if (role == Qt::DisplayRole)
	{
		if (item != NULL)
		{
			res = item->getName() +
				  " (" + QString::number(item->getDiscont()) + ")";
		}
	}
	else if (role == Qt::EditRole
			|| role == SortRole
			|| role == SearchRole)
	{
		if (item != NULL)
		{
			res = item->getName();
		}
	}
	else if (role == KeyRole)
	{
		res = item->getId();
	}
	else if (role == DiscontRole)
	{
		res = item->getDiscont();
	}

	return res;
}
