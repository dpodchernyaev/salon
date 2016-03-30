
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCompleter>

#include <model/item.h>
#include <model/item_model.h>
#include <model/item_proxy_model.h>

#include <gui/item_list_view.h>
#include "item_list_widget.h"

ItemListWidget::ItemListWidget(ItemModel* model) : model(model)
{
	view = new ItemListView(model);

	clearBtn = new QPushButton;
	clearBtn->setIcon(QIcon("pics/clear_128.png"));
	clearBtn->setToolTip("Очистить строку поиска");

	QPushButton* refreshBtn = new QPushButton;
	refreshBtn->setIcon(QIcon("pics/refresh_128.png"));
	refreshBtn->setToolTip("Обновить");

	editor = new QLineEdit;
	completer = new QCompleter(editor);
	completer->setCompletionMode(QCompleter::InlineCompletion);

	completer->setModel(model);
	completer->setCompletionRole(Qt::DisplayRole);
	editor->setCompleter(completer);

	QHBoxLayout* hbox = new QHBoxLayout;
	hbox->addWidget(editor);
	hbox->addWidget(clearBtn);
	hbox->addWidget(refreshBtn);

	QVBoxLayout* vbox = new QVBoxLayout;
	vbox->addWidget(view);
	vbox->addLayout(hbox);

	setLayout(vbox);

	connect(editor, SIGNAL(textChanged(QString)), this, SLOT(editorChanged()));
	connect(clearBtn, SIGNAL(clicked(bool)), this, SLOT(clearEditor()));
	connect(refreshBtn, SIGNAL(clicked(bool)), this, SLOT(refresh()));

	connect(view, SIGNAL(currentChanged(Item*)),
			this, SIGNAL(currentChanged(Item*)));
}

ItemListWidget::~ItemListWidget()
{
}

ItemListView* ItemListWidget::getView() const
{
	return view;
}

void ItemListWidget::showAnimation(bool f)
{
	if (f == true)
	{
		view->showAnimation();
	}
	else
	{
		view->hideAnimation();
	}
}

void ItemListWidget::setCurrentIndex(const QModelIndex &sind)
{
	ItemProxyModel* pmodel = view->getProxyModel();
	QModelIndex pind = pmodel->mapFromSource(sind);
	view->setCurrentIndex(pind);
}

void ItemListWidget::editorChanged()
{
	clearBtn->setEnabled(!editor->text().isEmpty());
	ItemProxyModel* proxy = view->getProxyModel();
	ItemProxyModel::Filter f;
	f.display = editor->text();
	proxy->setFilter(f);

	QModelIndex first = proxy->index(0, 0);
	if (first.isValid())
	{
		view->setCurrentIndex(first);
	}
}

void ItemListWidget::clearEditor()
{
	editor->clear();
}

void ItemListWidget::refresh()
{
	view->selectionModel()->clear();
	model->fetch();
}
