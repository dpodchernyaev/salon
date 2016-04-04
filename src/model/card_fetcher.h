
#pragma once

#include <model/fetcher.h>

class CardFetcher : public Fetcher
{
public:
	CardFetcher();
	virtual ~CardFetcher() {}

protected:
	virtual void fetchSlot();
	virtual bool deleteSlot(Item *i, DBConn *conn);
	virtual bool saveSlot(Item* item, DBConn* conn);
};
