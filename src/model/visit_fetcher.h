#ifndef VISITFETCHER_H
#define VISITFETCHER_H

#include <QDate>
#include <QMutex>

#include <model/visit_item.h>

#include <model/queue_fetcher.h>


class GroupParam;
class VisitParam;
class GroupFetcher;
class GroupModel;
class CsModel;

class VisitFetcher : public QueueFetcher
{
	Q_OBJECT
public:
	VisitFetcher();
	virtual ~VisitFetcher();

	void fetchForClient(int id, const QDateTime &dtime);

private:
	QList<Item*> fetchSlot(DBConn* conn);
	bool saveSlot(Item *item, DBConn* conn);
	bool deleteSlot(Item *i, DBConn *conn);

	void inc();
	void dec();

private:
	GroupModel* gModel;
	CsModel* csModel;
	QDateTime minDate;
};

#endif // VISITFETCHER_H
