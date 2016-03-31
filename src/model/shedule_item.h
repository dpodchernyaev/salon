#ifndef SHEDULEITEM_H
#define SHEDULEITEM_H

#include <variables.h>

#include <QDateTime>

#include <model/item.h>

struct SheduleParam
{
	int id;
	int coach_id;
	int hall_id;
	int day;
	QTime bTime;
	QTime eTime;

	SheduleParam()
	{
		id = 0;
		coach_id = 0;
		hall_id = 0;
		day = 0;
	}
};

class SheduleItem : public Item
{
public:
	SheduleItem();
	virtual ~SheduleItem();

	int getId() const;

	void setParam(const SheduleParam &p);
	SheduleParam getParam() const;

private:
	SheduleParam param;
};

#endif // SHEDULEITEM_H
