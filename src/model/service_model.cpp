
#include <model/service_item.h>
#include <model/service_fetcher.h>

#include "service_model.h"

ServiceModel::ServiceModel() : ItemModel(new ServiceFetcher)
{

}

ServiceModel::~ServiceModel()
{

}

QVariant ServiceModel::data(const QModelIndex &index, int role) const
{
	QVariant res;
	if (!index.isValid())
	{
		return res;
	}

	ServiceItem* item = (ServiceItem*)items.value(index.row());
	if (role == Qt::EditRole
			|| role == Qt::DisplayRole
			|| role == SortRole
			|| role == SearchRole)
	{
		if (item != NULL)
		{
			res = item->get().name;
		}
	}
	else if (role == KeyRole)
	{
		res = item->getId();
	}
	else if (role == PriceRole)
	{
		res = item->get().price;
	}

	return res;
}
