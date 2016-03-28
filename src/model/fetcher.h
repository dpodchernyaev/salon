#ifndef FETCHER_H
#define FETCHER_H

#include <QObject>
#include <QList>

class Item;

class Fetcher : public QObject
{
	Q_OBJECT
public:
	Fetcher();
	virtual ~Fetcher();

	void fetch();
	void save(Item* item);
	void deleteItem(int);

private Q_SLOTS:
	virtual void fetchSlot() = 0;
	virtual void saveSlot(Item* item) = 0;
	virtual void deleteSlot(int) {}

Q_SIGNALS:
	void fetchSignal();
	void saveSignal(Item*);
	void deleteSignal(int);
	void fetched(QList<Item*> items);
	void saved(bool);
	void deleted(bool);
};

#endif // FETCHER_H
