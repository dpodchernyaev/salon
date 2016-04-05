
#include <model/vid_item.h>
#include <model/vid_model.h>
#include <model/vid_fetcher.h>
#include "vid_model.h"

VidModel::VidModel() : ItemModel(new VidFetcher)
{

}

VidModel::~VidModel()
{

}

QVariant VidModel::data(const QModelIndex &index, int role) const
{
	QVariant res;
	if (!index.isValid())
	{
		return res;
	}

	VidItem* item = (VidItem*)items.value(index.row());
	if (item == NULL)
	{
		return QVariant();
	}

	if (role == Qt::EditRole
			|| role == Qt::DisplayRole
			|| role == SortRole
			|| role == SearchRole)
	{
		res = item->getParam().name;
	}
	else if (role == KeyRole)
	{
		res = item->getId();
	}
	return res;
}
