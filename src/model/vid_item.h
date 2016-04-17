#ifndef VIDITEM_H
#define VIDITEM_H

#include <model/item.h>

struct VidParam
{
	int id;
	QString name;

	VidParam()
	{
		id = 0;
		name = "";
	}
};

class VidItem : public Item
{
public:
	VidItem();
	virtual ~VidItem() {}

	int getId() const;

	void setParam(const VidParam &p);
	VidParam getParam() const;

	virtual QString toString() const;

private:
	VidParam param;
};

#endif // VIDITEM_H
