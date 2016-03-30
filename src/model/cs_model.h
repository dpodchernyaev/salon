#ifndef CSMODEL_H
#define CSMODEL_H

#include <model/item_model.h>

class CsFetcher;

class CsModel : public ItemModel
{
public:
	CsModel();
	virtual ~CsModel();

	void fetchForClient(int id);

	int columnCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;

private:
	int clientId;
};

#endif // CSMODEL_H
