#ifndef FETCHER_H
#define FETCHER_H

#include <QObject>
#include <QList>

class Item;
class DBConn;

class Fetcher : public QObject
{
	Q_OBJECT
public:
	Fetcher();
	virtual ~Fetcher();

	void fetch();
	void save(Item* item);
	bool refetch(DBConn* conn);
	void deleteItem(Item* item);

	bool saveItem(Item* item, DBConn* conn);
	virtual bool saveSlot(Item* item, DBConn* conn) = 0;
	virtual bool deleteSlot(Item* i, DBConn* conn) = 0;
	virtual QList<Item*> fetchSlot(DBConn* conn) = 0;

protected Q_SLOTS:
	virtual void fetchPrivate();

private Q_SLOTS:
	void savePrivate(Item* item);
	void deletePrivate(Item* item);


Q_SIGNALS:
	void fetchSignal();
	void saveSignal(Item*);
	void deleteSignal(Item*);
	void fetched(QList<Item*> items);
	void saved(Item*, bool);
	void deleted(bool);
};

#endif // FETCHER_H
