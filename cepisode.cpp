#include "cepisode.h"


#define DELETE(x) { if(x) delete x;x = 0;}

cEpisode::cEpisode() :
	m_iID(-1),
	m_szDirector(""),
	m_szEpisodeName(""),
	m_iEpisodeNumber(-1),
	m_firstAired(QDate(1900, 1, 1)),
	m_szGuestStars(""),
	m_szIMDBID(""),
	m_szLanguage(""),
	m_szOverview(""),
	m_szProductionCode(""),
	m_dRating(-1.0),
	m_iRatingCount(-1),
	m_iSeasonNumber(-1),
	m_szWriter(""),
	m_iSeasonID(-1),
	m_iSeriesID(-1),
	m_state(cEpisode::StateInit),
	m_lpGroup(0),
	m_lpLabel(0),
	m_lpButton1(0),
	m_lpButton2(0),
	m_lpButton3(0)
{
}

cEpisode::~cEpisode()
{
	deleteResources();
}

void cEpisode::setID(const qint32& iID)
{
	m_iID	= iID;
}

qint32 cEpisode::id()
{
	return(m_iID);
}

void cEpisode::setDirector(const QString& szDirector)
{
	m_szDirector	= szDirector.split("|");
	m_szDirector.removeAll("");
}

void cEpisode::setDirector(const QStringList& szDirector)
{
	m_szDirector	= szDirector;
	m_szDirector.removeAll("");
}

QStringList cEpisode::director()
{
	return(m_szDirector);
}

void cEpisode::setEpisodeName(const QString& szEpisodeName)
{
	m_szEpisodeName	= szEpisodeName;
}

QString cEpisode::episodeName()
{
	return(m_szEpisodeName);
}

void cEpisode::setEpisodeNumber(const qint16& iEpisodeNumber)
{
	m_iEpisodeNumber	= iEpisodeNumber;
}

qint16 cEpisode::episodeNumber()
{
	return(m_iEpisodeNumber);
}

void cEpisode::setFirstAired(const QString& szFirstAired)
{
	m_firstAired	= QDate::fromString(szFirstAired, "yyyy-MM-dd");
}

void cEpisode::setFirstAired(const QDate& firstAired)
{
	m_firstAired	= firstAired;
}

QDate cEpisode::firstAired()
{
	return(m_firstAired);
}

void cEpisode::setGuestStars(const QString& szGuestStars)
{
	m_szGuestStars	= szGuestStars.split("|");
	m_szGuestStars.removeAll("");
}

void cEpisode::setGuestStars(const QStringList& szGuestStars)
{
	m_szGuestStars	= szGuestStars;
	m_szGuestStars.removeAll("");
}

QStringList cEpisode::guestStars()
{
	return(m_szGuestStars);
}

void cEpisode::setIMDBID(const QString& szIMDBID)
{
	m_szIMDBID	= szIMDBID;
}

QString cEpisode::imdbID()
{
	return(m_szIMDBID);
}

void cEpisode::setLanguage(const QString& szLanguage)
{
	m_szLanguage	= szLanguage;
}

QString cEpisode::language()
{
	return(m_szLanguage);
}

void cEpisode::setOverview(const QString& szOverview)
{
	m_szOverview	= szOverview;
}

QString cEpisode::overview()
{
	return(m_szOverview);
}

void cEpisode::setProductionCode(const QString& szProductionCode)
{
	m_szProductionCode	= szProductionCode;
}

QString cEpisode::productionCode()
{
	return(m_szProductionCode);
}

void cEpisode::setRating(const qreal& dRating)
{
	m_dRating	= dRating;
}

qreal cEpisode::rating()
{
	return(m_dRating);
}

void cEpisode::setRatingCount(const qint32& iRatingCount)
{
	m_iRatingCount	= iRatingCount;
}

qint32 cEpisode::ratingCount()
{
	return(m_iRatingCount);
}

