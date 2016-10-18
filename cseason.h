#ifndef CSEASON_H
#define CSEASON_H


#include "cepisode.h"

#include <QMetaType>
#include <QList>

#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>


class cSerie;

class cSeason
{
public:
	cSeason();
	~cSeason();

	void				setNumber(const qint16& iNumber);
	qint16				number();

	void				setSerie(cSerie* lpSerie);
	cSerie*				serie();

	cEpisode*			addEpisode(qint16 iNumber);
	cEpisode*			addEpisode(cEpisode* lpEpisode);
	QList<cEpisode*>	episodeList();

	qint16				episodeCount();

	void				setGroupBox(QGroupBox* lpGroupBox);
	void				setGridLayout(QGridLayout* lpGridLayout);
	void				setGrid(QGridLayout* lpGrid);
	void				setLabel1(QLabel* lpLabel1);
	void				setAllInit(QPushButton* lpAllInit);
	void				setAllProgress(QPushButton* lpAllProgress);
	void				setAllDone(QPushButton* lpAllDone);
	void				setSpacer(QSpacerItem* lpSpacer);

	void				updateState();
	void				deleteResources();

	void				allInit();
	void				allProgress();
	void				allDone();

	QPushButton*		allInitButton();
	QPushButton*		allProgressButton();
	QPushButton*		allDoneButton();
private:
	qint16				m_iNumber;
	QList<cEpisode*>	m_episodeList;
	QGroupBox*			m_lpGroupBox;
	QGridLayout*		m_lpGridLayout;
	QGridLayout*		m_lpGrid;
	QLabel*				m_lpLabel1;
	QPushButton*		m_lpAllInit;
	QPushButton*		m_lpAllProgress;
	QPushButton*		m_lpAllDone;
	QSpacerItem*		m_lpSpacer;
	cSerie*				m_lpSerie;
};

Q_DECLARE_METATYPE(cSeason*)


#endif // CSEASON_H
