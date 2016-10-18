#include "cmessageanimatedialog.h"
#include "ui_cmessageanimatedialog.h"


cMessageAnimateDialog::cMessageAnimateDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::cMessageAnimateDialog),
	m_szMessage(""),
	m_lpTimer(0),
	m_iCount(0),
	m_iDirection(1)
{
	ui->setupUi(this);
}

cMessageAnimateDialog::~cMessageAnimateDialog()
{
	if(m_lpTimer)
		delete m_lpTimer;
	delete ui;
}

void cMessageAnimateDialog::setTitle(const QString& szTitle)
{
	setWindowTitle(szTitle);
}

void cMessageAnimateDialog::setMessage(const QString& szMessage)
{
	m_szMessage	= szMessage;
	ui->m_lpText->setText(m_szMessage);

	if(m_lpTimer)
		delete m_lpTimer;

	m_lpTimer = new QTimer(this);
	connect(m_lpTimer, SIGNAL(timeout()), this, SLOT(update()));
	m_lpTimer->start(500);
}

void cMessageAnimateDialog::update()
{
	m_iCount	+= m_iDirection;
	if(m_iCount == 4)
	{
		m_iCount		= 2;
		m_iDirection	= -1;
	}
	else if(m_iCount == -1)
	{
		m_iCount		= 1;
		m_iDirection	= 1;
	}
	QString	sz	= m_szMessage;
	for(int z = 0;z < m_iCount;z++)
		sz.append(".");
	ui->m_lpText->setText(sz);
}
