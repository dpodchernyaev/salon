

#include <model/card_model.h>
#include <model/coach_model.h>
#include <model/hall_model.h>
#include <model/service_model.h>
#include <model/client_model.h>

#include "model_factory.h"

ModelFactory* ModelFactory::inst = NULL;

ModelFactory::ModelFactory()
{
	models.insert(SERVICE, new ServiceModel);
	models.insert(CARD, new CardModel);
	models.insert(COACH, new CoachModel);
	models.insert(HALL, new HallModel);
	models.insert(CLIENT, new ClientModel);

	getModel(SERVICE)->fetch();
	getModel(CARD)->fetch();
	getModel(COACH)->fetch();
	getModel(HALL)->fetch();
	getModel(CLIENT)->fetch();
}

ModelFactory *ModelFactory::getInstance()
{
	if (inst == NULL)
	{
		inst = new ModelFactory;
	}
	return inst;
}

ItemModel *ModelFactory::getModel(ModelType type) const
{
	return models.value(type);
}

