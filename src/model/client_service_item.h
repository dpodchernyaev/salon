#ifndef CLIENTSERVICEITEM_H
#define CLIENTSERVICEITEM_H

#include <QDateTime>

#include <variables.h>
#include <model/item.h>

struct CsParam
{
	int id;
	int vid_id;
	int client_id;
	int limit_days;
	int limit_value;
	LimitType limit_type;
	double summ;
	QDateTime date;
	QString name;

	CsParam()
	{
		name = "";
		summ = 0;
		id = 0;
		vid_id = 0;
		client_id = 0;
		limit_days = 0;
		limit_type = LT_DATE_COUNT;
		limit_value = 0;
		date = MIN_DATE_TIME;
	}
};

class CsItem : public Item
{
public:
	CsItem();
	virtual ~CsItem();

	int getId() const;

	void setParam(const CsParam &p);
	CsParam getParam() const;

	virtual QString toString() const;


private:
	CsParam param;
};

#endif // CLIENTSERVICEITEM_H
