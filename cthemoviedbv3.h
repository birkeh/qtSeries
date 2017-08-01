#ifndef CTHEMOVIEDBV3_H
#define CTHEMOVIEDBV3_H


#include "cmovie.h"

#include <QString>
#include <QList>
#include <QStringList>


class cTheMovieDBV3
{
public:
	cTheMovieDBV3();

	QList<cMovie*>	search(const QString& szMovie, const qint16& year = -1, const QString& szLanguage = "all");
//	cSerie*			load(const qint32 &iID, const QString& szLanguage);
private:
	QString			m_szToken;
//	QStringList		getActors(const qint32& iID);
//	void			getEpisodes(cSerie* lpSerie, const QString& szLanguage);
//	cEpisode*		getEpisode(const qint32& iID, const QString& szLanguage);
};

#endif // CTHEMOVIEDBV3_H
