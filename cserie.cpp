#include "cserie.h"
#include <QStringList>
#include <QSqlQuery>
#include <QVariant>


void addPerson(const QString& szTable, const QString& szField1, const qint32 &iValue1, const QString &szField2, const QString& szName)
{
	qint32		personID;
	QSqlQuery	query;

	query.exec(QString("SELECT personID FROM person WHERE name='%1';").arg(szName));
	if(query.next())
		personID	= query.value(0).toInt();
	else
	{
		query.prepare("INSERT INTO person (name) VALUES (:name);");
		query.bindValue(":name", szName);
		query.exec();

		query.clear();
		query.prepare("SELECT personID FROM person WHERE name=:name;");
		query.bindValue(":name", szName);
		query.exec();
		query.next();
		personID	= query.value(0).toInt();
	}

	query.exec(QString("SELECT * FROM %1 WHERE %2=%3 AND %4=%5;").arg(szTable).arg(szField1).arg(iValue1).arg(szField2).arg(personID));
	if(!query.next())
	{
		query.prepare(QString("INSERT INTO %1 (%2, %3) VALUES (:%4, :%5);").arg(szTable).arg(szField1).arg(szField2).arg(szField1).arg(szField2));
		query.bindValue(QString(":%1").arg(szField1), iValue1);
		query.bindValue(QString(":%1").arg(szField2), personID);
		query.exec();
	}
}

cSerie::cSerie() :
	m_szSeriesName(""),
	m_iSeriesID(-1),
	m_szLanguage(""),
	m_szBanner(""),
	m_szOverview(""),
	m_firstAired(QDate(1900, 1, 1)),
	m_szNetwork(""),
	m_szIMDBID(""),
	m_iID(-1),
	m_szActors(),
	m_szContentRating(""),
	m_szGenre(),
	m_dRating(-1.0),
	m_iRatingCount(-1),
	m_iRuntime(-1),
	m_szStatus(""),
	m_szDownload(""),
	m_bCliffhanger(false)
{
}

void cSerie::setSeriesName(const QString& szSeriesName)
{
	m_szSeriesName	= szSeriesName;
}

QString cSerie::seriesName()
{
	return(m_szSeriesName);
}

void cSerie::setSeriesID(const qint32& iSeriesID)
{
	m_iSeriesID		= iSeriesID;
}

qint32 cSerie::seriesID()
{
	return(m_iSeriesID);
}

void cSerie::setLanguage(const QString& szLanguage)
{
	m_szLanguage	= szLanguage;
}

QString cSerie::language()
{
	return(m_szLanguage);
}

void cSerie::setBanner(const QString& szBanner)
{
	m_szBanner		= szBanner;
}

QString cSerie::banner()
{
	return(m_szBanner);
}

void cSerie::setOverview(const QString& szOverview)
{
	m_szOverview	= szOverview;
}

QString cSerie::overview()
{
	return(m_szOverview);
}

void cSerie::setFirstAired(const QString& szFirstAired)
{
	m_firstAired	= QDate::fromString(szFirstAired, "yyyy-MM-dd");
}

void cSerie::setFirstAired(const QDate& firstAired)
{
	m_firstAired	= firstAired;
}

QDate cSerie::firstAired()
{
	return(m_firstAired);
}

void cSerie::setNetwork(const QString& szNetwork)
{
	m_szNetwork		= szNetwork;
}

QString cSerie::network()
{
	return(m_szNetwork);
}

void cSerie::setIMDBID(const QString& szIMDBID)
{
	m_szIMDBID		= szIMDBID;
}

QString cSerie::imdbID()
{
	return(m_szIMDBID);
}

void cSerie::setID(const qint32& iID)
{
	m_iID			= iID;
}

qint32 cSerie::id()
{
	return(m_iID);
}

void cSerie::setActors(const QString& szActors)
{
	m_szActors	= szActors.split("|");
	m_szActors.removeAll("");
}

void cSerie::setActors(const QStringList& szActors)
{
	m_szActors	= szActors;
	m_szActors.removeAll("");
}

