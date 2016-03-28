#ifndef ITEMLISTWIDGET_H
#define ITEMLISTWIDGET_H

#include <QWidget>

class ItemListView;
class QLineEdit;
class ItemModel;
class QPushButton;
class QCompleter;
class QModelIndex;
class Item;

class ItemListWidget : public QWidget
{
	Q_OBJECT
public:
	ItemListWidget(ItemModel *model);
	virtual ~ItemListWidget();

	ItemListView* getView() const;
	void setCurrentIndex(const QModelIndex &sind);

	void showAnimation(bool f);

private Q_SLOTS:
	void editorChanged();
	void clearEditor();
	void refresh();

Q_SIGNALS:
	void currentChanged(Item*);

private:
	QLineEdit* editor;
	QCompleter* completer;
	ItemModel* model;
	ItemListView* view;
	QPushButton* clearBtn;
};

#endif // ITEMLISTWIDGET_H
