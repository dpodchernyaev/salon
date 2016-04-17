#include "shedule_item.h"

SheduleItem::SheduleItem()
{

}

SheduleItem::~SheduleItem()
{

}

int SheduleItem::getId() const
{
	return param.id;
}

void SheduleItem::setParam(const SheduleParam &p)
{
	param = p;
}

SheduleParam SheduleItem::getParam() const
{
	return param;
}

QString SheduleItem::toString() const
{
	return
			"id: " + str(param.id) + "\n"
			"vid_id: " + str(param.vid_id) + "\n"
			"coach_id: " + str(param.coach_id) + "\n"
			"hall_id: " + str(param.hall_id) + "\n"
			"day: " + str(param.day);
}