QStringList cSerie::actors()
{
	return(m_szActors);
}

void cSerie::setContentRating(const QString& szContentRating)
{
	m_szContentRating	= szContentRating;
}

QString cSerie::contentRating()
{
	return(m_szContentRating);
}

void cSerie::setGenre(const QString& szGenre)
{
	m_szGenre	= szGenre.split("|");
	m_szGenre.removeAll("");
}

void cSerie::setGenre(const QStringList& szGenre)
{
	m_szGenre	= szGenre;
	m_szGenre.removeAll("");
}

QStringList cSerie::genre()
{
	return(m_szGenre);
}

void cSerie::setRating(const qreal& dRating)
{
	m_dRating	= dRating;
}

qreal cSerie::rating()
{
	return(m_dRating);
}

void cSerie::setRatingCount(const qint16& iRatingCount)
{
	m_iRatingCount	= iRatingCount;
}

qint16 cSerie::ratingCount()
{
	return(m_iRatingCount);
}

void cSerie::setRuntime(const qint16& iRuntime)
{
	m_iRuntime	= iRuntime;
}

qint16 cSerie::runime()
{
	return(m_iRuntime);
}

void cSerie::setStatus(const QString& szStatus)
{
	m_szStatus	= szStatus;
}

QString cSerie::status()
{
	return(m_szStatus);
}

void cSerie::setDownload(const QString& szDownload)
{
	m_szDownload	= szDownload;
}

QString cSerie::download()
{
	return(m_szDownload);
}

void cSerie::setCliffhanger(const bool& bCliffhanger)
{
	m_bCliffhanger	= bCliffhanger;
}

bool cSerie::cliffhanger()
{
	return(m_bCliffhanger);
}

cSeason* cSerie::addSeason(const qint16& iSeason)
{
	cSeason*	lpNew	= new cSeason;
	lpNew->setNumber(iSeason);
	lpNew->setSerie(this);
	m_seasonList.append(lpNew);
	return(lpNew);
}

cSeason* cSerie::findSeason(const qint16& iSeason)
{
	for(int z = 0;z < m_seasonList.count();z++)
	{
		if(m_seasonList.at(z)->number() == iSeason)
			return(m_seasonList.at(z));
	}
	return(0);
}

QList<cSeason*> cSerie::seasonList()
{
	return(m_seasonList);
}

qint16 cSerie::minSeason()
{
	qint16	iMin	= 9999;
	for(int z = 0;z < m_seasonList.count();z++)
	{
		if(m_seasonList.at(z)->number() < iMin)
			iMin	= m_seasonList.at(z)->number();
	}
	if(iMin == 9999)
		return(-1);
	return(iMin);
}

qint16 cSerie::maxSeason()
{
	qint16	iMax	= -1;
	for(int z = 0;z < m_seasonList.count();z++)
	{
		if(m_seasonList.at(z)->number() > iMax)
			iMax	= m_seasonList.at(z)->number();
	}
	return(iMax);
}

cEpisode* cSerie::findEpisode(const qint32& id)
{
	for(int x = 0;x < m_seasonList.count();x++)
	{
		cSeason*	lpSeason	= m_seasonList.at(x);
		for(int y = 0;y < lpSeason->episodeList().count();y++)
		{
			cEpisode*	lpEpisode	= lpSeason->episodeList().at(y);
			if(lpEpisode->id() == id)
				return(lpEpisode);
		}
	}
	return(0);
}

bool cSerie::isValid()
{
	if(m_iSeriesID != -1 &&
		m_szSeriesName.length())
		return(true);
	return(false);
}

qint16 cSerie::maxEpisode()
{
	qint16	iMax	= -1;
	for(int z = 0;z < m_seasonList.count();z++)
	{
		if(m_seasonList.at(z)->episodeCount() > iMax)
			iMax	= m_seasonList.at(z)->episodeCount();
	}
	return(iMax);
}

