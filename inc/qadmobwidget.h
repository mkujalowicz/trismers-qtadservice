#ifndef QADMOBWIDGET_H
#define QADMOBWIDGET_H

#include <QWidget>
#include "qadmob.h"

class QADMOBSHARED_EXPORT QAdMobWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QAdMobWidget(QWidget *parent = 0);

    QAdMob& ad();

    /* from QWidget */
    QSize sizeHint() const;

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

signals:
    void adFetchResult(bool);

public slots:
    void fetchAd();
    void adReceived(bool);

private:

    QAdMob*  iAdMob;


};

#endif // QADMOBWIDGET_H
