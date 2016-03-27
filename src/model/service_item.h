
#pragma once

#include <QString>
#include <QDateTime>

#include <variables.h>
#include <model/item.h>

enum LimitType
{
	LT_DATE,
	LT_COUNT,
	LT_DATE_COUNT
};

struct ServiceParam
{
	int id;
	QString name;
	double price;
	LimitType limitType;
	int value;
	int limitDays;
	bool used;

	ServiceParam()
	{
		id = 0;
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

	virtual QString hash() const;

	void set(const ServiceParam&);
	ServiceParam get() const;

	static QString toString(LimitType type);
private:
	ServiceParam param;
};
