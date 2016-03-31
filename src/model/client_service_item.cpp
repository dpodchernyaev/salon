
#include "client_service_item.h"

CsItem::CsItem()
{

}

CsItem::~CsItem()
{

}

int CsItem::getId() const
{
	return param.id;
}

void CsItem::setParam(const CsParam &p)
{
	param = p;
}

CsParam CsItem::getParam() const
{
	return param;
}
