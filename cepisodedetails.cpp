#include "cepisodedetails.h"
#include "ui_cepisodedetails.h"

#include "cimage.h"
#include "cmessageanimatedialog.h"

#include <QDebug>


cEpisodeDetails::cEpisodeDetails(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::cEpisodeDetails),
	m_lpEpisode(0),
	m_bLoaded(false)
{
	ui->setupUi(this);
}

cEpisodeDetails::~cEpisodeDetails()
{
	delete ui;
}

void cEpisodeDetails::setEpisode(cEpisode* lpEpisode)
{
	m_lpEpisode	= lpEpisode;
	ui->m_lpEpisodeTitle->setText(lpEpisode->episodeName());
	ui->m_lpOverview->setText(lpEpisode->overview());
}

void cEpisodeDetails::loadImages()
{
	if(!m_lpEpisode)
		return;

	if(m_bLoaded)
		return;

	cMessageAnimateDialog*	lpDialog	= new cMessageAnimateDialog(this);
	lpDialog->setTitle("Details");
	lpDialog->setMessage("Loading Images");
	lpDialog->show();

	cImage	image;
	QPixmap	pixmap	= image.getImage(m_lpEpisode->fileName()).scaledToHeight(200);
	ui->m_lpEpisodeThumb->setPixmap(pixmap);

	delete lpDialog;

	m_bLoaded	= true;
}
