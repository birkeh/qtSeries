#ifndef CSERIE_H
#define CSERIE_H


#include "cseason.h"

#include <QMetaType>
#include <QDate>
#include <QSqlDatabase>


class cSerie
{
public:
	cSerie();

	void			setSeriesName(const QString& szSeriesName);
	QString			seriesName();

	void			setSeriesID(const qint32& iSeriesID);
	qint32			seriesID();

	void			setLanguage(const QString& szLanguage);
	QString			language();

	void			setBanner(const QString& szBanner);
	QString			banner();

	void			setOverview(const QString& szOverview);
	QString			overview();

	void			setFirstAired(const QString& szFirstAired);
	void			setFirstAired(const QDate& firstAired);
	QDate			firstAired();

	void			setNetwork(const QString& szNetwork);
	QString			network();

	void			setIMDBID(const QString& szIMDBID);
	QString			imdbID();

	void			setID(const qint32& iID);
	qint32			id();

	void			setActors(const QString& szActors);
	void			setActors(const QStringList& szActors);
	QStringList		actors();

	void			setContentRating(const QString& szContentRating);
	QString			contentRating();

	void			setGenre(const QString& szGenre);
	void			setGenre(const QStringList& szGenre);
	QStringList		genre();

	void			setRating(const qreal& dRating);
	qreal			rating();

	void			setRatingCount(const qint16& iRatingCount);
	qint16			ratingCount();

	void			setRuntime(const qint16& iRuntime);
	qint16			runime();

	void			setStatus(const QString& szStatus);
	QString			status();

	void			setDownload(const QString& szDownload);
	QString			download();

	void			setCliffhanger(const bool& bCliffhanger);
	bool			cliffhanger();

	cSeason*		addSeason(const qint16& iSeason);
	QList<cSeason*>	seasonList();

	cSeason*		findSeason(const qint16& iSeason);

	qint16			minSeason();
	qint16			maxSeason();

	qint16			maxEpisode();

	cEpisode*		findEpisode(const qint32& id);

	bool			isValid();

	bool			save(QSqlDatabase& db);
	bool			del(QSqlDatabase& db);

	void			allInit();
	void			allProgress();
	void			allDone();

	void			seasonInit(QPushButton* lpButton);
	void			seasonProgress(QPushButton* lpButton);
	void			seasonDone(QPushButton* lpButton);

	void			updateState();
	void			deleteResources();
private:
	QString			m_szSeriesName;
	qint32			m_iSeriesID;
	QString			m_szLanguage;
	QString			m_szBanner;
	QString			m_szOverview;
	QDate			m_firstAired;
	QString			m_szNetwork;
	QString			m_szIMDBID;
	qint32			m_iID;
	QStringList		m_szActors;
	QString			m_szContentRating;
	QStringList		m_szGenre;
	qreal			m_dRating;
	qint16			m_iRatingCount;
	qint16			m_iRuntime;
	QString			m_szStatus;
	QString			m_szDownload;
	bool			m_bCliffhanger;
	QList<cSeason*>	m_seasonList;
};

Q_DECLARE_METATYPE(cSerie*)

class cSerieList : public QList<cSerie*>
{
public:
	cSerie*			add(const qint32& iID);
	cSerie*			add(cSerie* lpSerie);

	qint16			minSeason();
	qint16			maxSeason();

	qint16			maxEpisode();
};

#endif // CSERIE_H
