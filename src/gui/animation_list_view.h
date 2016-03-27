#ifndef ANIMATIONLISTVIEW_H
#define ANIMATIONLISTVIEW_H

#include <gui/background_animation.h>

#include <QListView>

class AnimationListView : public QListView, public BackgroundAnimation
{
public:
	AnimationListView();
	virtual ~AnimationListView() {}

protected:
	void paintEvent(QPaintEvent *e);
};

#endif // ANIMATIONLISTVIEW_H
