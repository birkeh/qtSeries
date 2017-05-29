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

/*
cSerie* cTheTVDBV2::parseSeries(const QDomElement& element)
{
}
*/
/*
cSerie* cTheTVDBV2::load(const qint32 &iID, const QString &szLanguage)
{
}
*/
