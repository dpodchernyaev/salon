
#include <model/client_model.h>

#include "client_proxy_model.h"

ClientProxyModel::ClientProxyModel(ClientModel *model)
	:sourceModel(model)
{
	setSourceModel(model);
}

ClientProxyModel::~ClientProxyModel()
{

}

void ClientProxyModel::setFilter(ClientProxyModel::Filter f)
{
	filter = f;
	invalidateFilter();
}

bool ClientProxyModel::filterAcceptsRow(int source_row,
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
