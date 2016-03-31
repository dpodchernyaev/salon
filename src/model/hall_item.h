
#pragma once

#include <model/item.h>

class HallItem : public Item
{
public:
	HallItem();
	virtual ~HallItem() {}

	QString getName() const;
	int getCnt() const;
	int getId() const;

	void setCnt(int cnt);
	void setName(const QString &value);
	void setId(int id);

private:
	int id;
	QString name;
	int cnt;
};
