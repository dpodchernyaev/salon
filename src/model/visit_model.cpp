
#include <model/visit_item.h>

#include <model/model_factory.h>
#include <model/cs_model.h>
#include <model/client_service_item.h>
#include <model/visit_fetcher.h>

#include "visit_model.h"

VisitModel::VisitModel() : ItemModel(new VisitFetcher)
{

}

VisitItem* VisitModel::getItemInGroup(int groupId) const
{
	VisitItem* res = NULL;
	Q_FOREACH (Item* i, items)
	{
		VisitItem* vi = (VisitItem*)i;
		if (vi->getParam().vgroup_id == groupId)
		{
			res = vi;
			break;
		}
	}

	return res;
}

void VisitModel::fetch(int id)
{
	VisitFetcher* vf = (VisitFetcher*)fetcher;
	vf->fetchForClient(id);
}

int VisitModel::columnCount(const QModelIndex &parent) const
{
	// 0 - тип занятий
	// 1 - времмя
	// 2 - зал
	return 3;
}

QVariant VisitModel::data(const QModelIndex &index, int role) const
{
	QVariant res;

	if (!index.isValid())
	{
		return res;
	}

	Item* item = getItem(index);
	VisitItem* vItem = (VisitItem*)item;
	VisitParam param = vItem->getParam();

	if (role == Qt::DisplayRole)
	{
		if (index.column() == 0)
		{
			res = param.info;
		}
		else if (index.column() == 1)
		{
			res = param.dtime;
		}
		else if (index.column() == 2)
		{
			res = ModelFactory::getHallByGroup(param.vgroup_id);
		}
	}
	else if (role == SortRole)
	{
		res = param.dtime;
	}
	return res;
}

QVariant VisitModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	QVariant res = ItemModel::headerData(section, orientation, role);
	if (orientation == Qt::Horizontal)
	{
		if (role == Qt::DisplayRole)
		{
			if (section == 0)
			{
				res = "Вид";
			}
			else if (section == 1)
			{
				res = "Время";
			}
			else if (section == 2)
			{
				res = "Зал";
			}
		}
	}
	return res;
}
