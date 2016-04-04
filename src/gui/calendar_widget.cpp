#include "calendar_widget.h"

CalendarWidget::CalendarWidget()
{

}

CalendarWidget::~CalendarWidget()
{

}

#include <QPen>
#include <QBrush>
#include <QPainter>

#include <model/model_factory.h>
#include <model/shedule_model.h>
#include <model/group_model.h>
void CalendarWidget::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
	QCalendarWidget::paintCell(painter, rect, date);

	SheduleModel* model = (SheduleModel*)ModelFactory::getInstance()->getModel(SHEDULE);

	bool inShedule = model->contains(date);

	if (inShedule)
	{
		QRect r = rect;
		r.translate(-1, -1);
		painter->save();
		QBrush br(QColor(0, 255, 0, 122));
		painter->setBrush(br);
		painter->drawRect(r);
		painter->restore();
	}
}
