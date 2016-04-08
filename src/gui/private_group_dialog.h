#ifndef PRIVATEGROUPDIALOG_H
#define PRIVATEGROUPDIALOG_H

#include <QDialog>

class QCalendarWidget;
class QTimeEdit;
class ItemTableView;
class QDate;
class SheduleProxyModel;
class QComboBox;
class CoachModel;
class HallModel;
class GroupModel;
class PgProxyModel;
class Item;
class GroupItem;

class PrivateGroupDialog : public QDialog
{
	Q_OBJECT
public:
	PrivateGroupDialog();
	virtual ~PrivateGroupDialog();

private Q_SLOTS:
	void okClicked();
	void delClicked();
	void dayChanged();
	void privateChanged(Item*);
	void groupLocked(bool);

private:
	GroupItem* newItem;

	QComboBox* coachWidget;
	QComboBox* hallWidget;
	QComboBox* vidWidget;

	ItemTableView* shView;
	ItemTableView* privateView;
	QCalendarWidget* calendar;
	QTimeEdit* t1;
	QTimeEdit* t2;
	QPushButton* delBtn;

	PgProxyModel* pgProxyModel;
	SheduleProxyModel* psModel;
	CoachModel* cModel;
	GroupModel* gModel;
	HallModel* hModel;
};

#endif // PRIVATEGROUPDIALOG_H
