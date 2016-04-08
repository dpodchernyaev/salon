
#include <model/group_item.h>

#include <model/model_factory.h>
#include <model/group_fetcher.h>

#include "group_model.h"

GroupModel::GroupModel() : ItemModel(new GroupFetcher)
{
}

int GroupModel::getCount(const QDate &date, int vidId) const
{
	int res = 0;
	Q_FOREACH (Item* i, items)
	{
		GroupItem* gi = (GroupItem*)i;
		GroupParam p = gi->getParam();
		if ( (p.vid_id == vidId) && (p.bdtime.date() == date) )
		{
			res++;
		}
	}
	return res;
}

QList<GroupItem*> GroupModel::getItems(const QDate &date, int vidId) const
{
	QList<GroupItem*> res;
	Q_FOREACH (Item* i, items)
	{
		GroupItem* gi = (GroupItem*)i;
		GroupParam p = gi->getParam();
		if ( (p.vid_id == vidId) && (p.bdtime.date() == date) )
		{
			res.append(gi);
		}
	}
	return res;
}

bool GroupModel::isFull(GroupItem* gi) const
{
	bool res = false;
	GroupParam p = gi->getParam();
	int max = ModelFactory::getHallCnt(p.hall_id);
	if (p.cnt >= max)
	{
		res = true;
	}
	return res;
}

QList<GroupItem*> GroupModel::getActive(const QDate &date, int vidId) const
{
	QList<GroupItem*> res;
	Q_FOREACH (Item* i, items)
	{
		GroupItem* gi = (GroupItem*)i;
		GroupParam p = gi->getParam();
		if ( (p.vid_id == vidId) && (p.bdtime.date() == date) )
		{
			int max = ModelFactory::getHallCnt(p.hall_id);
			if (max > p.cnt)
			{
				res.append(gi);
			}
		}
	}
	return res;
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

QVariant GroupModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	QVariant res = ItemModel::headerData(section, orientation, role);

	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		if (section == 0)
		{
			res = "День";
		}
		else if (section == 1)
		{
			res = "Начало";
		}
		else if (section == 2)
		{
			res = "Конец";
		}
		else if (section == 3)
		{
			res = "Зал";
		}
		else if (section == 4)
		{
			res = "Тренер";
		}
	}
	return res;
}

int GroupModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return 5;
}

QVariant GroupModel::data(const QModelIndex &index, int role) const
{
	QVariant res;

	GroupItem* item = (GroupItem*)items.value(index.row());
	if (item == NULL)
	{
		return res;
	}

	GroupParam gp = item->getParam();

	if (role == Qt::DisplayRole)
	{
		if (index.column() == 0)
		{
			res = ModelFactory::getDay(gp.bdtime.date().dayOfWeek());
		}
		else if (index.column() == 1)
		{
			res = gp.bdtime.time();
		}
		else if (index.column() == 2)
		{
			res = gp.etime;
		}
		else if (index.column() == 3)
		{
			res = ModelFactory::getHall(gp.hall_id);
		}
		else if (index.column() == 4)
		{
			res = ModelFactory::getCoach(gp.coach_id);
		}
	}
	else if (role == KeyRole)
	{
		res = item->getId();
	}
	else if (role == SortRole)
	{
		res = gp.bdtime.time();
	}
	return res;
}
