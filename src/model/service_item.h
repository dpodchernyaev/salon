
#pragma once

#include <QString>
#include <QDateTime>

#include <variables.h>
#include <model/item.h>

struct ServiceParam
{
	int id;
	int vid_id;
	QString name;
	double price;
	LimitType limitType;
	int value;
	int limitDays;
	bool used;

	ServiceParam()
	{
		id = 0;
		vid_id = 0;
		name = "";
		limitType = LT_COUNT;
		price = 0.;
		value = 0;
		limitDays = 0;
		used = false;
	}
};

class ServiceItem : public Item
{
public:
	ServiceItem();
	virtual ~ServiceItem();

	int getId() const;

	void set(const ServiceParam&);
	ServiceParam get() const;

	static QString toString(LimitType type);

	virtual QString toString() const;

private:
	ServiceParam param;
};
