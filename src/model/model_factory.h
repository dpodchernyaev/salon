#ifndef MODELFACTORY_H
#define MODELFACTORY_H

#include <QMap>

class ItemModel;

enum ModelType
{
	SERVICE,
	COACH,
	HALL,
	CARD,
	CLIENT
};

class ModelFactory
{
public:
	ModelFactory();
	static ModelFactory* getInstance();

	ItemModel* getModel(ModelType type) const;

private:
	static ModelFactory* inst;
	QMap<ModelType, ItemModel*> models;

};

#endif // MODELFACTORY_H
