#include "cthetvdbv2.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


cTheTVDBV2::cTheTVDBV2()
{
	QByteArray				jsonRequest		= "{\"apikey\":\"BC0893B659680049\"}";
	QByteArray				postDataSize	= QByteArray::number(jsonRequest.size());
	QNetworkAccessManager	networkManager;

	QNetworkRequest			request(QUrl(QString("https://api.thetvdb.com/login")));

	request.setRawHeader("Content-Type", "application/json");
	request.setRawHeader("Content-Length", postDataSize);

	QNetworkReply*			reply   = networkManager.post(request, jsonRequest);
	QEventLoop				loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	if (reply->error() == QNetworkReply::NoError)
	{
		QString			strReply		= (QString)reply->readAll();
		QJsonDocument	jsonResponse	= QJsonDocument::fromJson(strReply.toUtf8());
		QJsonObject		jsonObj			= jsonResponse.object();
		m_szToken		= jsonObj["token"].toString();

		delete reply;
	}
	else
	{
		qDebug() << reply->errorString();
		delete reply;
	}
}

QList<cSerie*> cTheTVDBV2::search(const QString& szSerie, const QString& szLanguage)
{
	QList<cSerie*>			serieList;
	QNetworkAccessManager	networkManager;
	QNetworkRequest			request(QUrl(QString("https://api.thetvdb.com/search/series?name=%1").arg(szSerie)));

	request.setRawHeader("Content-Type", "application/json");
	request.setRawHeader("Authorization", QString("Bearer %1").arg(m_szToken).toUtf8());
	if(!szLanguage.contains("all"))
		request.setRawHeader("Accept-Language", szLanguage.toUtf8());
	else
		request.setRawHeader("Accept-Language", "en");

	QNetworkReply*			reply   = networkManager.get(request);
	QEventLoop				loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	if (reply->error() == QNetworkReply::NoError)
	{
		QString			strReply		= (QString)reply->readAll();
		QJsonDocument	jsonResponse	= QJsonDocument::fromJson(strReply.toUtf8());
		QJsonObject		jsonObj			= jsonResponse.object();
		QJsonArray		jsonArray		= jsonObj["data"].toArray();

		for(int z = 0;z < jsonArray.count();z++)
		{
			QJsonObject	serie			= jsonArray[z].toObject();
			cSerie*		lpSerie			= new cSerie;
			lpSerie->setID(serie["id"].toInt());
			lpSerie->setSeriesName(serie["seriesName"].toString());
			lpSerie->setFirstAired(serie["firstAired"].toString());
			serieList.append(lpSerie);
		}
		delete reply;
	}
	else
	{
		qDebug() << reply->errorString();
		delete reply;
	}
	return(serieList);
}

