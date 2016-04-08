#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QList>
#include <QAbstractTableModel>

class Item;
class Fetcher;

enum UserModelRole
{
	SortRole = Qt::UserRole + 1,
	KeyRole = Qt::UserRole + 2,
	SearchRole = Qt::UserRole + 3,
	PriceRole = Qt::UserRole + 4,
	DiscontRole = Qt::UserRole + 5,
	BirthdayRole = Qt::UserRole + 6,
	IsActive = Qt::UserRole + 7,
	TimeSortRole = Qt::UserRole + 8,
	LimitValueRole = Qt::UserRole + 9
};

class ItemModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	ItemModel(Fetcher* fetcher);

	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;
	QList<Item*> getItems() const;
	Item* getItem(const QModelIndex &ind) const;
	Item* getItem(int id) const;

	Fetcher* getFetcher() const;

	void fetch();
	void save(Item* item);
	void add(Item* item);
	void deleteItem(Item* item);


	virtual void clean();

	QModelIndex getIndex(Item* item) const;

	int indexOf(int id) const;
	int indexOf(Item* item) const;

protected Q_SLOTS:
	virtual void fetched(QList<Item*> newItems);
	virtual void saved(Item *item, bool);
	virtual void deleted(bool);

Q_SIGNALS:
	void lock(bool);
	void modelRestored();

protected:
	QList<Item*> items;
	Fetcher* fetcher;
};

#endif // ITEMMODEL_H
