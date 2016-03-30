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
	virtual void clear();
	virtual bool checkSave() const = 0;
	virtual void save();

	Item* get() const {return item;}

protected:
	Item* item;
};

#endif // ITEMWIDGET_H
