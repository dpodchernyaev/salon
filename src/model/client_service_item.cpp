
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

QString CsItem::toString() const
{
	return
			"Ид: " + str(param.id) + "\n"
			"Кл. Ид: "+ str(param.client_id) + "\n"
			"Дата: " + param.date.toString(DATE_TIME_FORMAT) + "\n"
			"Вид. Ид: " + str(param.vid_id) + "\n"
			"Имя: " + param.name + "\n"
			"Сумм: " + str(param.summ) + "\n"
			"Дней: " + str(param.limit_days) + "\n"
			"Тип: " + str(param.limit_type) + "\n"
			"Посещений: " + str(param.limit_value);
}
