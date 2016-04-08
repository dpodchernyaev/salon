#ifndef IGROUP_ITEM
#define IGROUP_ITEM

#include <QDate>

class IGroupItem
{
public:
	virtual QTime getTime1() const = 0;
	virtual QTime getTime2() const = 0;
	virtual int getDay() const = 0;
	virtual int getHallId() const = 0;
	virtual int getCoachId() const = 0;
};

#endif // IGROUP_ITEM