void cEpisode::setSeasonNumber(const qint16& iSeasonNumber)
{
	m_iSeasonNumber	= iSeasonNumber;
}

qint16 cEpisode::seasonNumber()
{
	return(m_iSeasonNumber);
}

void cEpisode::setWriter(const QString& szWriter)
{
	m_szWriter	= szWriter.split("|");
	m_szWriter.removeAll("");
}

void cEpisode::setWriter(const QStringList& szWriter)
{
	m_szWriter	= szWriter;
	m_szWriter.removeAll("");
}

QStringList cEpisode::writer()
{
	return(m_szWriter);
}

void cEpisode::setSeasonID(const qint32& iSeasonID)
{
	m_iSeasonID	= iSeasonID;
}

qint32 cEpisode::seasonID()
{
	return(m_iSeasonID);
}

void cEpisode::setSeriesID(const qint32& iSeriesID)
{
	m_iSeriesID	= iSeriesID;
}

qint32 cEpisode::seriesID()
{
	return(m_iSeriesID);
}

void cEpisode::setState(const State& state)
{
	m_state		= state;
	switch(m_state)
	{
	case StateInit:
		if(m_lpButton1)
			m_lpButton1->setChecked(true);
		if(m_lpButton2)
			m_lpButton2->setChecked(false);
		if(m_lpButton3)
			m_lpButton3->setChecked(false);
		break;
	case StateProgress:
		if(m_lpButton2)
			m_lpButton2->setChecked(true);
		if(m_lpButton1)
			m_lpButton1->setChecked(false);
		if(m_lpButton3)
			m_lpButton3->setChecked(false);
		break;
	case StateDone:
		if(m_lpButton3)
			m_lpButton3->setChecked(true);
		if(m_lpButton1)
			m_lpButton1->setChecked(false);
		if(m_lpButton2)
			m_lpButton2->setChecked(false);
		break;
	default:
		break;
	}
}

cEpisode::State cEpisode::state()
{
	return(m_state);
}

void cEpisode::setFileName(const QString& szFileName)
{
	m_szFileName	= szFileName;
}

QString cEpisode::fileName()
{
	return(m_szFileName);
}

void cEpisode::setThumbHeight(const qint16& iThumbHeight)
{
	m_iThumbHeight	= iThumbHeight;
}

qint16 cEpisode::thumbHeight()
{
	return(m_iThumbHeight);
}

void cEpisode::setThumbWidth(const qint16& iThumbWidth)
{
	m_iThumbWidth	= iThumbWidth;
}

qint16 cEpisode::thumbWidth()
{
	return(m_iThumbWidth);
}

bool cEpisode::isValid()
{
	if(m_iSeasonID != -1 && m_iEpisodeNumber != -1 && m_iSeasonNumber != -1)
		return(true);
	return(false);
}

void cEpisode::setGroup(QButtonGroup* lpGroup)
{
	m_lpGroup	= lpGroup;
}

void cEpisode::setLabel(QLabel* lpLabel)
{
	m_lpLabel	= lpLabel;
}

void cEpisode::setButton1(QRadioButton* lpButton1)
{
	m_lpButton1	= lpButton1;
}

void cEpisode::setButton2(QRadioButton* lpButton2)
{
	m_lpButton2	= lpButton2;
}

void cEpisode::setButton3(QRadioButton* lpButton3)
{
	m_lpButton3	= lpButton3;
}

void cEpisode::updateState()
{
	if(m_lpButton1)
	{
		if(m_lpButton1->isChecked())
			m_state	= StateInit;
	}
	if(m_lpButton2)
	{
		if(m_lpButton2->isChecked())
			m_state	= StateProgress;
	}
	if(m_lpButton3)
	{
		if(m_lpButton3->isChecked())
			m_state	= StateDone;
	}
}

void cEpisode::deleteResources()
{
	DELETE(m_lpGroup);
	DELETE(m_lpLabel);
	DELETE(m_lpButton1);
	DELETE(m_lpButton2);
	DELETE(m_lpButton3);
}
