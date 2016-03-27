
#pragma once

#include <model/fetcher.h>

class ServiceFetcher : public Fetcher
{
public:
	ServiceFetcher();
	virtual ~ServiceFetcher() {}

protected:
	virtual void fetchSlot();
	virtual void saveSlot(Item* item);
};
