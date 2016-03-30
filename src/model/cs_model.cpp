
#include <model/client_service_item.h>
#include <model/service_item.h>
#include <model/cs_fetcher.h>
#include <model/model_factory.h>

#include "cs_model.h"

CsModel::CsModel() : ItemModel(new CsFetcher)
{
	clientId = 0;
}

CsModel::~CsModel()
{

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
	int id = i->getParam().id;
	int service_id = i->getParam().service_id;

	ItemModel* extModel = ModelFactory::getInstance()->getModel(SERVICE);
	ServiceItem* sItem = (ServiceItem*)extModel->getItem(service_id);
	ServiceParam sParam = sItem->get();

	int col = index.column();
	if (role == Qt::DisplayRole)
	{
		if (col == 0)
		{
			res = sParam.name;
		}
		else if (col == 1)
		{
			res = i->getParam().date.toString(DATE_FORMAT);
		}
		else if (col == 2)
		{
			if (sParam.limitType != LT_COUNT)
			{
				res = i->getParam().date.addDays(sParam.limitDays).toString(DATE_FORMAT);
			}
			else
			{
				res = "Нет";
			}
		}
		else if (col == 3)
		{
			if (sParam.limitType != LT_DATE)
			{
				res = sParam.value;
			}
			else
			{
				res = "Нет";
			}
		}
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
