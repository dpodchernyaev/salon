#ifndef COACHFETCHER_H
#define COACHFETCHER_H

#include <model/fetcher.h>

class CoachFetcher : public Fetcher
{
public:
	CoachFetcher();
	virtual ~CoachFetcher() {}

protected:
	virtual void deleteSlot(int);
	virtual void fetchSlot();
	virtual void saveSlot(Item* item);
};

#endif // COACHFETCHER_H
