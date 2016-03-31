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
