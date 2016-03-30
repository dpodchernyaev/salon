#include "shedule_item.h"

SheduleItem::SheduleItem()
{

}

SheduleItem::~SheduleItem()
{

}


QString SheduleItem::hash() const
{
	return "SheduleItem_" + QString::number(param.id) + "_" +
			QString::number(param.coach_id) + "_" +
			QString::number(param.hall_id) + "_" +
			QString::number(param.day) + "_" +
			param.bTime.toString(DATE_TIME_FORMAT) + "_" +
			param.eTime.toString(DATE_TIME_FORMAT);
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
