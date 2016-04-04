#ifndef CSFETCHER_H
#define CSFETCHER_H

class Item;
class DBConn;

#include <QMutex>

#include <model/fetcher.h>

class CsFetcher : public Fetcher
{
public:
	CsFetcher();
	virtual ~CsFetcher() {}

	void fetchClient(int clientId);

protected:
	virtual bool deleteSlot(Item *i, DBConn *conn);
	virtual void fetchSlot();
	virtual bool saveSlot(Item* item, DBConn* conn);

private:
	QList<int> queue;
	int lastId;
	QMutex mtx;
};

#endif // CSFETCHER_H
