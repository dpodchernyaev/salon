#ifndef COACHFETCHER_H
#define COACHFETCHER_H

#include <model/fetcher.h>

class CoachFetcher : public Fetcher
{
public:
	CoachFetcher();
	virtual ~CoachFetcher() {}

protected:
	virtual bool deleteSlot(Item* i, DBConn* conn);
	virtual void fetchSlot();
	virtual bool saveSlot(Item* item, DBConn* conn);
};

#endif // COACHFETCHER_H
