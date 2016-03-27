/* Copyright (c) 2012 Forward Systems. All rights reserved */
#include "gui/background_animation.h"

#include <QMovie>
#include <QPixmap>
#include <QRect>
#include <QPainter>
#include <QPaintEvent>

/** Создает фильм-анимацию
  @param fileName имя файла
*/
BackgroundAnimationMovie::BackgroundAnimationMovie(const QString &fileName):
	QMovie(fileName)
{
	connect(this, SIGNAL(frameChanged(int)),
			this, SLOT(convertFrameChanged(int)));
}

void BackgroundAnimationMovie::convertFrameChanged(int)
{
	Q_EMIT frameChanged();
}


/** Создает анимацию для виджета
  @param displayWidget виджет
*/
BackgroundAnimation::BackgroundAnimation(QWidget *displayWidget):
	m_displayWidget(displayWidget), m_movie(0)
{
	setAnimation("pics/load.gif");
	animationSize = 48;
	transparency = 0.5;
}

BackgroundAnimation::~BackgroundAnimation()
{
	delete m_movie;
}

/**
  Установить файл с анимацией
  @param fileName имя файла
*/
void BackgroundAnimation::setAnimation(const QString &fileName)
{
	m_movie = new BackgroundAnimationMovie(fileName);
	m_animationVisible = true;
	m_movie->connect(m_movie, SIGNAL(frameChanged()),
				m_displayWidget, SLOT(repaint()));
	m_movie->start();
}

/** Запустить анимацию */
void BackgroundAnimation::startAnimation()
{
	if (m_movie)
	{
		m_movie->start();
		m_animationVisible = true;
	}
}

/** Остановить анимацию */
void BackgroundAnimation::stopAnimation()
{
	if (m_movie)
	{
		m_movie->stop();
	}
}

/** Показать анимацию */
void BackgroundAnimation::showAnimation()
{
	m_animationVisible = true;
	m_displayWidget->repaint();

	startAnimation();
}

/** Скрыть анимацию */
void BackgroundAnimation::hideAnimation()
{
	stopAnimation();

	m_animationVisible = false;
	m_displayWidget->repaint();
}

/** Установить размер анимации
  @param size размер
*/
void BackgroundAnimation::setAnimationSize(const int size)
{
	this->animationSize = size;
}

/** Установить прозрачность анимации
  @param value значение
*/
void BackgroundAnimation::setAnimatioTransparency(const float value)
{
	this->transparency = value;
}

/** Отрисовать анимацию
  @param event событие отрисовки
*/
void BackgroundAnimation::paintAnimation(QPaintEvent *event)
{
	if (m_animationVisible && m_movie)
	{
		QPixmap movieFrame = m_movie->currentPixmap().scaledToHeight(
								 animationSize, Qt::SmoothTransformation);
		QRect movieRect = movieFrame.rect();

		movieRect.moveCenter(m_displayWidget->rect().center());

		if (movieRect.intersects(event->rect()))
		{
			QPainter painter(m_displayWidget);
			painter.setOpacity(transparency);
			painter.drawPixmap(movieRect.left(), movieRect.top(), movieFrame);
		}
	}
}
