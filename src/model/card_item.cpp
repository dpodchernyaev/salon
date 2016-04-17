
#include "card_item.h"

CardItem::CardItem()
{
	id = 0;
	discont = 0;
}

QString CardItem::getName() const
{
	return name;
}

double CardItem::getDiscont() const
{
	return discont;
}

int CardItem::getId() const
{
	return id;
}

void CardItem::setDiscont(int discont)
{
	this->discont = discont;
}

void CardItem::setName(const QString &value)
{
	name = value;
}

void CardItem::setId(int id)
{
	this->id = id;
}

QString CardItem::toString() const
{
	return "id: " + str(id) + "\n"
			"name: " + name + "\n"
			"cnt: " + str(discont);
}
