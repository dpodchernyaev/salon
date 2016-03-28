#ifndef NEWCOACHDIALOG_H
#define NEWCOACHDIALOG_H

#include <QString>

#include <gui/new_item_dialog.h>

class CoachModel;
class QLineEdit;
class QListView;
class CoachWidget;
class CoachItem;
class QSortFilterProxyModel;

class NewCoachDialog : public NewItemDialog
{
	Q_OBJECT
public:
	NewCoachDialog(CoachModel* model);

protected:
	virtual void clear();
	virtual Item* createItem();
	virtual void setItem(Item *i);
	virtual void save();

private:
	void free();


private:
	CoachWidget* widget;
};

#endif // NEWCOACHDIALOG_H
