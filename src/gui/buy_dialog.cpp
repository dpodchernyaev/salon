
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>

#include <model/item.h>
#include <model/item_model.h>

#include <gui/item_list_view.h>

#include "buy_dialog.h"

BuyDialog::BuyDialog(ItemModel *model, int disc) : model(model), disc(disc)
{
	setWindowTitle("Покупка новой услуги");

	view = new ItemListView(model);

	buyBtn = new QPushButton;
	buyBtn->setIcon(QIcon("pics/buy.png"));
	buyBtn->setToolTip("Купить услугу");
	closeBtn = new QPushButton;
	closeBtn->setIcon(QIcon("pics/exit.png"));
	closeBtn->setToolTip("Выйти");

	QLabel* label = new QLabel("Цена со скидкой (" +
							   QString::number(disc) + "%):");
	price = new QLineEdit;
	price->setReadOnly(true);

	QHBoxLayout* priceBox = new QHBoxLayout;
	priceBox->addWidget(label);
	priceBox->addWidget(price);

	QHBoxLayout* btnBox = new QHBoxLayout;
	btnBox->addStretch(0);
	btnBox->addWidget(buyBtn);
	btnBox->addWidget(closeBtn);

	QVBoxLayout* vbox = new QVBoxLayout;
	vbox->addWidget(view);
	vbox->addLayout(priceBox);
	vbox->addLayout(btnBox);

	setLayout(vbox);
	status = false;
	selected = NULL;
	disc = 0;

	connect(view, SIGNAL(currentChanged(Item*)),
			this, SLOT(currentChanged(Item*)));

	connect(buyBtn, SIGNAL(clicked(bool)), this, SLOT(buySlot()));
	connect(closeBtn, SIGNAL(clicked(bool)), this, SLOT(closeSlot()));
}

BuyDialog::~BuyDialog()
{

}

void BuyDialog::currentChanged(Item *s)
{
	selected = s;
	QVariant var = s->data(PriceRole);
	if (var.isValid() && !var.isNull() && var.type() == QVariant::Double)
	{
		double value = var.toDouble() * (1 - (double)disc / 100);
		price->setText(QString::number(value, 'f', 2) + " р.");
	}
}

Item* BuyDialog::getSelected() const
{
	return selected;
}

bool BuyDialog::getStatus() const
{
	return status;
}

void BuyDialog::buySlot()
{
	status = true;
	close();
}

void BuyDialog::closeSlot()
{
	selected = NULL;
	status = false;
	close();
}
