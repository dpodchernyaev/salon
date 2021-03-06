
#include <model/hall_item.h>
#include <model/hall_fetcher.h>
#include "hall_model.h"

HallModel::HallModel() : ItemModel(new HallFetcher)
{

}

HallModel::~HallModel()
{

}

QVariant HallModel::data(const QModelIndex &index, int role) const
{
	QVariant res;
	if (!index.isValid())
	{
		return res;
	}

	HallItem* item = (HallItem*)items.value(index.row());
	if (role == Qt::EditRole
			|| role == Qt::DisplayRole
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
		return item->getId();
	}

	return res;
}
