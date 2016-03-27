
#include <QDebug>

#include <model/client_model.h>

#include "client_item.h"

ClientItem::ClientItem()
{
}

ClientItem::~ClientItem()
{

}

int ClientItem::getId() const
{
	return param.id;
}

QString ClientItem::hash() const
{
	return
			"ClientItem" + param.surname + "_"
			+ param.name + "_"
			+ param.patronymic + "_"
			+ param.birthday.toString(DATE_FORMAT);
}

void ClientItem::set(const ClientParam &p)
{
	param = p;
}

ClientParam ClientItem::get() const
{
	return param;
}
