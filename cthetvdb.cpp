#include "cthetvdb.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QUrlQuery>
#include <QBuffer>


cTheTVDB::cTheTVDB()
{

}

// BC0893B659680049
// Mirrors: http://thetvdb.com/api/BC0893B659680049/mirrors.xml
// Server time: http://thetvdb.com/api/Updates.php?type=none
// Search: http://thetvdb.com/api/GetSeries.php?seriesname=outer limits&language=all
// Episodes: http://thetvdb.com/api/BC0893B659680049/series/72224/all/de.xml
// Banner: http://thetvdb.com/banners/graphical/77092-g3.jpg // 758 x 140
// Banner: http://thetvdb.com/banners/graphical/77544-g2.jpg
// Banner: http://thetvdb.com/banners/graphical/210841-g10.jpg
QList<cSerie*> cTheTVDB::search(const QString& szSerie, const QString& szLanguage)
{
	QList<cSerie*>			serieList;
	QNetworkAccessManager	networkManager;
	cSerie*					lpNew;

	QNetworkRequest			request(QUrl(QString("http://thetvdb.com/api/GetSeries.php?seriesname=%1&language=%2").arg(szSerie).arg(szLanguage)));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/xml");

	QNetworkReply*			reply   = networkManager.get(request);
	QEventLoop				loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	QByteArray				szData  = reply->readAll();
	delete reply;

	QDomDocument			doc;
	QString					errorStr;
	int						errorLine;
	int						errorColumn;
	if(!doc.setContent(szData, false, &errorStr, &errorLine, &errorColumn))
		return(serieList);

	QDomElement				root	= doc.documentElement();
	if(root.tagName().compare("Data", Qt::CaseInsensitive))
		return(serieList);

	QDomNode				child				= root.firstChild();

	while(!child.isNull())
	{
		if(!child.toElement().tagName().compare("Series", Qt::CaseInsensitive))
		{
			lpNew	= parseSeries(child.toElement());
			if(lpNew)
				serieList.append(lpNew);
		}

		child	= child.nextSibling();
	}
	return(serieList);
}

cSerie* cTheTVDB::parseSeries(const QDomElement& element)
{
	cSerie*	lpNew	= new cSerie;

	QDomNode	child	= element.firstChild();
	while(!child.isNull())
	{
		if(!child.toElement().tagName().compare("SeriesName", Qt::CaseInsensitive))
			lpNew->setSeriesName(child.toElement().text());
		else if(!child.toElement().tagName().compare("seriesid", Qt::CaseInsensitive))
			lpNew->setSeriesID(child.toElement().text().toInt());
		else if(!child.toElement().tagName().compare("language", Qt::CaseInsensitive))
			lpNew->setLanguage(child.toElement().text());
		else if(!child.toElement().tagName().compare("banner", Qt::CaseInsensitive))
			lpNew->setBanner(child.toElement().text());
		else if(!child.toElement().tagName().compare("Overview", Qt::CaseInsensitive))
			lpNew->setOverview(child.toElement().text());
		else if(!child.toElement().tagName().compare("FirstAired", Qt::CaseInsensitive))
			lpNew->setFirstAired(child.toElement().text());
		else if(!child.toElement().tagName().compare("Network", Qt::CaseInsensitive))
			lpNew->setNetwork(child.toElement().text());
		else if(!child.toElement().tagName().compare("IMDB_ID", Qt::CaseInsensitive))
			lpNew->setIMDBID(child.toElement().text());
		else if(!child.toElement().tagName().compare("id", Qt::CaseInsensitive))
			lpNew->setID(child.toElement().text().toInt());
		else if(!child.toElement().tagName().compare("Actors", Qt::CaseInsensitive))
			lpNew->setActors(child.toElement().text());
		else if(!child.toElement().tagName().compare("ContentRating", Qt::CaseInsensitive))
			lpNew->setContentRating(child.toElement().text());
		else if(!child.toElement().tagName().compare("Genre", Qt::CaseInsensitive))
			lpNew->setGenre(child.toElement().text());
		else if(!child.toElement().tagName().compare("Rating", Qt::CaseInsensitive))
			lpNew->setRating(child.toElement().text().toDouble());
		else if(!child.toElement().tagName().compare("RatingCount", Qt::CaseInsensitive))
			lpNew->setRatingCount(child.toElement().text().toInt());
		else if(!child.toElement().tagName().compare("Runtime", Qt::CaseInsensitive))
			lpNew->setRuntime(child.toElement().text().toInt());
		else if(!child.toElement().tagName().compare("Status", Qt::CaseInsensitive))
			lpNew->setStatus(child.toElement().text());

		child	= child.nextSibling();
	}
	if(lpNew->isValid())
		return(lpNew);
	delete lpNew;
	return(0);
}