bool cSerie::save(QSqlDatabase &db)
{
	QSqlQuery	query;
	QSqlQuery	querySerie;
	QSqlQuery	queryEpisode;

	querySerie.prepare("INSERT INTO serie (seriesID,seriesName,language,banner,overview,firstAired,network,imdbid,id,contentRating,rating,ratingCount,runtime,status,download,cliffhanger,actor,genre)"
						  " VALUES (:seriesID,:seriesName,:language,:banner,:overview,:firstAired,:network,:imdbid,:id,:contentRating,:rating,:ratingCount,:runtime,:status,:download,:cliffhanger,:actor,:genre);");
	queryEpisode.prepare("INSERT INTO episode (episodeID,episodeName,episodeNumber,firstAired,imdbid,language,overview,productioncode,rating,ratingCount,seasonNumber,seasonID,seriesID,state,filename,thumb_height,thumb_width,director,gueststars,episode_writer) VALUES (:episodeID,:episodeName,:episodeNumber,:firstAired,:imdbid,:language,:overview,:productioncode,:rating,:ratingCount,:seasonNumber,:seasonID,:seriesID,:state,:filename,:thumb_height,:thumb_width,:director,:gueststars,:episode_writer);");

	db.transaction();
	query.exec(QString("SELECT id FROM serie WHERE id=%1;").arg(id()));
	if(!query.next())
	{
		query.prepare(QString("SELECT episodeID FROM episode WHERE episodeID=:episodeID"));

		querySerie.bindValue(":seriesID", seriesID());
		querySerie.bindValue(":seriesName", seriesName());
		querySerie.bindValue(":language", language());
		querySerie.bindValue(":banner", banner());
		querySerie.bindValue(":overview", overview());
		querySerie.bindValue(":firstAired", firstAired());
		querySerie.bindValue(":network", network());
		querySerie.bindValue(":imdbid", imdbID());
		querySerie.bindValue(":id", id());
		querySerie.bindValue(":contentRating", contentRating());
		querySerie.bindValue(":rating", rating());
		querySerie.bindValue(":ratingCount", ratingCount());
		querySerie.bindValue(":runtime", runime());
		querySerie.bindValue(":status", status());
		querySerie.bindValue(":download", download());
		querySerie.bindValue(":cliffhanger", cliffhanger());
		querySerie.bindValue(":actor", actors().join(","));
		querySerie.bindValue(":genre", genre().join(","));
		if(querySerie.exec())
		{
			QList<cSeason*>	seasonList	= this->seasonList();
			for(int season = 0;season < seasonList.count();season++)
			{
				QList<cEpisode*>	episodeList	= seasonList.at(season)->episodeList();
				for(int episode = 0;episode < episodeList.count();episode++)
				{
					cEpisode*	lpEpisode	= episodeList.at(episode);

					//query.bindValue(":episodeID", lpEpisode->id());
					//query.exec();
					//if(!query.next())
					{
						queryEpisode.bindValue(":episodeID", lpEpisode->id());
						queryEpisode.bindValue(":episodeName", lpEpisode->episodeName());
						queryEpisode.bindValue(":episodeNumber", lpEpisode->episodeNumber());
						queryEpisode.bindValue(":firstAired", lpEpisode->firstAired());
						queryEpisode.bindValue(":imdbid", lpEpisode->imdbID());
						queryEpisode.bindValue(":language", lpEpisode->language());
						queryEpisode.bindValue(":overview", lpEpisode->overview());
						queryEpisode.bindValue(":productioncode", lpEpisode->productionCode());
						queryEpisode.bindValue(":rating", lpEpisode->rating());
						queryEpisode.bindValue(":ratingCount", lpEpisode->ratingCount());
						queryEpisode.bindValue(":seasonNumber", lpEpisode->seasonNumber());
						queryEpisode.bindValue(":seasonID", lpEpisode->seasonID());
						queryEpisode.bindValue(":seriesID", lpEpisode->seriesID());
						queryEpisode.bindValue(":state", lpEpisode->state());
						queryEpisode.bindValue(":filename", lpEpisode->fileName());
						queryEpisode.bindValue(":thumb_height", lpEpisode->thumbHeight());
						queryEpisode.bindValue(":thumb_width", lpEpisode->thumbWidth());
						queryEpisode.bindValue(":director", lpEpisode->director().join(","));
						queryEpisode.bindValue(":gueststars", lpEpisode->guestStars().join(","));
						queryEpisode.bindValue(":episode_writer", lpEpisode->writer().join(","));
						if(queryEpisode.exec())
						{
						}
					}
				}
			}
		}
	}
	db.commit();

	return(true);
}

