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
