
#pragma once

#include <model/item.h>

class CardItem : public Item
{
public:
	CardItem();
	virtual ~CardItem() {}

	QString getName() const;
	double getDiscont() const;
	virtual int getId() const;

	void setDiscont(int discont);
	void setName(const QString &value);
	void setId(int id);

	QString hash() const;

private:
	int id;
	QString name;
	double discont;
};
