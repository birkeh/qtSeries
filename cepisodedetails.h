#ifndef CEPISODEDETAILS_H
#define CEPISODEDETAILS_H

#include <QWidget>

#include "cepisode.h"


namespace Ui {
class cEpisodeDetails;
}

class cEpisodeDetails : public QWidget
{
	Q_OBJECT

public:
	explicit cEpisodeDetails(QWidget *parent = 0);
	~cEpisodeDetails();

	void					setEpisode(cEpisode* lpEpisode);
	void					loadImages();
private:
	Ui::cEpisodeDetails*	ui;
	cEpisode*				m_lpEpisode;
	bool					m_bLoaded;
};

#endif // CEPISODEDETAILS_H
