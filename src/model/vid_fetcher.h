#ifndef VID_FETCHER_H
#define VID_FETCHER_H

#include <model/fetcher.h>

class VidFetcher : public Fetcher
{
	Q_OBJECT
public:
	VidFetcher();
	virtual ~VidFetcher() {}

	bool saveSlot(Item* item, DBConn* conn);
	bool deleteSlot(Item* i, DBConn* conn);
	QList<Item *> fetchSlot(DBConn *conn);
};

#endif // VID_FETCHER_H
