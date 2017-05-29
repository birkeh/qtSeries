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
//	cSerie*			load(const qint32 &iID, const QString& szLanguage);
private:
	QString			m_szToken;
//	cSerie*			parseSeries(const QDomElement& element);
//	void			parseEpisode(cSerie* lpSerie, const QDomElement& element);
};

#endif // CTHETVDBV2_H
