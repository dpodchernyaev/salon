#ifndef MODELFACTORY_H
#define MODELFACTORY_H

#include <QMap>
#include <QDate>

class ItemModel;

enum ModelType
{
	SERVICE,
	COACH,
	HALL,
	CARD,
	CLIENT,
	CS,
	SHEDULE,
	GROUP,
	VISIT,
	VID
};

class ModelFactory
{
public:
	ModelFactory();
	static ModelFactory* getInstance();

	ItemModel* getModel(ModelType type) const;

	void reload();

	static QString getDay(int id);
	static QString getHall(int id);
	static int getHallCnt(int id);
	static QString getHallByGroup(int id);
	static QString getCoach(int id);
	static QString getService(int id);
	static QString getVid(int id);
	static bool isFull(const QDate &date, int vidId);

private:
	static ModelFactory* inst;
	QMap<ModelType, ItemModel*> models;

};

#endif // MODELFACTORY_H
