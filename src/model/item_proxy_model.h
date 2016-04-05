
#pragma once

#include <QDate>

#include <QSortFilterProxyModel>

class ItemModel;
class CsModel;
class SheduleModel;

class ItemProxyModel : public QSortFilterProxyModel
{
public:
	struct Filter
	{
		QString display;
		Filter()
		{
			display = "";
		}
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

	void setFilterDate(const QDate &date);

private:
	QDate date;
};

class SheduleProxyModel : public ItemProxyModel
{
public:
	SheduleProxyModel(SheduleModel* model);

	void setFilterDate(const QDate &date);
	void setFilterVid(int vidId);

protected:
	virtual  bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

private:
	int vidId;
	QDate date;
};

class ClientProxyModel : public ItemProxyModel
{
public:
	ClientProxyModel(ItemModel* model);
	virtual ~ClientProxyModel();

protected:
	virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};
