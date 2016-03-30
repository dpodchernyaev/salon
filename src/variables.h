#ifndef VARIABLES_H
#define VARIABLES_H

#include <QDateTime>
#include <QDebug>

#define TIME_FORMAT "hh:mm"
#define DATE_FORMAT "dd.MM.yyyy"
#define DATE_TIME_FORMAT DATE_FORMAT " " TIME_FORMAT
#define MIN_DATE_TIME QDateTime(QDate(1900, 0, 0), QTime(0, 0))
#define MIN_DATE QDate(1900, 0, 0)

static bool isBirthday(const QDate &date)
{
	QDate curr = QDate::currentDate();
	return (curr.day() == date.day() && curr.month() == date.month());
}

#endif // VARIABLES_H
