#ifndef VISITITEM_H
#define VISITITEM_H

#include <QDateTime>
#include <variables.h>

#include <model/item.h>

class GroupItem;
class CsItem;

struct VisitParam
{
	int id;
	int cs_id;
	int vgroup_id;
	QString info;
	QDateTime dtime;

	VisitParam()
	{
		id = 0;
		cs_id = 0;
		vgroup_id = 0;
		info = "";
		dtime = MIN_DATE_TIME;
	}

};

class VisitItem : public Item
{
public:
	VisitItem();

	virtual int getId() const;
	void setParam(const VisitParam &p);
	VisitParam getParam() const;

	void setGroup(GroupItem* g);
	GroupItem* getGroup() const;

	void setCs(CsItem* cs);
	CsItem* getCs() const;

	virtual QString toString() const;

private:
	VisitParam param;
	GroupItem *group;
	CsItem* csItem;
};

#endif // VISITITEM_H
