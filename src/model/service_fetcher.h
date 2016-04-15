
#pragma once

#include <model/fetcher.h>

class ServiceFetcher : public Fetcher
{
	Q_OBJECT
public:
	ServiceFetcher();
	virtual ~ServiceFetcher() {}

protected:
	virtual QList<Item *> fetchSlot(DBConn *conn);
	virtual bool deleteSlot(Item *i, DBConn *conn);
	virtual bool saveSlot(Item* item, DBConn* conn);
};