bool cSerie::del(QSqlDatabase& db)
{
	QSqlQuery			query;

	db.transaction();
	query.prepare("DELETE FROM episode WHERE seriesID=:seriesID;");
	query.bindValue(":seriesID", id());
	query.exec();

	query.prepare("DELETE FROM serie WHERE id=:seriesID;");
	query.bindValue(":seriesID", id());
	query.exec();

	db.commit();
	return(true);
}

void cSerie::allInit()
{
	for(int z = 0;z < m_seasonList.count();z++)
		m_seasonList.at(z)->allInit();
}

void cSerie::allProgress()
{
	for(int z = 0;z < m_seasonList.count();z++)
		m_seasonList.at(z)->allProgress();
}

void cSerie::allDone()
{
	for(int z = 0;z < m_seasonList.count();z++)
		m_seasonList.at(z)->allDone();
}

void cSerie::seasonInit(QPushButton* lpButton)
{
	for(int z = 0;z < m_seasonList.count();z++)
	{
		if(m_seasonList.at(z)->allInitButton() == lpButton)
		{
			m_seasonList.at(z)->allInit();
			return;
		}
	}
}

void cSerie::seasonProgress(QPushButton* lpButton)
{
	for(int z = 0;z < m_seasonList.count();z++)
	{
		if(m_seasonList.at(z)->allProgressButton() == lpButton)
		{
			m_seasonList.at(z)->allProgress();
			return;
		}
	}
}

void cSerie::seasonDone(QPushButton* lpButton)
{
	for(int z = 0;z < m_seasonList.count();z++)
	{
		if(m_seasonList.at(z)->allDoneButton() == lpButton)
		{
			m_seasonList.at(z)->allDone();
			return;
		}
	}
}

void cSerie::updateState()
{
	for(int z = 0;z < m_seasonList.count();z++)
		m_seasonList.at(z)->updateState();
}

void cSerie::deleteResources()
{
	for(int z = 0;z < m_seasonList.count();z++)
		m_seasonList.at(z)->deleteResources();
}

cSerie* cSerieList::add(const qint32& iID)
{
	for(int z = 0;z < count();z++)
	{
		if(at(z)->id() == iID)
			return(at(z));
	}
	cSerie*	lpNew	= new cSerie;
	lpNew->setID(iID);
	append(lpNew);
	return(lpNew);
}

cSerie* cSerieList::add(cSerie* lpSerie)
{
	for(int z = 0;z < count();z++)
	{
		if(at(z) == lpSerie)
			return(0);
	}
	append(lpSerie);
	return(lpSerie);
}

qint16 cSerieList::minSeason()
{
	qint16	iMin	= 9999;
	for(int z = 0;z < count();z++)
	{
		if(at(z)->minSeason() < iMin)
			iMin	= at(z)->minSeason();
	}
	if(iMin == 9999 || iMin < 0)
		return(0);

	return(iMin);
}

qint16 cSerieList::maxSeason()
{
	qint16	iMax	= -1;
	for(int z = 0;z < count();z++)
	{
		if(at(z)->maxSeason() > iMax)
			iMax	= at(z)->maxSeason();
	}

	if(iMax < 0)
		return(0);

	return(iMax);
}

qint16 cSerieList::maxEpisode()
{
	qint16	iMax	= -1;
	for(int z = 0;z < count();z++)
	{
		if(at(z)->maxEpisode() > iMax)
			iMax	= at(z)->maxEpisode();
	}

	if(iMax < 0)
		return(0);

	return(iMax);
}
