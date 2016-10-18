#ifndef CSEASONDETAILS_H
#define CSEASONDETAILS_H


#include "cseason.h"

#include <QWidget>


namespace Ui {
class cSeasonDetails;
}

class cSeasonDetails : public QWidget
{
	Q_OBJECT

public:
	explicit cSeasonDetails(QWidget *parent = 0);
	~cSeasonDetails();

	void				setSeason(cSeason* lpSeason);

private slots:
	void				on_m_lpDetailsEpisodeTab_tabBarClicked(int index);

private:
	Ui::cSeasonDetails*	ui;
};

#endif // CSEASONDETAILS_H
