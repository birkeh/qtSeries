#include "cmovie.h"


cMovie::cMovie()
{

}

void cMovie::setMovieTitle(const QString& szTitle)
{
	m_szMovieTitle	= szTitle;
}

QString cMovie::movieTitle()
{
	return(m_szMovieTitle);
}

void cMovie::setMovieID(const qint32 iID)
{
	m_iID	= iID;
}

qint32 cMovie::movieID()
{
	return(m_iID);
}

void cMovie::setOriginalTitle(const QString& szOriginalTitle)
{
	m_szOriginalTitle	= szOriginalTitle;
}

QString cMovie::originalTitle()
{
	return(m_szOriginalTitle);
}

void cMovie::setReleaseDate(const QString& szDate)
{
	m_releaseDate	= QDate::fromString(szDate, "yyyy-MM-dd");
}

QDate cMovie::releaseDate()
{
	return(m_releaseDate);
}
