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
	IsActive = Qt::UserRole + 6,
	TimeSortRole = Qt::UserRole + 7,
};

class ItemModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	ItemModel(Fetcher* fetcher);

	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;
	Item* getItem(const QModelIndex &ind) const;
	Item* getItem(int id) const;

	void fetch();
	void save(Item* item);
	void deleteItem(Item* item);

	void clean();

	QModelIndex getIndex(Item* item) const;

	int indexOf(int id) const;
	int indexOf(Item* item) const;

protected Q_SLOTS:
	virtual void fetched(QList<Item*> newItems);
	virtual void saved(bool);
	virtual void deleted(bool);

Q_SIGNALS:
	void lock(bool);
	void modelRestored();

protected:
	QList<Item*> items;
	Fetcher* fetcher;

	Item* forSave;
};

#endif // ITEMMODEL_H
