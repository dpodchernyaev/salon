
#pragma once

#include <model/fetcher.h>

class HallFetcher : public Fetcher
{
	Q_OBJECT
public:
	HallFetcher();
	virtual ~HallFetcher() {}

protected:
	virtual bool deleteSlot(Item *i, DBConn *conn);
	virtual QList<Item *> fetchSlot(DBConn *conn);
	virtual bool saveSlot(Item* item, DBConn* conn);
};
