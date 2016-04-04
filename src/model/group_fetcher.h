#ifndef GROUPFETCHER_H
#define GROUPFETCHER_H

#include <QDateTime>

#include <model/fetcher.h>

class GroupParam;

class GroupFetcher : public Fetcher
{
	Q_OBJECT
public:
	GroupFetcher();
	virtual ~GroupFetcher();
	void setTimeInterval(const QDateTime &btime, const QDateTime &etime);

	void addGroup(const GroupParam &param);

private:
	void fetchSlot();
	bool saveSlot(Item *item, DBConn *conn);
	bool deleteSlot(Item *i, DBConn *conn);

private:
	QDateTime btime;
	QDateTime etime;
};

#endif // GROUPFETCHER_H
