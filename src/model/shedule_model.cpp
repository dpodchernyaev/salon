
#include <model/model_factory.h>

#include <model/coach_item.h>
#include <model/hall_item.h>
#include <model/shedule_item.h>

#include <model/hall_model.h>
#include <model/coach_model.h>

#include <model/shedule_fetcher.h>

#include "shedule_model.h"

SheduleModel::SheduleModel() : ItemModel(new SheduleFetcher)
{

}

SheduleModel::~SheduleModel()
{

}

QList<SheduleItem*> SheduleModel::getItems(int day, int vidId) const
{
	QList<SheduleItem*> res;
	Q_FOREACH (Item* i, items)
	{
		SheduleItem* si = (SheduleItem*)i;
		if ( (si->getParam().day == day)
			 && (si->getParam().vid_id == vidId) )
		{
			res.append(si);
		}
	}

	return res;
}

QVariant SheduleModel::headerData(int section, Qt::Orientation orientation, int role) const
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
int SheduleModel::columnCount(const QModelIndex &parent) const
{
	/*
	 * 0 - День
	 * 1 - Время начала
	 * 2 - Время окончания
	 * 3 - Зал
	 * 4 - Тренер
	 * 5 - сумарный
	*/
	return 6;
}

bool SheduleModel::isValid(SheduleItem* item) const
{
	bool res = true;
	SheduleParam p = item->getParam();

	if ( (p.coach_id <= 0) || (p.hall_id <= 0) || (p.day < 0) )
	{
		return false;
	}

	if (p.bTime >= p.eTime)
	{
		return false;
	}

	Q_FOREACH (Item* i, items)
	{
		if (item == i)
		{
			continue;
		}

		SheduleItem* si = (SheduleItem*)i;
		SheduleParam cp = si->getParam();

		// если имеется пересечение по времени
		if ( ! ( (p.bTime < cp.bTime) && (p.eTime < cp.bTime)
			 || (p.bTime > cp.eTime) && (p.bTime > cp.bTime) ) )
		{
			if (p.day == cp.day)
			{
				if (p.hall_id == cp.hall_id)
				{
					res = false;
				}
				else if (p.coach_id == cp.coach_id)
				{
					res = false;
				}
			}
		}
	}
	return res;
}

bool SheduleModel::contains(const QDate &date, int vidId) const
{
	bool res = false;
	Q_FOREACH (Item* i, items)
	{
		SheduleItem* s = (SheduleItem*)i;
		if ( (s->getParam().day == date.dayOfWeek())
			 && (s->getParam().vid_id == vidId) )
		{
			res = true;
			break;
		}
	}
	return res;
}

bool SheduleModel::contains(const QDate &date) const
{
	bool res = false;
	Q_FOREACH (Item* i, items)
	{
		SheduleItem* s = (SheduleItem*)i;
		if (s->getParam().day == date.dayOfWeek())
		{
			res = true;
			break;
		}
	}
	return res;
}

QVariant SheduleModel::data(const QModelIndex &index, int role) const
{
	QVariant res;
	if (!index.isValid())
	{
		return res;
	}
	SheduleItem* item = (SheduleItem*)items.value(index.row());
	if (item == NULL)
	{
		return res;
	}
	int col = index.column();
	SheduleParam p = item->getParam();

	/*
	 * 0 - День
	 * 1 - Время начала
	 * 2 - Время окончания
	 * 3 - Зал
	 * 4 - Тренер
	*/

	if (role == Qt::DisplayRole)
	{
		if (col == 0)
		{
			res = ModelFactory::getDay(p.day);
		}
		else if (col == 1)
		{
			res = p.bTime;
		}
		else if (col == 2)
		{
			res = p.eTime;
		}
		else if (col == 3)
		{
			res = ModelFactory::getHall(p.hall_id);
		}
		else if (col == 4)
		{
			res = ModelFactory::getCoach(p.coach_id);
		}
		else if (col == 5)
		{
			res =
					ModelFactory::getDay(p.day)
					+ " --> " + p.bTime.toString(TIME_FORMAT)
					+ " --> " + p.eTime.toString(TIME_FORMAT)
					+ " --> " + ModelFactory::getHall(p.hall_id)
					+ " --> " + ModelFactory::getCoach(p.coach_id);
		}
	}
	else if (role == KeyRole)
	{
		res = item->getId();
	}
	else if (role == SortRole)
	{
		res = p.bTime;
	}
	else if (role == TimeSortRole)
	{
		res = p.bTime;
	}
	else if (role == SearchRole)
	{
		res = ModelFactory::getDay(p.day) + " " +
			  ModelFactory::getCoach(p.coach_id) + " " +
			  ModelFactory::getHall(p.hall_id);
	}

	return res;
}

