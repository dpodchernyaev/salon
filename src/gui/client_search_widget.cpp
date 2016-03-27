
#include <QLineEdit>
#include <QCompleter>
#include <QVBoxLayout>
#include <QDebug>
#include <QPushButton>


#include <model/client_item.h>
#include <gui/client_list_view.h>
#include <model/client_model.h>
#include <model/client_proxy_model.h>
#include "client_search_widget.h"

ClientSearchWidget::ClientSearchWidget(ClientListView* view)
	:model(view->getSourceModel()), view(view)
{
	clearBtn = new QPushButton;
	clearBtn->setIcon(QIcon("pics/clear_128.png"));
	clearBtn->setToolTip("Очистить строку поиска");

	QPushButton* refreshBtn = new QPushButton;
	refreshBtn->setIcon(QIcon("pics/refresh_128.png"));
	refreshBtn->setToolTip("Обновить");

	editor = new QLineEdit;
	completer = new QCompleter(editor);

	completer->setModel(model);
	completer->setCompletionRole(Qt::DisplayRole);
	editor->setCompleter(completer);

	QHBoxLayout* hbox = new QHBoxLayout;
	hbox->addWidget(editor);
	hbox->addWidget(clearBtn);
	hbox->addWidget(refreshBtn);

	QVBoxLayout* vbox = new QVBoxLayout;
	vbox->addLayout(hbox);

	setLayout(vbox);

	connect(editor, SIGNAL(textChanged(QString)), this, SLOT(editorChanged()));
	connect(clearBtn, SIGNAL(clicked(bool)), this, SLOT(clearEditor()));
	connect(refreshBtn, SIGNAL(clicked(bool)), this, SLOT(refresh()));
}

ClientSearchWidget::~ClientSearchWidget()
{
}

void ClientSearchWidget::editorChanged()
{
	clearBtn->setEnabled(!editor->text().isEmpty());
	ClientProxyModel* proxy = view->getProxyModel();
	ClientProxyModel::Filter f;
	f.display = editor->text();
	proxy->setFilter(f);

	QModelIndex first = proxy->index(0, 0);
	if (first.isValid())
	{
		view->setCurrentIndex(first);
	}
}

void ClientSearchWidget::clearEditor()
{
	editor->clear();
}

void ClientSearchWidget::refresh()
{
	model->fetch();
}
