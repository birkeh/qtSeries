#include "cseasondelegate.h"
#include "cserie.h"
#include "cseason.h"
#include "cepisode.h"

#include <QPainter>
#include <QTreeWidget>
#include <QDebug>


#define FIELD_WIDTH		3
#define FIELD_STEP		4
#define FIELD_HEIGHT	15
#define STATE_INIT		Qt::lightGray
#define STATE_PROGRESS	Qt::blue
#define STATE_DONE		Qt::green
#define STATE_UNKNOWN	Qt::black


void cSeasonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if(index.column() > 2)
	{
		cSeason*	lpSeason	= 0;

		if(index.data(Qt::UserRole).canConvert<cSeason*>())
			lpSeason	= qvariant_cast<cSeason*>(index.data(Qt::UserRole));

		if(option.state & QStyle::State_Selected)
			painter->fillRect(option.rect, option.palette.highlight());

		if(lpSeason)
		{
			QList<cEpisode*>	episodeList	= lpSeason->episodeList();

			painter->save();

			painter->setRenderHint(QPainter::Antialiasing, false);
			painter->setBrush(option.palette.foreground());

			painter->translate(option.rect.x(), option.rect.y());
			for(int z = 0;z < episodeList.count();z++)
			{
				painter->setPen(Qt::NoPen);
				cEpisode*	lpEpisode	= episodeList.at(z);
				if(lpEpisode)
				{
					switch(lpEpisode->state())
					{
					case cEpisode::StateInit:
						painter->setBrush(STATE_INIT);
						break;
					case cEpisode::StateProgress:
						painter->setBrush(STATE_PROGRESS);
						break;
					case cEpisode::StateDone:
						painter->setBrush(STATE_DONE);
						break;
					default:
						break;
					}
					painter->drawRect(z*FIELD_STEP, 0, FIELD_STEP, FIELD_HEIGHT);
					painter->setPen(Qt::black);
					painter->drawLine(z*FIELD_STEP+FIELD_WIDTH, 0, z*FIELD_STEP+FIELD_WIDTH, FIELD_HEIGHT);
				}
			}
			painter->setBrush(Qt::black);
			painter->translate(1.0, 0.0);
			painter->restore();
		}
	}
	else
	{
		QStyledItemDelegate::paint(painter, option, index);
	}
}

QSize cSeasonDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if(index.column() > 2)
	{
		qint16		iTotal		= -1;
		qint16		z;
		cSeason*	lpSeason	= 0;

		if(index.data(Qt::UserRole).canConvert<cSeason*>())
			lpSeason	= qvariant_cast<cSeason*>(index.data(Qt::UserRole));

		if(lpSeason)
		{
			QList<cEpisode*>	episodeList	= lpSeason->episodeList();
			for(z = 0;z < episodeList.count();z++)
			{
				cEpisode*	lpEpisode	= episodeList.at(z);
				if(lpEpisode)
				{
					if(lpEpisode->episodeNumber() > iTotal)
						iTotal	= lpEpisode->episodeNumber();
				}
			}
			if(iTotal > 0 && lpSeason->number() != 0)
				return(QSize((iTotal+1)*FIELD_STEP, FIELD_HEIGHT));
			else
				return(QSize(FIELD_STEP*40, FIELD_HEIGHT));
		}
		return(QSize(FIELD_STEP, FIELD_HEIGHT));
	}
	return QStyledItemDelegate::sizeHint(option, index);
}
