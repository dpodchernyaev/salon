
#pragma once

#include <QSortFilterProxyModel>

class ItemModel;
class CsModel;

class ItemProxyModel : public QSortFilterProxyModel
{
public:
	struct Filter
	{
		QString display;
	};

	ItemProxyModel(ItemModel* model);
	virtual ~ItemProxyModel();

	void setFilter(Filter f);

protected:
	virtual bool filterAcceptsRow(int source_row,
								  const QModelIndex &source_parent) const;

protected:
	ItemModel* sourceModel;

private:
	Filter filter;
};

class CsProxyModel : public ItemProxyModel
{
public:
	CsProxyModel(CsModel* model);

protected:
	virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
	virtual  bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
};

class ClientProxyModel : public ItemProxyModel
{
public:
	ClientProxyModel(ItemModel* model);
	virtual ~ClientProxyModel();

protected:
	virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};
