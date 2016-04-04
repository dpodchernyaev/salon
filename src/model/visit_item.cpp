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
