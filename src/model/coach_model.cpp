
#include <model/coach_item.h>
#include <model/coach_fetcher.h>
#include "coach_model.h"

CoachModel::CoachModel() : ItemModel(new CoachFetcher)
{

}

CoachModel::~CoachModel()
{

}

QVariant CoachModel::data(const QModelIndex &index, int role) const
{
	QVariant res;
	if (!index.isValid())
	{
		return res;
	}

	CoachItem* item = (CoachItem*)items.value(index.row());
	if (item == NULL)
	{
		return QVariant();
	}

	if (role == Qt::EditRole
			|| role == Qt::DisplayRole
			|| role == SortRole
			|| role == SearchRole)
	{
		res = item->getName();
	}
	else if (role == KeyRole)
	{
		res = item->getId();
	}
	return res;
}
