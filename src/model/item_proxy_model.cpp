
#include <QDateTime>

#include <variables.h>
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
	QModelIndex sInd = sourceModel->index(source_row, 0);
	if (!filter.display.isEmpty())
	{
		QString value = sInd.data(SearchRole).toString();
		if (!value.contains(filter.display, Qt::CaseInsensitive))
		{
			res = false;
		}
	}
	return res;
}


// ====== ClientProxyModel ======

ClientProxyModel::ClientProxyModel(ItemModel* model) : ItemProxyModel(model)
{
}

ClientProxyModel::~ClientProxyModel()
{

}

bool ClientProxyModel::lessThan(const QModelIndex &left,
								const QModelIndex &right) const
{
	bool res = ItemProxyModel::lessThan(left, right);

	QVariant leftVar = left.data(BirthdayRole);
	QVariant rightVar = right.data(BirthdayRole);

	if (!leftVar.isNull() && !rightVar.isNull())
	{
		if (leftVar.type() == QVariant::DateTime &&
				rightVar.type() == QVariant::DateTime)
		{
			QDateTime rd = rightVar.toDateTime();
			QDateTime ld = leftVar.toDateTime();
			if (isBirthday(rd.date()) != isBirthday(ld.date()))
			{
				res = !isBirthday(rd.date());
			}
		}
	}

	return res;
}