cSerie* cTheTVDBV2::load(const qint32 &iID, const QString &szLanguage)
{
	cSerie*					lpSerie	= 0;
	QNetworkAccessManager	networkManager;
	QNetworkRequest			request(QUrl(QString("https://api.thetvdb.com/series/%1").arg(iID)));

	request.setRawHeader("Content-Type", "application/json");
	request.setRawHeader("Authorization", QString("Bearer %1").arg(m_szToken).toUtf8());
	if(!szLanguage.contains("all"))
		request.setRawHeader("Accept-Language", szLanguage.toUtf8());
	else
		request.setRawHeader("Accept-Language", "en");

	QNetworkReply*			reply   = networkManager.get(request);
	QEventLoop				loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	if (reply->error() == QNetworkReply::NoError)
	{
		QString			strReply		= (QString)reply->readAll();
		QJsonDocument	jsonResponse	= QJsonDocument::fromJson(strReply.toUtf8());
		QJsonObject		jsonObj			= jsonResponse.object();
		QJsonObject		serieDetails	= jsonObj["data"].toObject();
		QJsonArray		genreArray		= serieDetails["genre"].toArray();
		QStringList		genreList;

		delete reply;

		lpSerie	= new cSerie;
		lpSerie->setSeriesName(serieDetails["seriesName"].toString());
		lpSerie->setSeriesID(serieDetails["seriesId"].toInt());
		lpSerie->setLanguage(szLanguage);
		lpSerie->setBanner(serieDetails["banner"].toString());
		lpSerie->setOverview(serieDetails["overview"].toString());
		lpSerie->setFirstAired(serieDetails["firstAired"].toString());
		lpSerie->setNetwork(serieDetails["network"].toString());
		lpSerie->setIMDBID(serieDetails["imdbId"].toString());
		lpSerie->setID(serieDetails["id"].toInt());
		lpSerie->setContentRating(serieDetails["rating"].toString());
		for(int z = 0;z < genreArray.count();z++)
			genreList.append(genreArray[z].toString());
		lpSerie->setGenre(genreList);
		lpSerie->setRating(serieDetails["siteRating"].toDouble());
		lpSerie->setRatingCount(serieDetails["siteRatingCount"].toInt());
		lpSerie->setRuntime(serieDetails["runtime"].toString().toInt());
		lpSerie->setStatus(serieDetails["status"].toString());

		lpSerie->setActors(getActors(iID));

		getEpisodes(lpSerie, szLanguage);
	}
	else
	{
		qDebug() << reply->errorString();
		delete reply;
	}
	return(lpSerie);
}

QStringList cTheTVDBV2::getActors(const qint32& iID)
{
	QStringList				actorsList;
	QNetworkAccessManager	networkManager;
	QNetworkRequest			request(QUrl(QString("https://api.thetvdb.com/series/%1/actors").arg(iID)));

	request.setRawHeader("Content-Type", "application/json");
	request.setRawHeader("Authorization", QString("Bearer %1").arg(m_szToken).toUtf8());

	QNetworkReply*			reply   = networkManager.get(request);
	QEventLoop				loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	if (reply->error() == QNetworkReply::NoError)
	{
		QString			strReply		= (QString)reply->readAll();
		QJsonDocument	jsonResponse	= QJsonDocument::fromJson(strReply.toUtf8());
		QJsonObject		jsonObj			= jsonResponse.object();
		QJsonArray		actorsArray		= jsonObj["data"].toArray();

		for(int z = 0;z < actorsArray.count();z++)
		{
			QJsonObject	actor			= actorsArray.at(z).toObject();
			actorsList.append(actor["name"].toString());
		}
		delete reply;

	}
	else
	{
		qDebug() << reply->errorString();
		delete reply;
	}
	return(actorsList);
}

void cTheTVDBV2::getEpisodes(cSerie* lpSerie, const QString& szLanguage)
{
	QNetworkAccessManager	networkManager;
	QNetworkRequest			request(QUrl(QString("https://api.thetvdb.com/series/%1/episodes").arg(lpSerie->id())));

	request.setRawHeader("Content-Type", "application/json");
	request.setRawHeader("Authorization", QString("Bearer %1").arg(m_szToken).toUtf8());
	if(!szLanguage.contains("all"))
		request.setRawHeader("Accept-Language", szLanguage.toUtf8());
	else
		request.setRawHeader("Accept-Language", "en");

	QNetworkReply*			reply   = networkManager.get(request);
	QEventLoop				loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	if (reply->error() == QNetworkReply::NoError)
	{
		QString			strReply		= (QString)reply->readAll();
		QJsonDocument	jsonResponse	= QJsonDocument::fromJson(strReply.toUtf8());
		QJsonObject		jsonObj			= jsonResponse.object();
		QJsonArray		episodesArray	= jsonObj["data"].toArray();

		for(int z = 0;z < episodesArray.count();z++)
		{
			QJsonObject	episode			= episodesArray.at(z).toObject();
			cEpisode*	lpEpisode		= getEpisode(episode["id"].toInt(), szLanguage);
			cSeason*	lpSeason		= 0;

			if(lpEpisode)
			{
				lpSeason	= lpSerie->findSeason(lpEpisode->seasonNumber());
				if(!lpSeason)
					lpSeason	= lpSerie->addSeason(lpEpisode->seasonNumber());
				if(!lpSeason)
					delete lpEpisode;
				else
					lpSeason->addEpisode(lpEpisode);
			}
		}
		delete reply;

	}
	else
	{
		qDebug() << reply->errorString();
		delete reply;
	}
}

