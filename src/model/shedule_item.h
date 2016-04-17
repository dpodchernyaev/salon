#ifndef SHEDULEITEM_H
#define SHEDULEITEM_H

#include <variables.h>

#include <QDateTime>

#include <model/item.h>
#include <model/igroup_item.h>

struct SheduleParam
{
	int id;
	int vid_id;
	int coach_id;
	int hall_id;
	int day;
	QTime bTime;
	QTime eTime;

	SheduleParam()
	{
		id = 0;
		vid_id = 0;
		coach_id = 0;
		hall_id = 0;
		day = 0;
	}
};

class SheduleItem : public Item, public IGroupItem
{
public:
	SheduleItem();
	virtual ~SheduleItem();

	int getId() const;

	void setParam(const SheduleParam &p);
	SheduleParam getParam() const;

	// === IGroupItem ===
	virtual QTime getTime1() const { return param.bTime; }
	virtual QTime getTime2() const { return param.eTime; }
	virtual int getDay() const { return param.day; }
	virtual int getHallId() const { return param.hall_id; }
	virtual int getCoachId() const { return param.coach_id; }

	virtual QString toString() const;

private:
	SheduleParam param;
};

#endif // SHEDULEITEM_H
