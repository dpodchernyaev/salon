/* Copyright (c) 2012 Forward Systems. All rights reserved */

#ifndef BACKGROUND_ANIMATION_H
#define BACKGROUND_ANIMATION_H

#include <QWidget>
#include <QMovie>
#include <QtGui>

class BackgroundAnimationMovie;

/** @class BackgroundAnimation
  * @brief Классс для создания анимация загрузки виджета из gif-файла

	@note
	Для использования нужно:
	1. Унаследоваться от класса BackgroundAnimation
	2. Задать файл с анимацией setAnimation(...), по умолчанию fsqt_resources/load.gif
	3. Переопределить функцию paintEvent, из которой вызывать paintAnimation,
	пример:

	void TreeWidget::paintEvent(QPaintEvent *event)
	{
		QTreeView::paintEvent(event);
		paintAnimation(event);
	}

	3. Использовать функции showAnimation() и hideAnimation()
	для отображения/скрытия анимации.

*/
class BackgroundAnimation
{
public:
	BackgroundAnimation(QWidget *displayWidget);
	virtual ~BackgroundAnimation();
	void setAnimation(const QString &fileName);
	void showAnimation();
	void hideAnimation();
	void setAnimationSize(const int size);
	void setAnimatioTransparency(const float value);

protected:
	QWidget *m_displayWidget; 			///< Виджет для отрисовки
	bool m_animationVisible; 			///< Видимость анимации
	BackgroundAnimationMovie *m_movie; 	///< Анимация загрузки
	void paintAnimation(QPaintEvent *event);

private:
	void startAnimation();
	void stopAnimation();

	int animationSize;		///< Размер
	float transparency;		///< Прозрачность
};


/** @class BackgroundAnimationMovie
  * @brief Анимация загрузки
*/
class BackgroundAnimationMovie: public QMovie
{
	Q_OBJECT

Q_SIGNALS:
	void frameChanged();

public:
	BackgroundAnimationMovie(const QString &fileName);

private Q_SLOTS:
	void convertFrameChanged(int);
};

#endif
