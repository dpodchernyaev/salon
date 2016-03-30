#ifndef NEWSHEDULEDIALOG_H
#define NEWSHEDULEDIALOG_H

#include <QString>

#include <gui/new_item_dialog.h>

class SheduleModel;
class SheduleWidget;
class SheduleItem;

class NewSheduleDialog : public NewItemDialog
{
	Q_OBJECT
public:
	NewSheduleDialog(SheduleModel* model);

protected:
	virtual void clear();
	virtual Item* createItem();
	virtual void setItem(Item *i);
	virtual bool save();

private:
	void free();


private:
	SheduleWidget* widget;
};

#endif // NEWSHEDULEDIALOG_H
