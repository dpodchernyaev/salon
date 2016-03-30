
#include <QDebug>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QDateEdit>
#include <QCheckBox>

#include <model/card_model.h>
#include <model/model_factory.h>
#include <model/client_item.h>
#include <gui/photo_widget.h>

#include "client_widget.h"

ClientWidget::ClientWidget()
{
	item = NULL;
	photo = new PhotoWidget;
	photo->setFixedSize(150, 200);
	photo->setPhoto("pics/no_photo.png");

	QGridLayout* grid = new QGridLayout;

	int row = 0;

	QLabel* label = new QLabel("Фамилия: ");
	surnameWidget = new QLineEdit;
	grid->addWidget(label, row, 0);
	grid->addWidget(surnameWidget, row++, 1);

	label = new QLabel("Имя: ");
	nameWidget = new QLineEdit;
	nameWidget->setMinimumWidth(300);
	grid->addWidget(label, row, 0);
	grid->addWidget(nameWidget, row++, 1);

	label = new QLabel("Отчество: ");
	patronymicWidget = new QLineEdit;
	grid->addWidget(label, row, 0);
	grid->addWidget(patronymicWidget, row++, 1);

	label = new QLabel("День рождения: ");
	birthdayWidget = new QDateEdit;
	birthdayWidgetEmpty = new QLineEdit;
	birthdayWidget->setCalendarPopup(true);
	birthdayWidget->setDate(MIN_DATE);
	grid->addWidget(label, row, 0);
	grid->addWidget(birthdayWidgetEmpty, row, 1);
	grid->addWidget(birthdayWidget, row++, 1);

	label = new QLabel("Телефон: ");
	phoneWidget = new QLineEdit;
	grid->addWidget(label, row, 0);
	grid->addWidget(phoneWidget, row++, 1);


	cardType = new QComboBox;
	cardType->setModel(ModelFactory::getInstance()->getModel(CARD));

	cardNumberWidget = new QSpinBox;
	cardNumberWidget->setMaximum(1000000);
	cardNumberWidget->setMaximumWidth(120);

	QHBoxLayout* cardBox = new QHBoxLayout;
	cardBox->addWidget(cardNumberWidget);
	cardBox->addWidget(cardType);

	label = new QLabel("Карта: ");
	grid->addWidget(label, row, 0);
	grid->addLayout(cardBox, row++, 1);

	label = new QLabel("email: ");
	emailWidget = new QLineEdit;
	grid->addWidget(label, row, 0);
	grid->addWidget(emailWidget, row++, 1);

	notifyWidget = new QCheckBox("Оповещать по email");
	grid->addWidget(notifyWidget, row++, 1);

	QVBoxLayout* vbox = new QVBoxLayout;
	vbox->addLayout(grid);
	vbox->addStretch(1);

	QHBoxLayout* hbox = new QHBoxLayout;
	hbox->addWidget(photo);
	hbox->addLayout(vbox);
	hbox->addStretch(1);

	setLayout(hbox);

	birthdayWidgetEmpty->setVisible(false);
}

bool ClientWidget::checkSave() const
{
	bool res = true;
	if (nameWidget->text().isEmpty())
	{
		res = false;
	}
	else if (surnameWidget->text().isEmpty())
	{
		res = false;
	}
	else if (birthdayWidget->date() == MIN_DATE)
	{
		res = false;
	}

	return res;
}

void ClientWidget::set(Item* item)
{
	ItemWidget::set(item);
	setClient((ClientItem*) item);
}

void ClientWidget::setClient(ClientItem *item)
{
	this->item = item;
	if (item == NULL)
	{
		photo->setPhoto("");
		nameWidget->setText("");
		surnameWidget->setText("");
		patronymicWidget->setText("");
		emailWidget->setText("");
		phoneWidget->setText("");
		birthdayWidgetEmpty->setVisible(true);
		birthdayWidget->setVisible(false);
		notifyWidget->setCheckState(Qt::Unchecked);
		cardType->setCurrentIndex(-1);
		cardNumberWidget->setValue(0);
	}
	else
	{
		ClientParam p = item->get();
		if (p.photo.isEmpty())
		{
			photo->setName(
						p.surname + "_" +
						p.name + "_" +
						p.patronymic + "_" +
						p.birthday.toString(DATE_FORMAT));
		}
		else
		{
			photo->setName(p.photo);
		}
		nameWidget->setText(p.name);
		surnameWidget->setText(p.surname);
		patronymicWidget->setText(p.patronymic);
		emailWidget->setText(p.email);
		phoneWidget->setText(p.phone);
		birthdayWidget->setDate(p.birthday.date());
		birthdayWidgetEmpty->setVisible(false);
		birthdayWidget->setVisible(true);
		notifyWidget->setCheckState(p.notify ? Qt::Checked : Qt::Unchecked);

		CardModel* cm = (CardModel*)ModelFactory::getInstance()->getModel(CARD);
		cardType->setCurrentIndex(cm->indexOf(p.card_id));
		cardNumberWidget->setValue(p.cardNumber);
	}
}

void ClientWidget::apply()
{
	if (item != NULL)
	{
		ClientItem* c = (ClientItem*)item;
		ClientParam p = c->get();
		p.name = nameWidget->text();
		p.surname = surnameWidget->text();
		p.patronymic = patronymicWidget->text();
		p.birthday = QDateTime(birthdayWidget->date(), QTime());
		p.email = emailWidget->text();
		p.phone = phoneWidget->text();
		p.photo = photo->getName();
		p.notify = notifyWidget->checkState() == Qt::Checked ? true : false;

		p.cardNumber = cardNumberWidget->value();
		p.card_id = cardType->itemData(cardType->currentIndex(), KeyRole).toInt();
		c->set(p);
	}
}

void ClientWidget::save()
{
	ItemWidget::save();
	if (item != NULL)
	{
		photo->save();
	}
}

void ClientWidget::reset()
{
	set(item);
}

void ClientWidget::edit(bool flag)
{
	nameWidget->setReadOnly(!flag);
	surnameWidget->setReadOnly(!flag);
	patronymicWidget->setReadOnly(!flag);
	emailWidget->setReadOnly(!flag);
	phoneWidget->setReadOnly(!flag);
	birthdayWidget->setReadOnly(!flag);
	photo->setReadOnly(!flag);
	notifyWidget->setEnabled(flag);
	cardType->setEnabled(flag);
	cardNumberWidget->setReadOnly(!flag);
}
