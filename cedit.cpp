#include "cedit.h"
#include "ui_cedit.h"

#include "cimage.h"
#include "cseasondetails.h"
#include "cmessageanimatedialog.h"

#include <QRadioButton>
#include <QGroupBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QLineEdit>
#include <QScrollArea>
#include <QDialogButtonBox>
#include <QLabel>
#include <QButtonGroup>

#include <QDebug>
#include <QTime>


cEdit::cEdit(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::cEdit),
	m_bLoaded(false),
	m_lpButton1(0),
	m_lpButton2(0),
	m_lpButton3(0),
	m_lpInit(0),
	m_lpProgress(0),
	m_lpDone(0),
	m_lpGroupBox(0),
	m_lpGridLayout(0),
	m_lpGrid(0),
	m_lpLabel(0),
	m_lpLabel1(0),
	m_lpSpacer(0),
	m_lpGroup(0),
	m_lpAllInit(0),
	m_lpAllProgress(0),
	m_lpAllDone(0),
	m_lpVerticalSpacer(0),
//	m_lpButtonBox(0),
	m_lpSerie(0)
{
	ui->setupUi(this);

	connect(ui->m_lpAllInit, SIGNAL(clicked()), this, SLOT(lpAllInit_clicked()));
	connect(ui->m_lpAllProgress, SIGNAL(clicked()), this, SLOT(lpAllProgress_clicked()));
	connect(ui->m_lpAllDone, SIGNAL(clicked()), this, SLOT(lpAllDone_clicked()));

	ui->m_lpTabWidget->setCurrentIndex(0);
}

cEdit::~cEdit()
{
	if(m_lpSerie)
	{
		int	x;
		QList<cSeason*>	seasonList	= m_lpSerie->seasonList();
		for(x = 0;x < seasonList.count();x++)
		{
			cSeason*	lpSeason	= seasonList.at(x);

			QList<cEpisode*>	episodeList	= lpSeason->episodeList();
			for(int y = 0;y < episodeList.count();y++)
			{
				cEpisode*	lpEpisode	= episodeList.at(y);
				if(lpEpisode)
				{
					lpEpisode->setButton1(0);
					lpEpisode->setButton2(0);
					lpEpisode->setButton3(0);
				}
			}
		}
	}

	delete ui;
}

void cEdit::setSerie(cSerie* lpSerie)
{
	m_lpSerie	= lpSerie;

	if(!lpSerie)
		return;

	ui->m_lpName->setText(lpSerie->seriesName());
    ui->m_lpFirstAired->setDate(lpSerie->firstAired());
	ui->m_lpCliffhanger->setChecked(lpSerie->cliffhanger());
	ui->m_lpDownloadLink->setText(lpSerie->download());

	int	x;
	QList<cSeason*>	seasonList	= lpSerie->seasonList();
	for(x = 0;x < seasonList.count();x++)
	{
		cSeason*	lpSeason	= seasonList.at(x);

		m_lpGroupBox	= new QGroupBox(QString("Season %1").arg(lpSeason->number()), ui->m_lpScrollAreaWidget);
		lpSeason->setGroupBox(m_lpGroupBox);
		m_lpGridLayout	= new QGridLayout(m_lpGroupBox);
		lpSeason->setGridLayout(m_lpGridLayout);
		m_lpGrid		= new QGridLayout();
		lpSeason->setGrid(m_lpGrid);

		m_lpLabel1		= new QLabel("Episode");
		lpSeason->setLabel1(m_lpLabel1);
		m_lpAllInit		= new QPushButton("all Init");
		lpSeason->setAllInit(m_lpAllInit);
		m_lpAllProgress	= new QPushButton("all Prog");
		lpSeason->setAllProgress(m_lpAllProgress);
		m_lpAllDone		= new QPushButton("all Done");
		lpSeason->setAllDone(m_lpAllDone);

		connect(m_lpAllInit, SIGNAL(clicked()), this, SLOT(allInit_clicked()));
		connect(m_lpAllProgress, SIGNAL(clicked()), this, SLOT(allProgress_clicked()));
		connect(m_lpAllDone, SIGNAL(clicked()), this, SLOT(allDone_clicked()));

		m_lpGrid->addWidget(m_lpLabel1, 0, 0, 1, 1);
		m_lpGrid->addWidget(m_lpAllInit, 1, 0, 1, 1);
		m_lpGrid->addWidget(m_lpAllProgress, 2, 0, 1, 1);
		m_lpGrid->addWidget(m_lpAllDone, 3, 0, 1, 1);

		bool				bFirst		= true;
		QList<cEpisode*>	episodeList	= lpSeason->episodeList();
		for(int y = 0;y < episodeList.count();y++)
		{
			cEpisode*	lpEpisode	= episodeList.at(y);

			m_lpGroup	= new QButtonGroup(this);
			lpEpisode->setGroup(m_lpGroup);
			m_lpLabel	= new QLabel(QString("%1").arg(lpEpisode->episodeNumber()), m_lpGroupBox);
			lpEpisode->setLabel(m_lpLabel);

			if(bFirst)
			{
				m_lpButton1	= new QRadioButton("init", m_lpGroupBox);
				m_lpButton2	= new QRadioButton("prog", m_lpGroupBox);
				m_lpButton3	= new QRadioButton("done", m_lpGroupBox);
				bFirst = false;
			}
			else
			{
				m_lpButton1	= new QRadioButton(m_lpGroupBox);
				m_lpButton2	= new QRadioButton(m_lpGroupBox);
				m_lpButton3	= new QRadioButton(m_lpGroupBox);
			}
			lpEpisode->setButton1(m_lpButton1);
			lpEpisode->setButton2(m_lpButton2);
			lpEpisode->setButton3(m_lpButton3);

			m_lpLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

			m_lpButton1->setLayoutDirection(Qt::RightToLeft);
			m_lpButton2->setLayoutDirection(Qt::RightToLeft);
			m_lpButton3->setLayoutDirection(Qt::RightToLeft);

			switch(lpEpisode->state())
			{
			case cEpisode::StateInit:
				m_lpButton1->setChecked(true);
				break;
			case cEpisode::StateProgress:
				m_lpButton2->setChecked(true);
				break;
			case cEpisode::StateDone:
				m_lpButton3->setChecked(true);
				break;
			default:
				break;
			}

			m_lpGroup->addButton(m_lpButton1);
			m_lpGroup->addButton(m_lpButton2);
			m_lpGroup->addButton(m_lpButton3);

			m_lpGrid->addWidget(m_lpLabel, 0, y+1, 1, 1);
			m_lpGrid->addWidget(m_lpButton1, 1, y+1, 1, 1);
			m_lpGrid->addWidget(m_lpButton2, 2, y+1, 1, 1);
			m_lpGrid->addWidget(m_lpButton3, 3, y+1, 1, 1);
		}
		m_lpGridLayout->addLayout(m_lpGrid, 0, 0, 1, 1);
		m_lpSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
		lpSeason->setSpacer(m_lpSpacer);
		m_lpGridLayout->addItem(m_lpSpacer, 0, 1, 1, 1);
		ui->m_lpGridLayoutScroll->addWidget(m_lpGroupBox, x, 0, 1, 1);
	}

	m_lpVerticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	ui->m_lpGridLayoutScroll->addItem(m_lpVerticalSpacer, x+1, 0, 1, 1);
}

