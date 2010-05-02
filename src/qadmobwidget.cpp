#include "qadmobwidget.h"
#include "qadmob.h"

#include <QDesktopServices>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>


static const QSize kDefaultWidgetSize = QSize (200,50);

QAdMobWidget::QAdMobWidget(QWidget *parent) :
    QWidget(parent),
    iAdMob ( new QAdMob(this))
{
     setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

     connect(iAdMob, SIGNAL(adReceived(bool)), this, SLOT(adReceived(bool)));
}

 QAdMob& QAdMobWidget::ad()
 {
     return *iAdMob;
 }

QSize QAdMobWidget::sizeHint() const
{
    if (iAdMob->adReady())
    {
        return iAdMob->ad().size();
    }
    else
    {
        return kDefaultWidgetSize;
    }
}

void QAdMobWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.drawRect(rect());

    if (iAdMob->adReady())
    {
        QImage adTitleImage = iAdMob->ad().adTitleImage();
        QImage adUrlTypeImage = iAdMob->ad().adUrlTypeImage();
        QSize adSize = iAdMob->ad().size();

        QPoint point(0,0);
        painter.drawImage(point, adTitleImage);

        painter.drawText( QPoint ( adTitleImage.size().width() + 5, adTitleImage.size().height()/3 ),
                          iAdMob->ad().text());

        painter.drawText( QPoint ( adTitleImage.size().width() + 5 , adTitleImage.size().height()),
                         iAdMob->ad().adMobStampText());

        point.setX( adSize.width() - adUrlTypeImage.size().width());
        point.setY( (adSize.height() - adUrlTypeImage.size().height() )/2 );
        painter.drawImage(point, adUrlTypeImage);

    }


}

void QAdMobWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && iAdMob->adReady())
    {
        QDesktopServices::openUrl(iAdMob->ad().url());
    }
}

void QAdMobWidget::fetchAd()
{
    iAdMob->fetchAd();
}

void QAdMobWidget::adReceived(bool aResult)
{
    if (aResult)
    {
        setFixedSize(iAdMob->ad().size());
        setCursor(Qt::PointingHandCursor);
    }

}
