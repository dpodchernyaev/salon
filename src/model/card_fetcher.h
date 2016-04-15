
#pragma once

#include <model/fetcher.h>

class CardFetcher : public Fetcher
{
	Q_OBJECT
public:
	CardFetcher();
	virtual ~CardFetcher() {}

protected:
	virtual QList<Item *> fetchSlot(DBConn *conn);
	virtual bool deleteSlot(Item *i, DBConn *conn);
	virtual bool saveSlot(Item* item, DBConn* conn);
};
