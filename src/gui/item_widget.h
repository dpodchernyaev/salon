#ifndef ITEMWIDGET_H
#define ITEMWIDGET_H

#include <QWidget>

class Item;

class ItemWidget : public QWidget
{
public:
	ItemWidget();
	virtual ~ItemWidget();
	virtual void set(Item* item);
	virtual void apply() = 0;
	virtual void cancel() = 0;
	virtual void save();

protected:
	Item* item;
};

#endif // ITEMWIDGET_H
