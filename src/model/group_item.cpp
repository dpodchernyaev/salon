#include "group_item.h"

GroupItem::GroupItem()
{

}

GroupItem::~GroupItem()
{

}

GroupParam GroupItem::getParam() const
{
	return param;
}

void GroupItem::setParam(const GroupParam &p)
{
	param = p;
}

PrivateGroupParam GroupItem::getPrivateParam() const
{
	return privateParam;
}

void GroupItem::setPrivateParam(const PrivateGroupParam &p)
{
	privateParam = p;
}

bool GroupItem::isPrivate() const
{
	return privateParam.used;
}

int GroupItem::getId() const
{
	return param.id;
}

QString GroupItem::toString() const
{
	return
			"id: " + str(param.id) + "\n"
			"vid_id: " + str(param.vid_id) + "\n"
			"cnt: " + str(param.cnt) + "\n"
			"coach_id: " + str(param.coach_id) + "\n"
			"hall_id: " + str(param.hall_id) + "\n"
			"bdtime: " + param.bdtime.toString(DATE_TIME_FORMAT);
}
