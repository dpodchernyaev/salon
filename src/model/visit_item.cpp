#include "visit_item.h"

VisitItem::VisitItem()
{
	group = NULL;
	csItem = NULL;
}

int VisitItem::getId() const
{
	return param.id;
}

void VisitItem::setParam(const VisitParam &p)
{
	param = p;
}

VisitParam VisitItem::getParam() const
{
	return param;
}

void VisitItem::setGroup(GroupItem* g)
{
	group = g;
}

GroupItem* VisitItem::getGroup() const
{
	return group;
}

void VisitItem::setCs(CsItem* cs)
{
	this->csItem = cs;
}

CsItem* VisitItem::getCs() const
{
	return csItem;
}

QString VisitItem::toString() const
{
	return
			"id: " + str(param.id) + "\n"
			"cs_id: " + str(param.cs_id) + "\n"
			"vgroup_id: " + str(param.vgroup_id) + "\n"
			"info: " + param.info + "\n"
			"dtime: " + param.dtime.toString(DATE_TIME_FORMAT);
}
