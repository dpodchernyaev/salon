
#pragma once

#include <QDialog>
#include <QModelIndex>

#include <gui/new_item_dialog.h>

class ServiceWidget;
class ServiceModel;

class NewServiceDialog : public NewItemDialog
{
	Q_OBJECT
public:
	NewServiceDialog(ServiceModel* model);

protected:
	virtual void clear();
	virtual Item* createItem();
	virtual void setItem(Item *i);
	virtual void save();

private:
	void free();

private:
	ServiceWidget* widget;
};
