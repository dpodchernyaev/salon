
#pragma once

#include <model/fetcher.h>

class HallFetcher : public Fetcher
{
public:
	HallFetcher();
	virtual ~HallFetcher() {}

protected:
	virtual void deleteSlot(int id);
	virtual void fetchSlot();
	virtual void saveSlot(Item* item);
};
