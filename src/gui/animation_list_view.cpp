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


AnimationTableView::AnimationTableView() : BackgroundAnimation(viewport())
{
	hideAnimation();
}

void AnimationTableView::paintEvent(QPaintEvent *e)
{
	QTableView::paintEvent(e);
	paintAnimation(e);
}
