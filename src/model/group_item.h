#ifndef GROUPITEM_H
#define GROUPITEM_H

#include <QDateTime>
#include <QTime>

#include <variables.h>
#include <model/item.h>

struct GroupParam
{
	int id;
	int coach_id;
	int hall_id;
	QDateTime bdtime;
	QTime etime;

	GroupParam()
	{
		id = 0;
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

	virtual int getId() const;

private:
	GroupParam param;
};

#endif // GROUPITEM_H
