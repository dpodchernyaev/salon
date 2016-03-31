
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
