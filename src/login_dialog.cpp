
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>

#include <db/db_service.h>
#include <db/dbconnection.h>

#include "login_dialog.h"

LoginDialog::LoginDialog()
{
	setFixedSize(250, 150);
	QPushButton* okBtn = new QPushButton("Ок");
	QPushButton* cBtn = new QPushButton("Отмена");

	QLabel* label = NULL;
	QGridLayout* g = new QGridLayout;

	label = new QLabel("Логин: ");
	name = new QLineEdit;
	g->addWidget(label, 0, 0);
	g->addWidget(name, 0, 1);

	label = new QLabel("Пароль: ");
	passwd = new QLineEdit;
	passwd->setEchoMode(QLineEdit::Password);
	g->addWidget(label, 1, 0);
	g->addWidget(passwd, 1, 1);


	QHBoxLayout* btnBox = new QHBoxLayout;
	btnBox->addStretch(1);
	btnBox->addWidget(okBtn);
	btnBox->addWidget(cBtn);

	QVBoxLayout* vbox = new QVBoxLayout;
	vbox->addLayout(g);
	vbox->addLayout(btnBox);


	setLayout(vbox);

	connect(cBtn, SIGNAL(clicked(bool)), this, SLOT(reject()));
	connect(okBtn, SIGNAL(clicked(bool)), this, SLOT(ok()));
}

void LoginDialog::ok()
{
	DBConn* conn = DBService::getInstance()->getConnection();
	if (!conn->isConnected())
	{
		QMessageBox::critical(NULL, "Ошибка БД", "Нет подключения к БД");
		reject();
	}

	QString sql =
			QString("SELECT count(*) FROM users WHERE user_name = \'%1\' AND password = \'%2\'")
			.arg(name->text(), passwd->text());
	QSqlQuery q = conn->executeQuery(sql);
	bool res = false;
	if (q.isActive() && q.next())
	{
		res = q.value(0).toInt() > 0;
	}

	if (res == false)
	{
		QMessageBox::warning(NULL, "Предупреждение", "Неверный пароль");
	}
	else
	{
		accept();
	}
}