cEpisode* cTheTVDBV2::getEpisode(const qint32& iID, const QString& szLanguage)
{
	cEpisode*	lpEpisode	= 0;

	QNetworkAccessManager	networkManager;
	QNetworkRequest			request(QUrl(QString("https://api.thetvdb.com/episodes/%1").arg(iID)));

	request.setRawHeader("Content-Type", "application/json");
	request.setRawHeader("Authorization", QString("Bearer %1").arg(m_szToken).toUtf8());
	if(!szLanguage.contains("all"))
		request.setRawHeader("Accept-Language", szLanguage.toUtf8());
	else
		request.setRawHeader("Accept-Language", "en");

	QNetworkReply*			reply   = networkManager.get(request);
	QEventLoop				loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	if (reply->error() == QNetworkReply::NoError)
	{
		lpEpisode		= new cEpisode;

		QString			strReply		= (QString)reply->readAll();
		QJsonDocument	jsonResponse	= QJsonDocument::fromJson(strReply.toUtf8());
		QJsonObject		jsonObj			= jsonResponse.object();
		QJsonObject		episodeObj		= jsonObj["data"].toObject();

		QJsonArray		writerArray		= episodeObj["writers"].toArray();
		QStringList		writerList;
		QJsonArray		guestArray		= episodeObj["guestStars"].toArray();
		QStringList		guestList;

		lpEpisode->setID(episodeObj["id"].toInt());
		lpEpisode->setDirector(episodeObj["director"].toString());
		lpEpisode->setEpisodeName(episodeObj["episodeName"].toString());
		lpEpisode->setEpisodeNumber(episodeObj["airedEpisodeNumber"].toInt());
		lpEpisode->setFirstAired(episodeObj["firstAired"].toString());
		for(int z = 0;z < guestArray.count();z++)
			guestList.append(guestArray[z].toString());
		lpEpisode->setGuestStars(guestList);
		lpEpisode->setIMDBID(episodeObj["imdbId"].toString()); // IMDBID
		lpEpisode->setLanguage(szLanguage);
		lpEpisode->setOverview(episodeObj["overview"].toString());
		lpEpisode->setProductionCode(episodeObj["productionCode"].toString()); // PRORUCTION CODE
		lpEpisode->setRating(episodeObj["siteRating"].toDouble()); // RATING
		lpEpisode->setRatingCount(episodeObj["siteRatingCount"].toInt()); // RATING COUNT
		lpEpisode->setSeasonNumber(episodeObj["airedSeason"].toInt());
		for(int z = 0;z < writerArray.count();z++)
			writerList.append(writerArray[z].toString());
		lpEpisode->setWriter(writerList);
		lpEpisode->setSeasonID(episodeObj["airedSeasonID"].toInt()); // SEASON ID
		lpEpisode->setSeriesID(episodeObj["seriesId"].toInt()); // SERIES ID
		lpEpisode->setFileName(episodeObj["filename"].toString()); // FILENAME
		lpEpisode->setThumbHeight(episodeObj["thumbHeight"].toString().toInt());
		lpEpisode->setThumbWidth(episodeObj["thumbWidth"].toString().toInt());

		if(!lpEpisode->isValid())
		{
			delete lpEpisode;
			return(0);
		}
		return(lpEpisode);
	}
	else
	{
		qDebug() << reply->errorString();
		delete reply;
		return(0);
	}
}
