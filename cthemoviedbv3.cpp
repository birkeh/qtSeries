#include "cthemoviedbv3.h"


#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QEventLoop>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


cTheMovieDBV3::cTheMovieDBV3()
{
	m_szToken	= "a33271b9e54cdcb9a80680eaf5522f1b";
/*
	QNetworkAccessManager	networkManager;

	QNetworkRequest			request(QUrl(QString("https://api.themoviedb.org/3/authentication/token/new?api_key=a33271b9e54cdcb9a80680eaf5522f1b")));

	QNetworkReply*			reply   = networkManager.get(request);
	QEventLoop				loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	if (reply->error() == QNetworkReply::NoError)
	{
		QString			strReply		= (QString)reply->readAll();
		QJsonDocument	jsonResponse	= QJsonDocument::fromJson(strReply.toUtf8());
		QJsonObject		jsonObj			= jsonResponse.object();
		bool			success			= jsonObj["success"].toBool();
		m_szToken		= jsonObj["request_token"].toString();

		delete reply;
	}
	else
	{
		qDebug() << reply->errorString();
		delete reply;
	}
*/
}

QList<cMovie*> cTheMovieDBV3::search(const QString& szMovie, const qint16& year, const QString& szLanguage)
{
	QList<cMovie*>			movieList;
	QNetworkAccessManager	networkManager;
	QString					szRequest	= QString("https://api.themoviedb.org/3/search/movie?api_key=%1").arg(m_szToken);
	qint16					page		= 1;

	if(!szLanguage.contains("all"))
		szRequest.append(QString("&language=%1").arg(szLanguage));

	szRequest.append(QString("&query=%1").arg(szMovie));

	if(year != -1)
		szRequest.append(QString("&year=%1").arg(year));

	szRequest.append("&include_adult=false");

	for(;;)
	{
		QNetworkRequest			request(QUrl(QString("%1&page=%2").arg(szRequest).arg(page)));

		QNetworkReply*			reply   = networkManager.get(request);
		QEventLoop				loop;

		QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
		loop.exec();

		if (reply->error() == QNetworkReply::NoError)
		{
			QString			strReply		= (QString)reply->readAll();
			QJsonDocument	jsonResponse	= QJsonDocument::fromJson(strReply.toUtf8());
			QJsonObject		jsonObj			= jsonResponse.object();
			QJsonArray		jsonArray		= jsonObj["results"].toArray();

			for(int z = 0;z < jsonArray.count();z++)
			{
				QJsonObject	movie			= jsonArray[z].toObject();
				cMovie*		lpMovie			= new cMovie;
//				lpSerie->setID(serie["id"].toInt());
//				lpSerie->setSeriesName(serie["seriesName"].toString());
//				lpSerie->setFirstAired(serie["firstAired"].toString());
				lpMovie->setMovieTitle(movie["title"].toString());
				lpMovie->setMovieID(movie["id"].toInt());
				lpMovie->setOriginalTitle(movie["original_title"].toString());
				lpMovie->setReleaseDate(movie["release_date"].toString());
				movieList.append(lpMovie);
			}
			if(jsonObj["total_pages"].toInt() == page)
				break;

			page++;
			delete reply;
		}
		else
		{
			qDebug() << reply->errorString();
			delete reply;
		}
	}
	return(movieList);
}
