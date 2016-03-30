#ifndef CSFETCHER_H
#define CSFETCHER_H

class Item;

#include <model/fetcher.h>

class CsFetcher : public Fetcher
{
public:
	CsFetcher();
	virtual ~CsFetcher() {}

	void fetchClient(int clientId);

protected:
	virtual void deleteSlot(int);
	virtual void fetchSlot();
	virtual void saveSlot(Item* item);

private:
	int clientId;
};

#endif // CSFETCHER_H
