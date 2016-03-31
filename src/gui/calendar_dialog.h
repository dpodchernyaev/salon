
#pragma once

#include <QDialog>
#include <QDate>

class CalendarWidget;
class ItemTableView;
class SheduleProxyModel;

class CalendarDialog : public QDialog
{
	Q_OBJECT
public:
	CalendarDialog();
	virtual ~CalendarDialog();

private Q_SLOTS:
	void activated(QDate date);
	void okClicked();

private:
	CalendarWidget* calendar;
	ItemTableView* dayWidget;
	SheduleProxyModel* sProxy;
	QPushButton* okBtn;
};
