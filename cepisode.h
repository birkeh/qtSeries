#ifndef CEPISODE_H
#define CEPISODE_H


#include <QMetaType>
#include <QList>
#include <QDate>

#include <QButtonGroup>
#include <QLabel>
#include <QRadioButton>


class cEpisode
{
public:
	enum State
	{
		StateUnknown	= 0,
		StateInit		= 1,
		StateProgress	= 2,
		StateDone		= 3,
	};

	cEpisode();
	~cEpisode();

	void		setID(const qint32& iID);
	qint32		id();

	void		setDirector(const QString& szDirector);
	void		setDirector(const QStringList& szDirector);
	QStringList	director();

	void		setEpisodeName(const QString& szEpisodeName);
	QString		episodeName();

	void		setEpisodeNumber(const qint16& iEpisodeNumber);
	qint16		episodeNumber();

	void		setFirstAired(const QString& szFirstAired);
	void		setFirstAired(const QDate& firstAired);
	QDate		firstAired();

	void		setGuestStars(const QString& szGuestStars);
	void		setGuestStars(const QStringList& szGuestStars);
	QStringList	guestStars();

	void		setIMDBID(const QString& szIMDBID);
	QString		imdbID();

	void		setLanguage(const QString& szLanguage);
	QString		language();

	void		setOverview(const QString& szOverview);
	QString		overview();

	void		setProductionCode(const QString& szProductionCode);
	QString		productionCode();

	void		setRating(const qreal& dRating);
	qreal		rating();

	void		setRatingCount(const qint32& iRatingCount);
	qint32		ratingCount();

	void		setSeasonNumber(const qint16& iSeasonNumber);
	qint16		seasonNumber();

	void		setWriter(const QString& szWriter);
	void		setWriter(const QStringList& szWriter);
	QStringList	writer();

	void		setSeasonID(const qint32& iSeasonID);
	qint32		seasonID();

	void		setSeriesID(const qint32& iSeriesID);
	qint32		seriesID();

	void		setState(const State& state);
	State		state();

	void		setFileName(const QString& szFileName);
	QString		fileName();

	void		setThumbHeight(const qint16& iThumbHeight);
	qint16		thumbHeight();

	void		setThumbWidth(const qint16& iThumbWidth);
	qint16		thumbWidth();

	bool		isValid();

	void		setGroup(QButtonGroup* lpGroup);
	void		setLabel(QLabel* lpLabel);
	void		setButton1(QRadioButton* lpButton1);
	void		setButton2(QRadioButton* lpButton2);
	void		setButton3(QRadioButton* lpButton3);

	void		updateState();
	void		deleteResources();
private:
	qint32		m_iID;
	QStringList	m_szDirector;
	QString		m_szEpisodeName;
	qint16		m_iEpisodeNumber;
	QDate		m_firstAired;
	QStringList	m_szGuestStars;
	QString		m_szIMDBID;
	QString		m_szLanguage;
	QString		m_szOverview;
	QString		m_szProductionCode;
	qreal		m_dRating;
	qint32		m_iRatingCount;
	qint16		m_iSeasonNumber;
	QStringList	m_szWriter;
	qint32		m_iSeasonID;
	qint32		m_iSeriesID;
	State		m_state;
	QString		m_szFileName;
	qint16		m_iThumbHeight;
	qint16		m_iThumbWidth;
	QButtonGroup*	m_lpGroup;
	QLabel*			m_lpLabel;
	QRadioButton*	m_lpButton1;
	QRadioButton*	m_lpButton2;
	QRadioButton*	m_lpButton3;
};

Q_DECLARE_METATYPE(cEpisode*)

#endif // CEPISODE_H
