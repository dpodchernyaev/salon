#ifndef BUYDIALOG_H
#define BUYDIALOG_H

#include <QDialog>

#include <QPushButton>

class ItemListView;
class Item;
class ItemModel;
class QLineEdit;

class BuyDialog : public QDialog
{
	Q_OBJECT
public:
	BuyDialog(ItemModel* model, int disc);
	virtual ~BuyDialog();

	Item* getSelected() const;
	bool getStatus() const;

private Q_SLOTS:
	void buySlot();
	void closeSlot();
	void currentChanged(Item*);

private:
	int disc;
	bool status;
	Item* selected;
	ItemListView* view;
	ItemModel* model;
	QPushButton* buyBtn;
	QLineEdit* price;
	QPushButton* closeBtn;
};

#endif // BUYDIALOG_H
