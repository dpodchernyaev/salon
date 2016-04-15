#ifndef CSFETCHER_H
#define CSFETCHER_H

class Item;
class DBConn;

#include <QMutex>

#include <model/queue_fetcher.h>

class CsFetcher : public QueueFetcher
{
	Q_OBJECT
public:
	CsFetcher();
	virtual ~CsFetcher() {}

protected:
	virtual bool deleteSlot(Item *i, DBConn *conn);
	virtual QList<Item *> fetchSlot(DBConn *conn);
	virtual bool saveSlot(Item* item, DBConn* conn);
};

#endif // CSFETCHER_H
