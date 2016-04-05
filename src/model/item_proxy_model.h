
#pragma once

#include <QDate>
#include <QColor>

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

	struct Param
	{
		// идентификация
		int hallId;
		int day;
		QTime time;

		// параметры
		bool disabled;
		QColor color;
	};

	SheduleProxyModel(SheduleModel* model);

	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;


	void setFilterDate(const QDate &date);
	void setFilterVid(int vidId);

	void flushSheduleParam();
	void addParam(const Param &p);

private:
	Param* getParam(const QModelIndex &ind) const;
	QColor getBackgroundColor(const QModelIndex& ind) const;

protected:
	virtual  bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

private:
	QList<Param*> params;
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
