
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <config/config.h>
#include <db/dbconnection.h>
#include <db/db_service.h>

#include "client_report.h"

ClientReport::ClientReport(int clId) : clId(clId)
{
	setWindowTitle("Отчет по посещениям клиента");
	QPushButton* okBtn = new QPushButton("Создать");
	QPushButton* cancelBtn = new QPushButton("Отмена");

	QVBoxLayout* vbox = new QVBoxLayout;

	QLabel* label = new QLabel("Начало:");
	t1 = new QDateEdit(QDate::currentDate().addMonths(-1));
	vbox->addWidget(label);
	vbox->addWidget(t1);

	label = new QLabel("Конец:");
	t2 = new QDateEdit(QDate::currentDate());
	vbox->addWidget(label);
	vbox->addWidget(t2);

	QHBoxLayout* bbox = new QHBoxLayout;
	bbox->addWidget(okBtn);
	bbox->addWidget(cancelBtn);

	QVBoxLayout* mb = new QVBoxLayout;
	mb->addLayout(vbox);
	mb->addLayout(bbox);

	setLayout(mb);

	connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(reject()));
	connect(okBtn, SIGNAL(clicked(bool)), this, SLOT(ok()));
}

ClientReport::~ClientReport()
{

}


void ClientReport::ok()
{
	QString sql = "SELECT client.name, client.surname, client.patronymic, client.birthday"
						", client_service.name, visit.dtime, visit.info"
				  " FROM client, client_service, visit"
				  " WHERE client.id = ?"
						" AND client.id = client_service.client_id"
						" AND client_service.id = visit.client_service_id"
						" AND visit.dtime BETWEEN ? AND ?"
				  " ORDER BY visit.dtime";

	DBConn* conn = DBService::getInstance()->getConnection();

	conn->beginTransaction();

	QSqlQuery q(conn->qtDatabase());
	q.prepare(sql);
	q.bindValue(0, clId);
	q.bindValue(1, t1->date());
	q.bindValue(2, t2->date());
	bool res = conn->executeQuery(q);


	if (res == false)
	{
		QMessageBox::critical(NULL, "Ошибка", "Ошибка составления отчета");
		conn->commit();
		return;
	}

	QString dirStr = Config::getInstance()->getValue(REPORT_DIR).toString();
	QDir dir(dirStr);
	if (!dir.exists())
	{
		QMessageBox::critical(NULL, "Ошибка", "Нет заданного каталога с отчетами");
		conn->commit();
		return;
	}

	QString path = dir.absoluteFilePath("client_report.txt");
	QFile file(path);
	if (file.open(QFile::WriteOnly | QFile::Truncate) == false)
	{
		QMessageBox::critical(NULL, "Ошибка", "Ошибка открытия файла отчета для записи");
		conn->commit();
		return;
	}

	QTextStream stream(&file);

	bool first = true;
	while (q.next())
	{
		if (first == true)
		{
			stream << q.value(1).toString() << " " << q.value(0).toString()
				   << " " << q.value(2).toString()
				   << " (" << q.value(3).toString() << ")";
			first = false;
		}
		stream << endl << "\t" << q.value(5).toDateTime().toString("dd.MM.yyyy hh.mm")
			   << " - " << q.value(6).toString() << " - (" << q.value(4).toString() << ")";
	}

	file.close();

	QMessageBox::information(NULL, "Выполнено", "Отчет сформирован - " + path);
	conn->commit();

	accept();
}
