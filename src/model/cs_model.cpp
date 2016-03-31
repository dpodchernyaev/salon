
#include <model/client_service_item.h>
#include <model/cs_fetcher.h>

#include "cs_model.h"

CsModel::CsModel() : ItemModel(new CsFetcher)
{
	clientId = 0;
}

CsModel::~CsModel()
{

}

double CsModel::getSumm() const
{
	double summ = 0;
	Q_FOREACH (Item* i, items)
	{
		CsItem* cs = (CsItem*)i;
		summ += cs->getParam().summ;
	}

	return summ;
}

void CsModel::fetchForClient(int id)
{
	Q_EMIT lock(true);
	clientId = id;
	CsFetcher* f = (CsFetcher*)fetcher;
	f->fetchClient(id);
}

QVariant CsModel::data(const QModelIndex &index, int role) const
{
	QVariant res;
	if (!index.isValid())
	{
		return res;
	}

	CsItem* i = (CsItem*)getItem(index);
	CsParam p = i->getParam();

	int col = index.column();
	if (role == Qt::DisplayRole)
	{
		if (col == 0)
		{
			res = p.name;
		}
		else if (col == 1)
		{
			res = p.date.toString(DATE_FORMAT);
		}
		else if (col == 2)
		{
			if (p.limit_type != LT_COUNT)
			{
				res = p.date.addDays(p.limit_days).toString(DATE_FORMAT);
			}
			else
			{
				res = "Нет";
			}
		}
		else if (col == 3)
		{
			if (p.limit_type != LT_DATE)
			{
				res = p.limit_value;
			}
			else
			{
				res = "Нет";
			}
		}
	}
	else if (role == IsActive)
	{
		return isActive(index);
	}
	else if (role == SortRole)
	{
		return p.date;
	}
	return res;
}

int CsModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED (parent);
	// 0 - название
	// 1 - дата покупки
	// 2 - срок истекания
	// 3 - остаток
	return 4;
}

QVariant CsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	QVariant res = ItemModel::headerData(section, orientation, role);
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		if (section == 0)
		{
			res = "Название";
		}
		else if (section == 1)
		{
			res = "Дата покупки";
		}
		else if (section == 2)
		{
			res = "Истекает (дн.)";
		}
		else if (section == 3)
		{
			res = "Остаток";
		}
	}
	return res;
}

bool CsModel::isActive(const QModelIndex& ind) const
{
	bool res = true;

	CsItem* i = (CsItem*)getItem(ind);
	CsParam p = i->getParam();

	if (p.limit_type != LT_COUNT)
	{
		QDateTime currDate = QDateTime::currentDateTime();
		QDateTime limitDate = p.date;
		limitDate = limitDate.addDays(p.limit_days);
		res = currDate.date() <= limitDate.date();
	}

	return res;
}
