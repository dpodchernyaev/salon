
#pragma once

#include <model/fetcher.h>

class ServiceFetcher : public Fetcher
{
public:
	ServiceFetcher();
	virtual ~ServiceFetcher() {}

protected:
	virtual void fetchSlot();
	virtual bool deleteSlot(Item *i, DBConn *conn);
	virtual bool saveSlot(Item* item, DBConn* conn);
};
