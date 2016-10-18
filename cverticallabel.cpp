#include "cverticallabel.h"

#include <QPainter>


cVerticalLabel::cVerticalLabel(QWidget *parent)
	: QLabel(parent)
{
}

cVerticalLabel::cVerticalLabel(const QString &text, QWidget *parent)
	: QLabel(text, parent)
{
}

void cVerticalLabel::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setPen(Qt::black);
	painter.setBrush(Qt::Dense1Pattern);

	painter.translate(sizeHint().width(), sizeHint().height());
	painter.rotate(270);

	painter.drawText(0,0, text());
}

QSize cVerticalLabel::minimumSizeHint() const
{
	QSize s = QLabel::minimumSizeHint();
	return QSize(s.height(), s.width());
}

QSize cVerticalLabel::sizeHint() const
{
	QSize s = QLabel::sizeHint();
	return QSize(s.height(), s.width());
}
