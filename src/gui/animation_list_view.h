#ifndef ANIMATIONLISTVIEW_H
#define ANIMATIONLISTVIEW_H

#include <gui/background_animation.h>

#include <QListView>
#include <QTableView>

class AnimationListView : public QListView, public BackgroundAnimation
{
public:
	AnimationListView();
	virtual ~AnimationListView() {}

protected:
	void paintEvent(QPaintEvent *e);
};

class AnimationTableView : public QTableView, public BackgroundAnimation
{
public:
	AnimationTableView();
	virtual ~AnimationTableView() {}

protected:
	void paintEvent(QPaintEvent *e);
};

#endif // ANIMATIONLISTVIEW_H
