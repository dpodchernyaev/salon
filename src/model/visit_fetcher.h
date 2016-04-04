#ifndef VISITFETCHER_H
#define VISITFETCHER_H

#include <QDate>
#include <QMutex>

#include <model/visit_item.h>

#include <model/fetcher.h>


class GroupParam;
class VisitParam;
class GroupFetcher;
class GroupModel;
class CsModel;

class VisitFetcher : public Fetcher
{
	Q_OBJECT
public:
	VisitFetcher();
	virtual ~VisitFetcher();

	void fetchForClient(int id);

private:
	void fetchSlot();
	bool saveSlot(Item *item, DBConn* conn);
	bool deleteSlot(Item *i, DBConn *conn);

	void inc();
	void dec();

private:
	GroupModel* gModel;
	CsModel* csModel;
	QList<int> queue;
	QMutex mtx;

	QDate minDate;
};

#endif // VISITFETCHER_H
