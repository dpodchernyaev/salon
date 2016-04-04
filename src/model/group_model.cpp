
#include <model/group_item.h>

#include <model/group_fetcher.h>

#include "group_model.h"

GroupModel::GroupModel() : ItemModel(new GroupFetcher)
{
}

GroupItem* GroupModel::getItem(const QDateTime &date, int hallId) const
{
	GroupItem* res = NULL;
	Q_FOREACH (Item* i, items)
	{
		GroupItem* gi = (GroupItem*)i;
		GroupParam p = gi->getParam();
		if (p.hall_id == hallId)
		{
			if (p.bdtime == date)
			{
				res = gi;
				break;
			}
		}
	}
	return res;
}

QVariant GroupModel::data(const QModelIndex &index, int role) const
{
	QVariant res;

	GroupItem* item = (GroupItem*)items.value(index.row());
	if (item == NULL)
	{
		return res;
	}

	if (role == KeyRole)
	{
		res = item->getId();
	}
	return res;
}
