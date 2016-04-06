
#pragma once

#include <QDialog>
#include <QDate>

class CalendarWidget;
class ItemTableView;
class SheduleProxyModel;
class SheduleItem;
class SheduleModel;
class VidModel;
class CsItem;
class GroupModel;
class QComboBox;

class CalendarDialog : public QDialog
{
	Q_OBJECT
public:
	CalendarDialog();
	virtual ~CalendarDialog();

	SheduleItem* getSheduleItem() const;
	QDate getDate() const;

	void setFilterItem(CsItem* item);

private Q_SLOTS:
	void vidChanged(int ind);
	void activated(QDate date);
	void okClicked();
	void exitClicked();

private:
	QComboBox *vidWidget;
	VidModel* vModel;
	SheduleModel* sModel;
	GroupModel* gModel;
	CalendarWidget* calendar;
	ItemTableView* dayWidget;
	SheduleProxyModel* sProxy;
	QPushButton* okBtn;
};
