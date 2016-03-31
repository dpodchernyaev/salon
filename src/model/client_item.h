#ifndef CLIENTITEM_H
#define CLIENTITEM_H

#include <QString>
#include <QDateTime>

#include <variables.h>
#include <model/item.h>

class ClientModel;

struct ClientParam
{
	int id;
	QString name;
	QString surname;
	QString patronymic;
	QString email;
	QDateTime birthday;
	QString phone;
	QString photo;
	QString info;
	bool notify;
	int cardNumber;
	int card_id;

	ClientParam()
	{
		id = 0;
		card_id = 0;
		name = "";
		surname = "";
		patronymic = "";
		email = "";
		phone = "";
		photo = "";
		info = "";
		notify = true;
		birthday = MIN_DATE_TIME;
		cardNumber = 0;
	}
};

class ClientItem : public Item
{
public:
	ClientItem();
	virtual ~ClientItem();

	int getId() const;

	void set(const ClientParam&);
	ClientParam get() const;

private:
	ClientParam param;
};

#endif // CLIENTITEM_H