cSerie* cTheTVDB::load(const qint32 &iID, const QString &szLanguage)
{
	cSerie*					lpSerie	= 0;
	QNetworkAccessManager	networkManager;

	QNetworkRequest			request(QUrl(QString("http://thetvdb.com/api/BC0893B659680049/series/%1/all/%2.xml").arg(iID).arg(szLanguage)));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/xml");

	QNetworkReply*			reply   = networkManager.get(request);
	QEventLoop				loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	QByteArray				szData  = reply->readAll();
	delete reply;

	QDomDocument			doc;
	QString					errorStr;
	int						errorLine;
	int						errorColumn;

	if(!doc.setContent(szData, false, &errorStr, &errorLine, &errorColumn))
		return(lpSerie);

	QDomElement				root	= doc.documentElement();
	if(root.tagName().compare("Data", Qt::CaseInsensitive))
		return(lpSerie);

	QDomNode				child				= root.firstChild();

	while(!child.isNull())
	{
		if(!child.toElement().tagName().compare("Series", Qt::CaseInsensitive))
		{
			lpSerie	= parseSeries(child.toElement());
			if(!lpSerie)
				return(lpSerie);
		}
		else if(!child.toElement().tagName().compare("Episode", Qt::CaseInsensitive))
			parseEpisode(lpSerie, child.toElement());

		child	= child.nextSibling();
	}

	return(lpSerie);
}

void cTheTVDB::parseEpisode(cSerie* lpSerie, const QDomElement& element)
{
	cSeason*	lpSeason	= 0;
	cEpisode*	lpEpisode	= new cEpisode;

	QDomNode	child	= element.firstChild();
	while(!child.isNull())
	{
		if(!child.toElement().tagName().compare("id", Qt::CaseInsensitive))
			lpEpisode->setID(child.toElement().text().toInt());
		else if(!child.toElement().tagName().compare("Director", Qt::CaseInsensitive))
			lpEpisode->setDirector(child.toElement().text());
		else if(!child.toElement().tagName().compare("EpisodeName", Qt::CaseInsensitive))
			lpEpisode->setEpisodeName(child.toElement().text());
		else if(!child.toElement().tagName().compare("EpisodeNumber", Qt::CaseInsensitive))
			lpEpisode->setEpisodeNumber(child.toElement().text().toInt());
		else if(!child.toElement().tagName().compare("FirstAired", Qt::CaseInsensitive))
			lpEpisode->setFirstAired(child.toElement().text());
		else if(!child.toElement().tagName().compare("GuestStars", Qt::CaseInsensitive))
			lpEpisode->setGuestStars(child.toElement().text());
		else if(!child.toElement().tagName().compare("IMDB_ID", Qt::CaseInsensitive))
			lpEpisode->setIMDBID(child.toElement().text());
		else if(!child.toElement().tagName().compare("Language", Qt::CaseInsensitive))
			lpEpisode->setLanguage(child.toElement().text());
		else if(!child.toElement().tagName().compare("Overview", Qt::CaseInsensitive))
			lpEpisode->setOverview(child.toElement().text());
		else if(!child.toElement().tagName().compare("ProductionCode", Qt::CaseInsensitive))
			lpEpisode->setProductionCode(child.toElement().text());
		else if(!child.toElement().tagName().compare("Rating", Qt::CaseInsensitive))
			lpEpisode->setRating(child.toElement().text().toDouble());
		else if(!child.toElement().tagName().compare("RatingCount", Qt::CaseInsensitive))
			lpEpisode->setRatingCount(child.toElement().text().toInt());
		else if(!child.toElement().tagName().compare("SeasonNumber", Qt::CaseInsensitive))
			lpEpisode->setSeasonNumber(child.toElement().text().toInt());
		else if(!child.toElement().tagName().compare("Writer", Qt::CaseInsensitive))
			lpEpisode->setWriter(child.toElement().text());
		else if(!child.toElement().tagName().compare("seasonid", Qt::CaseInsensitive))
			lpEpisode->setSeasonID(child.toElement().text().toInt());
		else if(!child.toElement().tagName().compare("seriesid", Qt::CaseInsensitive))
			lpEpisode->setSeriesID(child.toElement().text().toInt());
		else if(!child.toElement().tagName().compare("filename", Qt::CaseInsensitive))
			lpEpisode->setFileName(child.toElement().text());
		else if(!child.toElement().tagName().compare("thumb_height", Qt::CaseInsensitive))
			lpEpisode->setThumbHeight(child.toElement().text().toInt());
		else if(!child.toElement().tagName().compare("thumb_width", Qt::CaseInsensitive))
			lpEpisode->setThumbWidth(child.toElement().text().toInt());

		child	= child.nextSibling();
	}
	if(!lpEpisode->isValid())
	{
		delete lpEpisode;
		return;
	}

	lpSeason	= lpSerie->findSeason(lpEpisode->seasonNumber());
	if(!lpSeason)
		lpSeason	= lpSerie->addSeason(lpEpisode->seasonNumber());
	if(!lpSeason)
	{
		delete lpEpisode;
		return;
	}

	lpSeason->addEpisode(lpEpisode);
}
