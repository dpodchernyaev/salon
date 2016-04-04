#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include <QHash>

class ItemModel;

/* порядок обязателен */
enum LimitType
{
	LT_DATE,
	LT_COUNT,
	LT_DATE_COUNT
};

class Item
{
public:
	Item();
	virtual ~Item();

	void setModel(ItemModel* model);
	void save();

	QVariant data(int role);

	virtual int getId() const = 0;

protected:
	ItemModel* model;
};

#endif // ITEM_H
