#ifndef CSMODEL_H
#define CSMODEL_H

#include <model/item_model.h>

class CsFetcher;
class CsParam;

class CsModel : public ItemModel
{
public:
	CsModel();
	virtual ~CsModel();

	void fetchForClient(int id);

	int columnCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;

	bool isActive(const QModelIndex& ind) const;
	bool isActive(const CsParam &p) const;

	double getSumm() const;

private:
	int clientId;
	QMap<int, QList<Item*> > cached;
};

#endif // CSMODEL_H
