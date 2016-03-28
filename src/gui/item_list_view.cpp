
#include <QDebug>

#include <model/model_factory.h>
#include <model/client_item.h>
#include <model/client_model.h>
#include <model/item_proxy_model.h>

#include "item_list_view.h"

ItemListView::ItemListView(ItemModel* model) : model(model)
{
	proxyModel = new ItemProxyModel(model);
	proxyModel->setSortRole(SortRole);
	proxyModel->setDynamicSortFilter(true);
	proxyModel->sort(0);
	setModel(proxyModel);
}

ItemListView::~ItemListView()
{
	delete proxyModel;
}

ItemModel *ItemListView::getSourceModel() const
{
	return model;
}

ItemProxyModel* ItemListView::getProxyModel() const
{
	return proxyModel;
}

QModelIndex ItemListView::mapFromSource(const QModelIndex &index) const
{
	if (!index.isValid())
	{
		return QModelIndex();
	}
	return proxyModel->mapFromSource(index);
}

void ItemListView::currentChanged(const QModelIndex &current,
									const QModelIndex &previous)
{
	QListView::currentChanged(current, previous);
	QModelIndex sInd = proxyModel->mapToSource(current);
	Item* item = model->getItem(sInd);
	Q_EMIT currentChanged(item);
}
