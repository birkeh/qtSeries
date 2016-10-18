#include "cseasondetails.h"
#include "ui_cseasondetails.h"

#include "cepisode.h"
#include "cepisodedetails.h"

#include "cmessageanimatedialog.h"

#include <QDebug>


cSeasonDetails::cSeasonDetails(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::cSeasonDetails)
{
	ui->setupUi(this);
}

cSeasonDetails::~cSeasonDetails()
{
	delete ui;
}

void cSeasonDetails::setSeason(cSeason* lpSeason)
{
	for(int x = 0;x < lpSeason->episodeList().count();x++)
	{
		cEpisode*			lpEpisode			= lpSeason->episodeList().at(x);
		cEpisodeDetails*	lpEpisodeDetails	= new cEpisodeDetails(ui->m_lpDetailsEpisodeTab);
		ui->m_lpDetailsEpisodeTab->addTab(lpEpisodeDetails, QString("Episode %1").arg(lpEpisode->episodeNumber()));
		lpEpisodeDetails->setEpisode(lpEpisode);
		if(!x)
			lpEpisodeDetails->loadImages();
	}
}

void cSeasonDetails::on_m_lpDetailsEpisodeTab_tabBarClicked(int index)
{
	cEpisodeDetails*		lpEpisodeDetails	= (cEpisodeDetails*)ui->m_lpDetailsEpisodeTab->widget(index);
	if(!lpEpisodeDetails)
		return;

	lpEpisodeDetails->loadImages();
}