void cEdit::lpAllInit_clicked()
{
	m_lpSerie->allInit();
}

void cEdit::lpAllProgress_clicked()
{
	m_lpSerie->allProgress();
}

void cEdit::lpAllDone_clicked()
{
	m_lpSerie->allDone();
}

void cEdit::allInit_clicked()
{
	QPushButton*	lpButton	= (QPushButton*)sender();
	m_lpSerie->seasonInit(lpButton);
}

void cEdit::allProgress_clicked()
{
	QPushButton*	lpButton	= (QPushButton*)sender();
	m_lpSerie->seasonProgress(lpButton);
}

void cEdit::allDone_clicked()
{
	QPushButton*	lpButton	= (QPushButton*)sender();
	m_lpSerie->seasonDone(lpButton);
}

QString cEdit::download()
{
	return(ui->m_lpDownloadLink->text());
}

void cEdit::on_m_lpTabWidget_tabBarClicked(int index)
{
	if(index == 1)
	{
		if(!m_bLoaded)
		{
			cMessageAnimateDialog*	lpDialog	= new cMessageAnimateDialog(this);
			lpDialog->setTitle("Edit");
			lpDialog->setMessage("Loading");
			lpDialog->show();

			cImage	image;
			QPixmap	banner	= image.getImage(m_lpSerie->banner());
			ui->m_lpDetailsBanner->setPixmap(banner);

			ui->m_lpDetailsOverview->setText(m_lpSerie->overview());
			ui->m_lpDetailsSeasonTab->clear();

			QTreeWidgetItem*	lpItem;
			for(int x = 0;x < m_lpSerie->actors().count();x++)
			{
				lpItem	= new QTreeWidgetItem(ui->m_lpDetailsActors);
				lpItem->setText(0, m_lpSerie->actors().at(x));
				ui->m_lpDetailsActors->addTopLevelItem(lpItem);
			}
			for(int x = 0;x < m_lpSerie->genre().count();x++)
			{
				lpItem	= new QTreeWidgetItem(ui->m_lpDetailsGenre);
				lpItem->setText(0, m_lpSerie->genre().at(x));
				ui->m_lpDetailsGenre->addTopLevelItem(lpItem);
			}
			for(int x = 0;x < m_lpSerie->seasonList().count();x++)
			{
				cSeason*		lpSeason		= m_lpSerie->seasonList().at(x);
				cSeasonDetails*	lpSeasonDetails	= new cSeasonDetails(ui->m_lpDetailsSeasonTab);
				ui->m_lpDetailsSeasonTab->addTab(lpSeasonDetails, QString("Season %1").arg(lpSeason->number()));
				lpSeasonDetails->setSeason(lpSeason);
			}
			delete lpDialog;

			m_bLoaded = true;
		}
	}
}

void cEdit::on_m_lpCliffhanger_clicked()
{
	m_lpSerie->setCliffhanger(ui->m_lpCliffhanger->checkState() == Qt::Checked);
}

void cEdit::on_m_lpFirstAired_dateChanged(const QDate &date)
{
    m_lpSerie->setFirstAired(date);
}
