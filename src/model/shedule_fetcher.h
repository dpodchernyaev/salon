#ifndef SHEDULEFETCHER_H
#define SHEDULEFETCHER_H

#include <model/fetcher.h>

class SheduleFetcher : public Fetcher
{
	Q_OBJECT
public:
	SheduleFetcher();
	virtual ~SheduleFetcher() {}

protected:
	virtual bool deleteSlot(Item *i, DBConn *conn);
	virtual QList<Item *> fetchSlot(DBConn *conn);
	virtual bool saveSlot(Item* item, DBConn* conn);
};

#endif // SHEDULEFETCHER_H
