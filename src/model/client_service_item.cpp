
#include "client_service_item.h"

CsItem::CsItem()
{

}

CsItem::~CsItem()
{

}

QString CsItem::hash() const
{
	return "ClientServiceItem_" +
			QString::number(param.client_id) + "_" +
			QString::number(param.service_id) + "_" +
			param.date.toString(DATE_TIME_FORMAT) + "_" +
			QString::number(param.id);
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
