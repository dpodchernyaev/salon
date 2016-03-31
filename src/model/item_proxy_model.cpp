
#include <QDateTime>
#include <QPixmap>
#include <QBrush>

#include <variables.h>

#include <model/client_model.h>
#include <model/cs_model.h>
#include <model/shedule_model.h>

#include <model/shedule_item.h>
#include <model/client_service_item.h>

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


// =====================


CsProxyModel::CsProxyModel(CsModel* model) : ItemProxyModel(model)
{
}

void CsProxyModel::setFilterDate(const QDate &date)
{
	this->date = date;
}

QVariant CsProxyModel::data(const QModelIndex &index, int role) const
{
	QVariant res = ItemProxyModel::data(index, role);
	if (role == Qt::DecorationRole)
	{
		if ( (index.column() == 0) && (index.data(IsActive).toBool() == false) )
		{
			res = QPixmap("pics/del_16.png", "PNG");
		}
	}
	else if (role == Qt::BackgroundColorRole)
	{
		if (index.data(IsActive).toBool() == false)
		{
			return QBrush(QColor(122, 122, 122, 122));
		}
	}
	return res;
}
bool CsProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	bool res = QSortFilterProxyModel::lessThan(left, right);

	if (left.data(IsActive) != right.data(IsActive))
	{
		res = left.data(IsActive).toBool();
		if (sortOrder() == Qt::DescendingOrder)
		{
			res = !res;
		}
	}
	return res;
}


bool CsProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
	bool res = ItemProxyModel::filterAcceptsRow(source_row, source_parent);

	if (date.isValid())
	{
		Item* item = sourceModel->getItem(sourceModel->index(source_row, 0));
		CsItem* cs = (CsItem*)item;
	}

	return res;
}


// =====================


SheduleProxyModel::SheduleProxyModel(SheduleModel* model) : ItemProxyModel(model)
{
}

void SheduleProxyModel::setFilterDate(const QDate &date)
{
	this->date = date;
}

bool SheduleProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
	bool res = ItemProxyModel::filterAcceptsRow(source_row, source_parent);

	if (date.isValid())
	{
		Item* item = sourceModel->getItem(sourceModel->index(source_row, 0));
		SheduleItem* cs = (SheduleItem*)item;
		if (cs->getParam().day != date.dayOfWeek())
		{
			res = false;
		}
	}

	return res;
}
