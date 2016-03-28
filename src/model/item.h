#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include <QHash>

class ItemModel;

class Item
{
public:
	Item();
	virtual ~Item();

	void setModel(ItemModel* model);
	void save();

	int getId() const;

	virtual QString hash() const = 0;

	static Item* getItem(const QString &hash);
	static Item* addToHash(Item*);

protected:
	static QHash<QString, Item*> hashItems;
	ItemModel* model;
};

#endif // ITEM_H
