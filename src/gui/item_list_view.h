
#pragma once

#include <gui/background_animation.h>

#include <gui/animation_list_view.h>

class ItemModel;
class ItemProxyModel;
class Item;

class ItemListView : public AnimationListView
{
	Q_OBJECT
public:
	ItemListView(ItemModel *model);
	virtual ~ItemListView();

	void setProxyModel(ItemProxyModel* proxy);
	ItemModel* getSourceModel() const;
	ItemProxyModel* getProxyModel() const;
	QModelIndex mapFromSource(const QModelIndex &index) const;
	Item* getSelected() const;

protected:
	void currentChanged(const QModelIndex &current, const QModelIndex &previous);

Q_SIGNALS:
	void currentChanged(Item*);

private:
	ItemModel* model;
	ItemProxyModel* proxyModel;
};

class ItemTableView : public AnimationTableView
{
	Q_OBJECT
public:
	ItemTableView(ItemModel *model);
	virtual ~ItemTableView();

	ItemModel* getSourceModel() const;
	ItemProxyModel* getProxyModel() const;
	QModelIndex mapFromSource(const QModelIndex &index) const;

	Item* getSelected() const;

protected:
	void currentChanged(const QModelIndex &current, const QModelIndex &previous);

Q_SIGNALS:
	void currentChanged(Item*);

private:
	ItemModel* model;
	ItemProxyModel* proxyModel;
};
