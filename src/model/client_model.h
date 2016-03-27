#ifndef CLIENTMODEL_H
#define CLIENTMODEL_H

#include <QString>
#include <QMutex>
#include <model/item_model.h>

class ClientFetcher;
class ClientItem;
class Item;

class ClientModel : public ItemModel
{
	Q_OBJECT
public:
	ClientModel();

	virtual QVariant data(const QModelIndex &index, int role) const;
	ClientItem* getItem(const QModelIndex &index) const;
	QModelIndex getIndex(int id) const;
	ClientItem* getItem(int id) const;

private Q_SLOTS:
	void fetched(QList<Item*>);
};

#endif // CLIENTMODEL_H
