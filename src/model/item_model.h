#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QList>
#include <QAbstractListModel>

class Item;
class Fetcher;

enum UserModelRole
{
	SortRole = Qt::UserRole + 1,
	KeyRole = Qt::UserRole + 2,
	SearchRole = Qt::UserRole + 3
};

class ItemModel : public QAbstractListModel
{
	Q_OBJECT
public:
	ItemModel(Fetcher* fetcher);

	int rowCount(const QModelIndex &parent) const;
	Item* getItem(const QModelIndex &ind) const;

	void fetch();
	void save(Item* item);

	int indexOf(int id) const;

protected Q_SLOTS:
	virtual void fetched(QList<Item*> newItems);
	virtual void saved(bool);

Q_SIGNALS:
	void lock(bool);
	void modelRestored();

protected:
	QList<Item*> items;
	Fetcher* fetcher;
};

#endif // ITEMMODEL_H
