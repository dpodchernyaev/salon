#ifndef VARIABLES_H
#define VARIABLES_H

#include <QDateTime>

#define DATE_FORMAT "dd.MM.yyyy"
#define DATE_TIME_FORMAT DATE_FORMAT " hh:mm:ss"
#define MIN_DATE_TIME QDateTime(QDate(1900, 0, 0), QTime(0, 0))
#define MIN_DATE QDate(1900, 0, 0)

#endif // VARIABLES_H
