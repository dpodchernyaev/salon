#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <QCalendarWidget>

class CalendarWidget : public QCalendarWidget
{
public:
	CalendarWidget();
	virtual ~CalendarWidget();

	virtual void paintCell(QPainter *painter, const QRect &rect, const QDate &date) const;
};

#endif // CALENDARWIDGET_H
