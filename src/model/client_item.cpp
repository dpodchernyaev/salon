
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

void ClientItem::set(const ClientParam &p)
{
	param = p;
}

ClientParam ClientItem::get() const
{
	return param;
}

QString ClientItem::toString() const
{
	return
		"id: " + str(param.id) + "\n"
		"card_id: "+ str(param.card_id) + "\n"
		"name: " + param.name + "\n"
		"surname: " + param.surname + "\n"
		"patronymic: " + param.patronymic + "\n"
		"email: " + param.email + "\n"
		"phone: " + param.phone + "\n"
		"photo: " + param.photo + "\n"
		"info: " + param.info + "\n"
		"notify: " + str(param.notify) + "\n"
		"birthday: " + param.birthday.toString(DATE_FORMAT) + "\n"
		"cardNumber: " + str(param.cardNumber);
}
