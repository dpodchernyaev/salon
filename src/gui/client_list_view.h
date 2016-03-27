#ifndef CLIENTLISTVIEW_H
#define CLIENTLISTVIEW_H

#include <gui/background_animation.h>

#include <gui/animation_list_view.h>

class ClientModel;
class ClientProxyModel;
class ClientItem;

class ClientListView : public AnimationListView
{
	Q_OBJECT
public:
	ClientListView();
	virtual ~ClientListView();

	ClientModel* getSourceModel() const;
	ClientProxyModel* getProxyModel() const;
	QModelIndex mapFromSource(const QModelIndex &index) const;

protected:
	void currentChanged(const QModelIndex &current, const QModelIndex &previous);

Q_SIGNALS:
	void currentChanged(ClientItem*);

private:
	ClientModel* model;
	ClientProxyModel* proxyModel;
};

#endif // CLIENTLISTVIEW_H
