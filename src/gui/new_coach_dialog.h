#ifndef NEWCOACHDIALOG_H
#define NEWCOACHDIALOG_H

#include <QString>
#include <QModelIndex>

#include <QDialog>

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

private Q_SLOTS:
	virtual void save();
	virtual void exit();
	virtual void add();
	void currentChanged(QModelIndex ind);

private:
	void free();

private:
	QListView *view;
	QLineEdit* editor;
	QPushButton* newBtn;
	QPushButton* cancelBtn;
	QPushButton* saveBtn;

	QSortFilterProxyModel* pmodel;
	CoachItem* item;
	CoachModel* model;
	CoachWidget* widget;
};

#endif // NEWCOACHDIALOG_H
