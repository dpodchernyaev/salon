#ifndef NEW_ITEM_DIALOG_H
#define NEW_ITEM_DIALOG_H

#include <QDialog>
#include <QModelIndex>

class QHBoxLayout;
class QVBoxLayout;
class QSortFilterProxyModel;
class QListView;

class ItemModel;

class NewItemDialog : public QDialog
{
	Q_OBJECT
public:
	NewItemDialog(ItemModel* model);
	virtual ~NewItemDialog() {}

protected:
	QHBoxLayout* btnBox;
	QVBoxLayout* leftBox;
	QVBoxLayout* rightBox;

	ItemModel* model;
	QSortFilterProxyModel* pmodel;

protected Q_SLOTS:
	virtual void currentChanged(QModelIndex);

	virtual void add();
	virtual void del();
	virtual void exit();
	virtual void edit();
	virtual void save();
	virtual void cancel();

private:
	QWidget* rightWidget;
	QListView* view;
	QPushButton* addBtn;
	QPushButton* editBtn;
	QPushButton* saveBtn;
	QPushButton* delBtn;
	QPushButton* exitBtn;
	QPushButton* cancelBtn;
};

#endif // NEW_ITEM_DIALOG_H
