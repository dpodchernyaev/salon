#ifndef COACHITEM_H
#define COACHITEM_H

#include <model/item.h>

class CoachItem : public Item
{
public:
	CoachItem();
	virtual ~CoachItem() {}

	QString getName() const;
	QString getPhone() const;
	int getId() const;

	void setName(const QString &value);
	void setPhone(const QString &value);
	void setId(int id);

private:
	int id;
	QString name;
	QString phone;
};

#endif // COACHITEM_H
