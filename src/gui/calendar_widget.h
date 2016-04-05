#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <QCalendarWidget>

class CsItem;

class CalendarWidget : public QCalendarWidget
{
public:
	CalendarWidget();
	virtual ~CalendarWidget();

	void setVidFilter(int  vid);

protected:
	virtual void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const;

private:
	int vid;
};

#endif // CALENDARWIDGET_H
