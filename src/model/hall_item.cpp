#include "hall_item.h"

HallItem::HallItem()
{
	id = 0;
	cnt = 0;
}

QString HallItem::getName() const
{
	return name;
}

int HallItem::getCnt() const
{
	return cnt;
}

int HallItem::getId() const
{
	return id;
}

void HallItem::setCnt(int cnt)
{
	this->cnt = cnt;
}

void HallItem::setName(const QString &value)
{
	name = value;
}

void HallItem::setId(int id)
{
	this->id = id;
}

QString HallItem::toString() const
{
	return "id: " + str(id) + "\n"
			"name: " + name + "\n"
			"cnt: " + str(cnt);
}
