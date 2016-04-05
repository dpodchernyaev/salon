
#include <model/model_factory.h>
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
	if (cached.size() > 50)
	{
		Q_FOREACH (int id, cached.keys().toSet())
		{
			qDeleteAll(cached.value(id));
		}
		cached.clear();
	}

	// удаление старых объектов из кэш
	if (cached.contains(clientId) == true)
	{
		qDeleteAll(cached.take(clientId));
	}

	// добавление текущих
	QList<Item*> tmpLst;
	Q_FOREACH (Item* i, items)
	{
		CsItem* csi = (CsItem*)i;
		CsItem* ncsi = new CsItem;
		*ncsi = *csi;
		tmpLst.append(ncsi);
	}
	cached.insert(clientId, tmpLst);
	tmpLst.clear();

	// изымаем старые записи, если есть. Если нет, то выгружаем из БД
	Q_EMIT lock(true);
	if (cached.contains(id) == true)
	{
		tmpLst = cached.take(id);
		fetched(tmpLst);
	}
	else
	{
		CsFetcher* f = (CsFetcher*)fetcher;
		f->fetchClient(id);
	}
	clientId = id;
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
		int colNum = 0;
		if (col == colNum++)
		{
			res = ModelFactory::getVid(p.vid_id);
		}
		else if (col == colNum++)
		{
			res = p.name;
		}
		else if (col == colNum++)
		{
			res = p.date.toString(DATE_FORMAT);
		}
		else if (col == colNum++)
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
		else if (col == colNum++)
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
	else if (role == KeyRole)
	{
		return p.id;
	}
	return res;
}

int CsModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED (parent);
	// 0 - вид
	// 1 - название
	// 2 - дата покупки
	// 3 - срок истекания
	// 4 - остаток
	return 5;
}

QVariant CsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	QVariant res = ItemModel::headerData(section, orientation, role);
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		if (section == 0)
		{
			res = "Вид";
		}
		else if (section == 0)
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

bool CsModel::isActive(const CsParam &p) const
{
	bool f1 = true;
	bool f2 = true;


	if ( (p.limit_type == LT_DATE) || (p.limit_type == LT_DATE_COUNT) )
	{
		QDateTime currDate = QDateTime::currentDateTime();
		QDateTime limitDate = p.date;
		limitDate = limitDate.addDays(p.limit_days);
		f1 = currDate.date() <= limitDate.date();
	}

	if ( (p.limit_type == LT_COUNT) || (p.limit_type == LT_DATE_COUNT) )
	{
		f2 = p.limit_value > 0;
	}

	return f1 && f2;
}

bool CsModel::isActive(const QModelIndex& ind) const
{
	CsItem* i = (CsItem*)getItem(ind);
	CsParam p = i->getParam();

	return isActive(p);
}
