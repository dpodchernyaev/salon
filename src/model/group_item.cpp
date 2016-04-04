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

int GroupItem::getId() const
{
	return param.id;
}
