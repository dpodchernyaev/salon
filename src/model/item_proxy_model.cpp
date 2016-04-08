
#include <QDateTime>
#include <QPixmap>
#include <QBrush>

#include <variables.h>

#include <model/client_model.h>
#include <model/cs_model.h>
#include <model/shedule_model.h>
#include <model/group_model.h>

#include <model/shedule_item.h>
#include <model/group_item.h>
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
	vidId = 0;
}

void SheduleProxyModel::setFilterDate(const QDate &date)
{
	this->date = date;
}

void SheduleProxyModel::setFilterVid(int vidId)
{
	this->vidId = vidId;
}

SheduleProxyModel::Param* SheduleProxyModel::getParam(const QModelIndex& ind) const
{
	SheduleProxyModel::Param* res = NULL;
	QModelIndex sind = mapToSource(ind);
	Item* i  = sourceModel->getItem(sind);
	if (i != NULL)
	{
		SheduleItem* si = (SheduleItem*)i;
		SheduleParam sp = si->getParam();
		Q_FOREACH (SheduleProxyModel::Param* p, params)
		{
			if ( (sp.day == p->day) &&
				 sp.bTime == p->time &&
				 sp.hall_id == p->hallId)
			{
				res = p;
				break;
			}
		}
	}
	return res;
}

void SheduleProxyModel::flushSheduleParam()
{
	qDeleteAll(params);
	params.clear();
}

QColor SheduleProxyModel::getBackgroundColor(const QModelIndex& ind) const
{
	QColor res;

	SheduleProxyModel::Param* p = getParam(ind);
	if (p != NULL)
	{
		res = p->color;
	}
	return res;
}

Qt::ItemFlags SheduleProxyModel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags res = ItemProxyModel::flags(index);

	SheduleProxyModel::Param* p = getParam(index);
	if ( (p != NULL) && (p->disabled == true) )
	{
		res = Qt::NoItemFlags;
	}
	return res;
}

QVariant SheduleProxyModel::data(const QModelIndex &index, int role) const
{
	QVariant res = ItemProxyModel::data(index, role);
	if (role == Qt::BackgroundColorRole)
	{
		QColor color = getBackgroundColor(index);
		if (color.isValid())
		{
			color.setAlpha(122);
			res = QBrush(color);
		}
	}
	return res;
}

void SheduleProxyModel::addParam(const SheduleProxyModel::Param &p)
{
	SheduleProxyModel::Param *newP = new SheduleProxyModel::Param;
	*newP = p;
	params.append(newP);
}

bool SheduleProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
	bool res = ItemProxyModel::filterAcceptsRow(source_row, source_parent);

	Item* item = sourceModel->getItem(sourceModel->index(source_row, 0));
	SheduleItem* cs = (SheduleItem*)item;
	if (date.isValid())
	{
		if (cs->getParam().day != date.dayOfWeek())
		{
			res = false;
		}
	}

	if (vidId > 0)
	{
		if (cs->getParam().vid_id != vidId)
		{
			res = false;
		}
	}

	return res;
}

// ============================================

PgProxyModel::PgProxyModel(GroupModel *model) : ItemProxyModel(model)
{

}

void PgProxyModel::setFilterDate(const QDate &date)
{
	filterDate = date;
}

PgProxyModel::~PgProxyModel()
{

}

bool PgProxyModel::filterAcceptsRow(int source_row,
							   const QModelIndex &source_parent) const
{
	bool res = ItemProxyModel::filterAcceptsRow(source_row, source_parent);

	Item* item = sourceModel->getItem(sourceModel->index(source_row, 0));
	if (item != NULL)
	{
		GroupItem* cs = (GroupItem*)item;
		if (!cs->isPrivate())
		{
			res = false;
		}

		QDate date = cs->getParam().bdtime.date();
		if (!filterDate.isNull() && (res == true) )
		{
			res = date == filterDate;
		}
	}
	return res;
}
