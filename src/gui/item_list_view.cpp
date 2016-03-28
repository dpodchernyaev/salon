
#include <QDebug>

#include <model/model_factory.h>
#include <model/client_item.h>
#include <model/client_model.h>
#include <model/client_proxy_model.h>

#include "client_list_view.h"

ClientListView::ClientListView()
{
	model = (ClientModel*)ModelFactory::getInstance()->getModel(CLIENT);
	proxyModel = new ClientProxyModel(model);
	proxyModel->setSortRole(SortRole);
	proxyModel->setDynamicSortFilter(true);
	proxyModel->sort(0);
	setModel(proxyModel);
}

ClientListView::~ClientListView()
{
	delete proxyModel;
	delete model;
}

ClientModel *ClientListView::getSourceModel() const
{
	return model;
}

ClientProxyModel* ClientListView::getProxyModel() const
{
	return proxyModel;
}

QModelIndex ClientListView::mapFromSource(const QModelIndex &index) const
{
	if (!index.isValid())
	{
		return QModelIndex();
	}
	return proxyModel->mapFromSource(index);
}

void ClientListView::currentChanged(const QModelIndex &current,
									const QModelIndex &previous)
{
	QListView::currentChanged(current, previous);
	QModelIndex sInd = proxyModel->mapToSource(current);
	ClientItem* item = model->getItem(sInd);
	Q_EMIT currentChanged(item);
}
