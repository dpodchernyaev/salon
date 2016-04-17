#include "vid_item.h"

VidItem::VidItem()
{

}


int VidItem::getId() const
{
	return param.id;
}

void VidItem::setParam(const VidParam &p)
{
	param = p;
}

VidParam VidItem::getParam() const
{
	return param;
}

QString VidItem::toString() const
{
	return "id: " + str(param.id) + "\n"
			"name: " + param.name;
}
