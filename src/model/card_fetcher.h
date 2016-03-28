
#pragma once

#include <model/fetcher.h>

class CardFetcher : public Fetcher
{
public:
	CardFetcher();
	virtual ~CardFetcher() {}

protected:
	virtual void fetchSlot();
	virtual void deleteSlot(int id);
	virtual void saveSlot(Item* item);
};
