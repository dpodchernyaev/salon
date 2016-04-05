#ifndef CLIENTREPORT_H
#define CLIENTREPORT_H

#include <QDateEdit>
#include <QDialog>

class ClientReport : public QDialog
{
	Q_OBJECT
public:
	ClientReport(int clId);
	virtual ~ClientReport();

private Q_SLOTS:
	void ok();

private:
	QDateEdit* t1;
	QDateEdit* t2;

	int clId;
};

#endif // CLIENTREPORT_H
