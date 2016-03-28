#ifndef NEWCLIENTDIALOG_H
#define NEWCLIENTDIALOG_H

#include <gui/new_item_dialog.h>

class ClientModel;
class ItemWidget;

class NewClientDialog : public NewItemDialog
{
	Q_OBJECT
public:
	NewClientDialog(ClientModel* model);

protected:
	virtual void clear();
	virtual Item* createItem();
	virtual void setItem(Item *i);
	virtual void save();

private:
	void free();

private:
	ItemWidget* widget;
};

#endif // NEWCLIENTDIALOG_H
