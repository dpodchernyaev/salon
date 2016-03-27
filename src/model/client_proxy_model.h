#ifndef CLIENTPROXYMODEL_H
#define CLIENTPROXYMODEL_H

#include <QSortFilterProxyModel>

class ClientModel;

class ClientProxyModel : public QSortFilterProxyModel
{
public:
	struct Filter
	{
		QString display;
	};

	ClientProxyModel(ClientModel* model);
	virtual ~ClientProxyModel();

	void setFilter(Filter f);

protected:
	virtual bool filterAcceptsRow(int source_row,
								  const QModelIndex &source_parent) const;

private:
	ClientModel* sourceModel;
	Filter filter;
};

#endif // CLIENTPROXYMODEL_H
