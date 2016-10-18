#include "cseason.h"

#define DELETE(x) { if(x) delete x; x=0; }

cSeason::cSeason() :
	m_iNumber(-1),
	m_lpGroupBox(0),
	m_lpGridLayout(0),
	m_lpGrid(0),
	m_lpLabel1(0),
	m_lpAllInit(0),
	m_lpAllProgress(0),
	m_lpAllDone(0),
	m_lpSpacer(0),
	m_lpSerie(0)

{
}

cSeason::~cSeason()
{
	deleteResources();
}

void cSeason::setNumber(const qint16& iNumber)
{
	m_iNumber	= iNumber;
}

qint16 cSeason::number()
{
	return(m_iNumber);
}

void cSeason::setSerie(cSerie* lpSerie)
{
	m_lpSerie	= lpSerie;
}

cSerie* cSeason::serie()
{
	return(m_lpSerie);
}

cEpisode* cSeason::addEpisode(qint16 iNumber)
{
	cEpisode*	lpNew	= new cEpisode;
	lpNew->setEpisodeNumber(iNumber);
	lpNew->setState(cEpisode::StateInit);
	m_episodeList.append(lpNew);
	return(lpNew);
}

cEpisode* cSeason::addEpisode(cEpisode* lpEpisode)
{
	if(episodeList().contains(lpEpisode))
		return(0);
	m_episodeList.append(lpEpisode);
	return(lpEpisode);
}

QList<cEpisode*> cSeason::episodeList()
{
	return(m_episodeList);
}

qint16 cSeason::episodeCount()
{
	qint16	iTotal = -1;
	for(int z = 0;z < m_episodeList.count();z++)
	{
		cEpisode*	lpEpisode	= m_episodeList.at(z);
		if(lpEpisode)
		{
			if(lpEpisode->episodeNumber() > iTotal)
				iTotal	= lpEpisode->episodeNumber();
		}
	}
	return(iTotal);
}

void cSeason::setGroupBox(QGroupBox* lpGroupBox)
{
	m_lpGroupBox	= lpGroupBox;
}

void cSeason::setGridLayout(QGridLayout* lpGridLayout)
{
	m_lpGridLayout	= lpGridLayout;
}

void cSeason::setGrid(QGridLayout* lpGrid)
{
	m_lpGrid	= lpGrid;
}

void cSeason::setLabel1(QLabel* lpLabel1)
{
	m_lpLabel1	= lpLabel1;
}

void cSeason::setAllInit(QPushButton* lpAllInit)
{
	m_lpAllInit	= lpAllInit;
}

void cSeason::setAllProgress(QPushButton* lpAllProgress)
{
	m_lpAllProgress	= lpAllProgress;
}

void cSeason::setAllDone(QPushButton* lpAllDone)
{
	m_lpAllDone	= lpAllDone;
}

void cSeason::setSpacer(QSpacerItem* lpSpacer)
{
	m_lpSpacer	= lpSpacer;
}

void cSeason::deleteResources()
{
	DELETE(m_lpGroupBox);
	DELETE(m_lpGridLayout);
	DELETE(m_lpGrid);
	DELETE(m_lpLabel1);
	DELETE(m_lpAllInit);
	DELETE(m_lpAllProgress);
	DELETE(m_lpAllDone);
	DELETE(m_lpSpacer);

	for(int z = 0;z < m_episodeList.count();z++)
		m_episodeList.at(z)->deleteResources();
}

void cSeason::updateState()
{
	for(int z = 0;z < m_episodeList.count();z++)
		m_episodeList.at(z)->updateState();
}

void cSeason::allInit()
{
	for(int z = 0;z < m_episodeList.count();z++)
		m_episodeList.at(z)->setState(cEpisode::StateInit);
}

void cSeason::allProgress()
{
	for(int z = 0;z < m_episodeList.count();z++)
		m_episodeList.at(z)->setState(cEpisode::StateProgress);
}

void cSeason::allDone()
{
	for(int z = 0;z < m_episodeList.count();z++)
		m_episodeList.at(z)->setState(cEpisode::StateDone);
}

QPushButton* cSeason::allInitButton()
{
	return(m_lpAllInit);
}

QPushButton* cSeason::allProgressButton()
{
	return(m_lpAllProgress);
}

QPushButton* cSeason::allDoneButton()
{
	return(m_lpAllDone);
}
