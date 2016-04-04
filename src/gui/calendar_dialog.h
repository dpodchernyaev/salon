
#pragma once

#include <QDialog>
#include <QDate>

class CalendarWidget;
class ItemTableView;
class SheduleProxyModel;
class SheduleItem;
class SheduleModel;

class CalendarDialog : public QDialog
{
	Q_OBJECT
public:
	CalendarDialog();
	virtual ~CalendarDialog();

	SheduleItem* getSheduleItem() const;
	QDate getDate() const;

private Q_SLOTS:
	void activated(QDate date);
	void okClicked();
	void exitClicked();

private:
	SheduleModel* sModel;
	CalendarWidget* calendar;
	ItemTableView* dayWidget;
	SheduleProxyModel* sProxy;
	QPushButton* okBtn;
};
