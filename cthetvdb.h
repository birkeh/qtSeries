#ifndef CTHETVDB_H
#define CTHETVDB_H


#include "cserie.h"

#include <QList>
#include <QStringList>
#include <QDomDocument>


class cTheTVDB
{
public:
	cTheTVDB();

	QList<cSerie*>	search(const QString& szSerie, const QString& szLanguage = "all");
	cSerie*			load(const qint32 &iID, const QString& szLanguage);
private:
	cSerie*			parseSeries(const QDomElement& element);
	void			parseEpisode(cSerie* lpSerie, const QDomElement& element);
};

#endif // CTHETVDB_H
