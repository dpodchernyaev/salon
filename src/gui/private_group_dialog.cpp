
#include <QTimeEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QCalendarWidget>

#include <model/group_item.h>
#include <model/model_factory.h>
#include <model/shedule_model.h>
#include <model/group_model.h>
#include <model/coach_model.h>
#include <model/hall_model.h>
#include <model/vid_model.h>
#include <model/item_proxy_model.h>
#include <gui/item_list_view.h>

#include "private_group_dialog.h"

PrivateGroupDialog::PrivateGroupDialog()
{
	setWindowFlags(windowFlags() | Qt::CustomizeWindowHint);
	setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
	setWindowTitle("Частные группы");
	setMinimumSize(900, 400);
	resize(700, 400);

	newItem = NULL;

	SheduleModel* sModel = (SheduleModel*)ModelFactory::getInstance()->getModel(SHEDULE);
	cModel = (CoachModel*)ModelFactory::getInstance()->getModel(COACH);
	hModel = (HallModel*)ModelFactory::getInstance()->getModel(HALL);
	gModel = (GroupModel*)ModelFactory::getInstance()->getModel(GROUP);

	VidModel* vModel = (VidModel*)ModelFactory::getInstance()->getModel(VID);

	psModel = new SheduleProxyModel(sModel);
	psModel->setSortRole(SortRole);
	psModel->sort(0);

	QSortFilterProxyModel* coachProxyModel = new QSortFilterProxyModel;
	coachProxyModel->setDynamicSortFilter(true);
	coachProxyModel->setSourceModel(cModel);
	coachProxyModel->setSortRole(SortRole);
	coachProxyModel->sort(0);

	pgProxyModel = new PgProxyModel(gModel);
	pgProxyModel->setSortRole(SortRole);
	pgProxyModel->setDynamicSortFilter(true);
	pgProxyModel->sort(0);

	QGridLayout* timeGrid = new QGridLayout;
	int i = 0;

	vidWidget = new QComboBox;
	vidWidget->setModel(vModel);
	timeGrid->addWidget(new QLabel("Вид: "), i, 0);
	timeGrid->addWidget(vidWidget, i++, 1);

	coachWidget = new QComboBox;
	coachWidget->setModel(coachProxyModel);
	timeGrid->addWidget(new QLabel("Тренер: "), i, 0);
	timeGrid->addWidget(coachWidget, i++, 1);

	hallWidget = new QComboBox;
	hallWidget->setModel(hModel);
	timeGrid->addWidget(new QLabel("Зал: "), i, 0);
	timeGrid->addWidget(hallWidget, i++, 1);

	t1 = new QTimeEdit;
	t1->setTime(QTime::currentTime());
	timeGrid->addWidget(new QLabel("Начало: "), i, 0);
	timeGrid->addWidget(t1, i++, 1);

	t2 = new QTimeEdit;
	t2->setTime(QTime::currentTime());
	timeGrid->addWidget(new QLabel("Конец: "), i, 0);
	timeGrid->addWidget(t2, i++, 1);

	calendar = new QCalendarWidget;
	calendar->setMaximumSize(300, 300);
	calendar->setMinimumDate(QDate::currentDate());
	calendar->setMaximumDate(QDate::currentDate().addMonths(2));
	calendar->setFirstDayOfWeek(Qt::Monday);

	shView = new ItemTableView(sModel);
	shView->setProxyModel(psModel);
	shView->horizontalHeader()->hideSection(5);
	shView->setSelectionMode(QAbstractItemView::NoSelection);
	shView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	privateView = new ItemTableView(gModel);
	privateView->setProxyModel(pgProxyModel);
	privateView->setSelectionMode(QAbstractItemView::SingleSelection);
	privateView->setEditTriggers(QAbstractItemView::NoEditTriggers);
	privateView->setSelectionBehavior(QAbstractItemView::SelectRows);

	QPushButton* okBtn = new QPushButton();
	okBtn->setIcon(QIcon("pics/add.png"));
	okBtn->setToolTip("Создать приватное занятие");
	QPushButton* cancelBtn = new QPushButton();
	cancelBtn->setIcon(QIcon("pics/exit.png"));
	cancelBtn->setToolTip("Закрыть окно");
	delBtn = new QPushButton();
	delBtn->setIcon(QIcon("pics/del.png"));
	delBtn->setToolTip("Удалить выбранное приватное занятие");

	QHBoxLayout* btnbox = new QHBoxLayout;
	btnbox->addStretch(1);
	btnbox->addWidget(okBtn);
	btnbox->addWidget(delBtn);
	btnbox->addWidget(cancelBtn);

	QVBoxLayout* leftBox = new QVBoxLayout;
	leftBox->addWidget(calendar);
	leftBox->addLayout(timeGrid);

	QVBoxLayout* rbox = new QVBoxLayout;
	rbox->addWidget(new QLabel("Занятия по расписанию:"), 1, Qt::AlignLeft);
	rbox->addWidget(shView);
	rbox->addWidget(new QLabel("Частные группы:"), 1, Qt::AlignLeft);
	rbox->addWidget(privateView);

	QHBoxLayout* hbox = new QHBoxLayout;
	hbox->addLayout(leftBox);
	hbox->addLayout(rbox);

	QVBoxLayout* mainbox = new QVBoxLayout;
	mainbox->addLayout(hbox);
	mainbox->addLayout(btnbox);

	setLayout(mainbox);

	connect(cancelBtn, SIGNAL(clicked(bool)), this, SLOT(reject()));
	connect(okBtn, SIGNAL(clicked(bool)), this, SLOT(okClicked()));
	connect(delBtn, SIGNAL(clicked(bool)), this, SLOT(delClicked()));

	connect(gModel, SIGNAL(lock(bool)), this, SLOT(groupLocked(bool)));

	connect(calendar, SIGNAL(selectionChanged()), this, SLOT(dayChanged()));

	connect(privateView, SIGNAL(currentChanged(Item*)),
			this, SLOT(privateChanged(Item*)));

	dayChanged();
	privateChanged(privateView->getSelected());
	vidWidget->setCurrentIndex(-1);
	hallWidget->setCurrentIndex(-1);
	coachWidget->setCurrentIndex(-1);
}

