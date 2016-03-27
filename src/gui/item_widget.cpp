
#include <model/item.h>

#include "item_widget.h"

ItemWidget::ItemWidget()
{
	item = NULL;
}

ItemWidget::~ItemWidget()
{

}

void ItemWidget::set(Item* item)
{
	this->item = item;
}

void ItemWidget::save()
{
	if (item != NULL)
	{
		item->save();
	}
}
