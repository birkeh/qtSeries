#ifndef CVERTICALLABEL_H
#define CVERTICALLABEL_H


#include <QLabel>


class cVerticalLabel : public QLabel
{
	Q_OBJECT
public:
	explicit	cVerticalLabel(QWidget *parent = 0);
	explicit	cVerticalLabel(const QString &text, QWidget *parent=0);

signals:

protected:
	void		paintEvent(QPaintEvent*);
	QSize		sizeHint() const ;
	QSize		minimumSizeHint() const;

public slots:

private:
};

#endif // CVERTICALLABEL_H
