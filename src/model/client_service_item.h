#ifndef CLIENTSERVICEITEM_H
#define CLIENTSERVICEITEM_H

#include <QDateTime>

#include <variables.h>
#include <model/item.h>

struct CsParam
{
	int id;
	int client_id;
	int service_id;
	double summ;
	QDateTime date;

	CsParam()
	{
		summ = 0;
		id = 0;
		client_id = 0;
		service_id = 0;
		date = MIN_DATE_TIME;
	}
};

class CsItem : public Item
{
public:
	CsItem();
	virtual ~CsItem();

	QString hash() const;
	int getId() const;

	void setParam(const CsParam &p);
	CsParam getParam() const;


private:
	CsParam param;
};

#endif // CLIENTSERVICEITEM_H
