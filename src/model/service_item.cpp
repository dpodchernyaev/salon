
#include <QDebug>

#include <model/service_model.h>

#include "service_item.h"

ServiceItem::ServiceItem()
{
}

ServiceItem::~ServiceItem()
{

}

int ServiceItem::getId() const
{
	return param.id;
}

QString ServiceItem::hash() const
{
	return "ServiceItem" + param.name;
}

void ServiceItem::set(const ServiceParam &p)
{
	param = p;
}

ServiceParam ServiceItem::get() const
{
	return param;
}

QString ServiceItem::toString(LimitType type)
{

	QString res = "Неизвестно";
	if (type == LT_DATE)
	{
		res = "По времени";
	}
	else if (type == LT_COUNT)
	{
		res = "По количеству";
	}
	else if (type == LT_DATE_COUNT)
	{
		res = "По времени и количеству";
	}
	return res;
}
