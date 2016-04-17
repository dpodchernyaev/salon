
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

QString ServiceItem::toString() const
{
	return
			"id: " + str(param.id) + "\n"
			"vid_id: " + str(param.vid_id) + "\n"
			"name: " + param.name + "\n"
			"limitType: " + str(param.limitType) + "\n"
			"price: " + str(param.price) + "\n"
			"value: " + str(param.value) + "\n"
			"limitDays: " + str(param.limitDays) + "\n"
			"used: " + str(param.used);
}
