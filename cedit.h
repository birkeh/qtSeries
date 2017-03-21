#ifndef CEDIT_H
#define CEDIT_H


#include "cserie.h"

#include <QDialog>
//#include <QDialogButtonBox>


namespace Ui {
class cEdit;
}

class cEdit : public QDialog
{
	Q_OBJECT

public:
	explicit cEdit(QWidget *parent = 0);
	~cEdit();

	void		setSerie(cSerie* lpSerie);
	QString		download();
private slots:
	void		lpAllInit_clicked();
	void		lpAllProgress_clicked();
	void		lpAllDone_clicked();

	void		allInit_clicked();
	void		allProgress_clicked();
	void		allDone_clicked();

	void		on_m_lpTabWidget_tabBarClicked(int index);

	void on_m_lpCliffhanger_clicked();

    void on_m_lpFirstAired_dateChanged(const QDate &date);

private:
	Ui::cEdit*			ui;
	bool				m_bLoaded;
	QRadioButton*		m_lpButton1;
	QRadioButton*		m_lpButton2;
	QRadioButton*		m_lpButton3;
	QPushButton*		m_lpInit;
	QPushButton*		m_lpProgress;
	QPushButton*		m_lpDone;

	QGroupBox*			m_lpGroupBox;
	QGridLayout*		m_lpGridLayout;
	QGridLayout*		m_lpGrid;
	QLabel*				m_lpLabel;
	QLabel*				m_lpLabel1;
	QSpacerItem*		m_lpSpacer;
	QButtonGroup*		m_lpGroup;
	QPushButton*		m_lpAllInit;
	QPushButton*		m_lpAllProgress;
	QPushButton*		m_lpAllDone;

	QSpacerItem*		m_lpVerticalSpacer;
//	QDialogButtonBox*	m_lpButtonBox;

	cSerie*				m_lpSerie;
};

#endif // CEDIT_H
