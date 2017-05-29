#ifndef CTHETVDBV2_H
#define CTHETVDBV2_H


#include "cserie.h"

#include <QList>
#include <QStringList>


class cTheTVDBV2
{
public:
    cTheTVDBV2();

	QList<cSerie*>	search(const QString& szSerie, const QString& szLanguage = "all");
	cSerie*			load(const qint32 &iID, const QString& szLanguage);
private:
	QString			m_szToken;
	QStringList		getActors(const qint32& iID);
	void			getEpisodes(cSerie* lpSerie, const QString& szLanguage);
	cEpisode*		getEpisode(const qint32& iID, const QString& szLanguage);
};

#endif // CTHETVDBV2_H
