
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

void ItemListView::setProxyModel(ItemProxyModel* proxy)
{
	setModel(proxy);
	delete proxyModel;
	proxyModel = proxy;
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

Item* ItemListView::getSelected() const
{
	QItemSelectionModel* smodel = selectionModel();
	QModelIndexList indList = smodel->selectedIndexes();
	if (indList.isEmpty())
	{
		return NULL;
	}

	QModelIndex ind = indList.first();
	QModelIndex sind = proxyModel->mapToSource(ind);
	return model->getItem(sind);
}

//================
ItemTableView::ItemTableView(ItemModel* model) : model(model)
{
	proxyModel = new ItemProxyModel(model);
	proxyModel->setSortRole(SortRole);
	proxyModel->setDynamicSortFilter(true);
	proxyModel->sort(0);
	setModel(proxyModel);
}

ItemTableView::~ItemTableView()
{
	delete proxyModel;
}

ItemModel *ItemTableView::getSourceModel() const
{
	return model;
}

ItemProxyModel* ItemTableView::getProxyModel() const
{
	return proxyModel;
}

QModelIndex ItemTableView::mapFromSource(const QModelIndex &index) const
{
	if (!index.isValid())
	{
		return QModelIndex();
	}
	return proxyModel->mapFromSource(index);
}

Item* ItemTableView::getSelected() const
{
	QItemSelectionModel* smodel = selectionModel();
	QModelIndexList indList = smodel->selectedIndexes();
	if (indList.isEmpty())
	{
		return NULL;
	}

	QModelIndex ind = indList.first();
	QModelIndex sind = proxyModel->mapToSource(ind);
	return model->getItem(sind);
}

void ItemTableView::currentChanged(const QModelIndex &current,
									const QModelIndex &previous)
{
	QTableView::currentChanged(current, previous);
	QModelIndex sInd = proxyModel->mapToSource(current);
	Item* item = model->getItem(sInd);
	Q_EMIT currentChanged(item);
}
