#ifndef COACHFETCHER_H
#define COACHFETCHER_H

#include <model/fetcher.h>

class CoachFetcher : public Fetcher
{
	Q_OBJECT
public:
	CoachFetcher();
	virtual ~CoachFetcher() {}

protected:
	virtual bool deleteSlot(Item* i, DBConn* conn);
	virtual QList<Item *> fetchSlot(DBConn *conn);
	virtual bool saveSlot(Item* item, DBConn* conn);
};

#endif // COACHFETCHER_H
