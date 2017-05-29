#include "cupdatethread.h"
#include "cserie.h"
#include "cseason.h"
#include "cepisode.h"
#include "cthetvdbv2.h"
#include "cmessagedialog.h"
#include <QMessageBox>

#include <QDebug>
#include <QTime>


cUpdateThread::cUpdateThread() :
	m_bStop(false)
{
}

void cUpdateThread::stop()
{
	QMutexLocker	locker(&m_mutex);
	m_bStop	= true;
	emit updateAppendMessage(" - cancel pending ...");
}

void cUpdateThread::run()
{
	QTime	timer;
	timer.restart();
	QString szFailed;

	for(int x = 0;x < m_indexList.count();x++)
	{
		cSerie*	lpSerie	= m_indexList.at(x).data(Qt::UserRole).value<cSerie*>();
		if(lpSerie)
		{
			emit updateMessage(lpSerie->seriesName(), x);
			cTheTVDBV2		tvDB;
			cSerie*			lpSerieNew;

			if(lpSerie->seriesID() != -1)
			{
				lpSerieNew	= tvDB.load(lpSerie->id(), "de");
				if(!lpSerieNew)
					lpSerieNew = tvDB.load(lpSerie->id(), "en");
				if(!lpSerieNew)
				{
					if(szFailed.length())
						szFailed += ", ";
					szFailed += lpSerie->seriesName();
					continue;
				}
				lpSerieNew->setDownload(lpSerie->download());
				for(int x = 0;x < lpSerieNew->seasonList().count();x++)
				{
					cSeason*	lpSeasonNew	= lpSerieNew->seasonList().at(x);
					for(int y = 0;y < lpSeasonNew->episodeList().count();y++)
					{
						cEpisode*	lpEpisodeNew	= lpSeasonNew->episodeList().at(y);
						cEpisode*	lpEpisode		= lpSerie->findEpisode(lpEpisodeNew->id());
						if(lpEpisode)
							lpEpisodeNew->setState(lpEpisode->state());
					}
				}
                lpSerieNew->setCliffhanger(lpSerie->cliffhanger());
				lpSerie->del(m_db);
				lpSerieNew->save(m_db);
			}
		}

		QMutexLocker	locker(&m_mutex);
		if(m_bStop)
			break;
		msleep(10);
	}
/*
	if(szFailed.length())
	{
		QMessageBox	msgBox;
		msgBox.setText(szFailed + QString(" has failed to update."));
		msgBox.exec();
	}
*/
}

void cUpdateThread::setData(cMessageDialog* lpMessageDialog, const QModelIndexList &indexList, const QSqlDatabase& db)
{
	connect(lpMessageDialog->cancelButton(), SIGNAL(clicked()), this, SLOT(stop()));
	m_indexList	= indexList;
	m_db		= db;
}
