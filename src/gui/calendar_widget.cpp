
#include <QPen>
#include <QBrush>
#include <QPainter>

#include <model/model_factory.h>
#include <model/shedule_model.h>
#include <model/group_model.h>
#include <model/visit_model.h>
#include <model/group_model.h>

#include "calendar_widget.h"

CalendarWidget::CalendarWidget()
{

}

CalendarWidget::~CalendarWidget()
{

}

void CalendarWidget::setVidFilter(int vid)
{
	this->vid = vid;
}

void CalendarWidget::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
	QCalendarWidget::paintCell(painter, rect, date);

	SheduleModel* model = (SheduleModel*)ModelFactory::getInstance()->getModel(SHEDULE);

	bool inShedule = model->contains(date, vid);
	if (inShedule)
	{
		QColor color = QColor(0, 255, 0, 122);
		if (ModelFactory::isFull(date, vid) == true)
		{
			color = QColor(255, 0, 0, 122);
		}
		QRect r = rect;
		r.translate(-1, -1);
		painter->save();
		QBrush br(color);
		painter->setBrush(br);
		painter->drawRect(r);
		painter->restore();
	}
}
