
#pragma once

#include <QSortFilterProxyModel>

class ItemModel;

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

private:
	ItemModel* sourceModel;
	Filter filter;
};
