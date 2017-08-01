#ifndef CMOVIE_H
#define CMOVIE_H


#include <QMetaType>
#include <QDate>
#include <QSqlDatabase>


class cMovie
{
public:
	cMovie();

	void			setMovieTitle(const QString& szTitle);
	QString			movieTitle();

	void			setMovieID(const qint32 iID);
	qint32			movieID();

	void			setOriginalTitle(const QString& szOriginalTitle);
	QString			originalTitle();

	void			setReleaseDate(const QString& szDate);
	QDate			releaseDate();
private:
	QString			m_szMovieTitle;
	qint32			m_iID;
	QString			m_szOriginalTitle;
	QDate			m_releaseDate;
};

Q_DECLARE_METATYPE(cMovie*)

class cMovieList : public QList<cMovie*>
{
public:
//	cMovie*			add(const qint32& iID);
//	cMovie*			add(cMovie* lpMovie);
};

#endif // CMOVIE_H
