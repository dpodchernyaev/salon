#include "animation_list_view.h"

AnimationListView::AnimationListView() : BackgroundAnimation(viewport())
{
	hideAnimation();
}

void AnimationListView::paintEvent(QPaintEvent *e)
{
	QListView::paintEvent(e);
	paintAnimation(e);
}
