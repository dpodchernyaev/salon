
#pragma once

#include <model/fetcher.h>

class HallFetcher : public Fetcher
{
public:
	HallFetcher();
	virtual ~HallFetcher() {}

protected:
	virtual bool deleteSlot(Item *i, DBConn *conn);
	virtual void fetchSlot();
	virtual bool saveSlot(Item* item, DBConn* conn);
};
