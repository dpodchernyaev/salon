
#include <model/client_model.h>

#include "item_proxy_model.h"

ItemProxyModel::ItemProxyModel(ItemModel *model)
	:sourceModel(model)
{
	setSourceModel(model);
}

ItemProxyModel::~ItemProxyModel()
{

}

void ItemProxyModel::setFilter(ItemProxyModel::Filter f)
{
	filter = f;
	invalidateFilter();
}

bool ItemProxyModel::filterAcceptsRow(int source_row,
										const QModelIndex &source_parent) const
{
	bool res = QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
	QModelIndex sInd = sourceModel->index(source_row);
	if (!filter.display.isEmpty())
	{
		QString value = sInd.data(SearchRole).toString();
		if (!value.contains(filter.display))
		{
			res = false;
		}
	}
	return res;
}
