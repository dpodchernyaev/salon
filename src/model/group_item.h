#ifndef GROUPITEM_H
#define GROUPITEM_H

#include <QDateTime>
#include <QTime>

#include <variables.h>
#include <model/item.h>

struct PrivateGroupParam
{
	double summ;
	bool used;

	PrivateGroupParam()
	{
		summ = 0;
		used = false;
	}
};

struct GroupParam
{
	int id;
	int vid_id;
	int coach_id;
	int hall_id;
	int cnt;
	QDateTime bdtime;
	QTime etime;

	GroupParam()
	{
		id = 0;
		vid_id = 0;
		cnt = 0;
		coach_id = 0;
		hall_id = 0;
		bdtime = MIN_DATE_TIME;
	}
};

class GroupItem : public Item
{
public:
	GroupItem();
	virtual ~GroupItem();

	GroupParam getParam() const;
	void setParam(const GroupParam &p);

	PrivateGroupParam getPrivateParam() const;
	void setPrivateParam(const PrivateGroupParam &p);

	bool isPrivate() const;

	virtual int getId() const;

private:
	GroupParam param;
	PrivateGroupParam privateParam;
};

#endif // GROUPITEM_H