PrivateGroupDialog::~PrivateGroupDialog()
{
}

void PrivateGroupDialog::groupLocked(bool f)
{
	setEnabled(!f);
	if (f == true)
	{
		shView->showAnimation();
		privateView->showAnimation();
	}
	else
	{
		shView->hideAnimation();;
		privateView->hideAnimation();

		vidWidget->setCurrentIndex(-1);
		hallWidget->setCurrentIndex(-1);
		coachWidget->setCurrentIndex(-1);

		if (!((ItemModel*)gModel)->getItems().contains(newItem))
		{
			delete newItem;
			newItem = NULL;
		}
	}
}

void PrivateGroupDialog::privateChanged(Item *i)
{
	delBtn->setEnabled(i != NULL);
}

void PrivateGroupDialog::okClicked()
{
	newItem = new GroupItem;
	PrivateGroupParam pp = newItem->getPrivateParam();
	GroupParam p = newItem->getParam();

	QDate date = calendar->selectedDate();
	if (date.isNull() || !date.isValid())
	{
		return;
	}

	int coachId = coachWidget->itemData(coachWidget->currentIndex(), KeyRole).toInt();
	int hallId = hallWidget->itemData(hallWidget->currentIndex(), KeyRole).toInt();
	int vidId = vidWidget->itemData(vidWidget->currentIndex(), KeyRole).toInt();

	pp.used = true;
	p.bdtime = QDateTime(calendar->selectedDate(), t1->time());
	p.etime = t2->time();
	p.cnt = 0;
	p.hall_id = hallId;
	p.coach_id = coachId;
	p.vid_id = vidId;
	newItem->setPrivateParam(pp);
	newItem->setParam(p);

	if (p.hall_id == 0 || p.coach_id == 0 || p.vid_id == 0
			|| t1->time() >= t2->time())
	{
		QMessageBox::warning(NULL, "Предупреждение", "Не все поля заполнены корректно");
		delete newItem;
		newItem = NULL;
		return;
	}

	bool res = !ModelFactory::hasIntersect(newItem);

	if (res == false)
	{
		QMessageBox::warning(NULL, "Предупреждение", "Обнаружено пересечение в расписании");
		delete newItem;
		newItem = NULL;
		return;
	}
	else
	{
		gModel->save(newItem);
	}
}

void PrivateGroupDialog::delClicked()
{
	Item* i = privateView->getSelected();
	if (i != NULL)
	{
		QMessageBox msgBox(
					QMessageBox::Question,
					"Подтверждение удаления",
					"Вы действительно хотите удалить запись?",
					QMessageBox::Yes | QMessageBox::No);
		msgBox.setButtonText(QMessageBox::Yes, trUtf8("Да"));
		msgBox.setButtonText(QMessageBox::No, trUtf8("Нет"));

		int btn = msgBox.exec();

		if (btn == QMessageBox::Yes)
		{
			gModel->deleteItem(i);
		}
	}
}

void PrivateGroupDialog::dayChanged()
{
	QDate date = calendar->selectedDate();
	pgProxyModel->setFilterDate(date);
	psModel->setFilterDate(date);
	psModel->invalidate();
	pgProxyModel->invalidate();
	shView->hideColumn(5);
	delBtn->setEnabled(false);
}
