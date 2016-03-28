
#include <QDebug>

#include "coach_item.h"

CoachItem::CoachItem()
{
	id = 0;
}

QString CoachItem::hash() const
{
	return "CoachItem_" + name + "_" + phone;
}

QString CoachItem::getName() const
{
	return name;
}

QString CoachItem::getPhone() const
{
	return phone;
}

int CoachItem::getId() const
{
	return id;
}

void CoachItem::setName(const QString &value)
{
	name = value;
}

void CoachItem::setPhone(const QString &value)
{
	phone = value;
}

void CoachItem::setId(int id)
{
	this->id = id;
}
