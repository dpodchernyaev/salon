
#pragma once

#include <QString>

#include <gui/new_item_dialog.h>

class HallModel;
class QListView;
class HallItem;
class HallWidget;
class QSortFilterProxyModel;

class NewHallDialog : public NewItemDialog
{
	Q_OBJECT
public:
	NewHallDialog(HallModel* model);

protected:
	virtual void clear();
	virtual Item* createItem();
	virtual void setItem(Item *i);
	virtual bool save();

private:
	void free();


private:
	HallWidget* widget;
};
