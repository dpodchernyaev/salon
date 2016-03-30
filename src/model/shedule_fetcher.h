#ifndef SHEDULEFETCHER_H
#define SHEDULEFETCHER_H

#include <model/fetcher.h>

class SheduleFetcher : public Fetcher
{
public:
	SheduleFetcher();
	virtual ~SheduleFetcher() {}

protected:
	virtual void deleteSlot(int);
	virtual void fetchSlot();
	virtual void saveSlot(Item* item);
};

#endif // SHEDULEFETCHER_H
