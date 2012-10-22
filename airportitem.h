/*
 *  Copyright (C) 2010 Cutehacks AS. All rights reserved.
 *  info@cutehacks.com
 *  http://cutehacks.com
 *
 */

#ifndef AIRPORTITEM_H
#define AIRPORTITEM_H

#include <QWidget>
#include <QBasicTimer>

class AirportItem : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString airport READ airport)
    Q_PROPERTY(bool active READ active WRITE setActive)
public:
    AirportItem(int index, const QString airport, QWidget *parent = 0);
    ~AirportItem();

    QString airport() const;

    bool active() const;
    void setActive(bool set);

Q_SIGNALS:
    void activated(int index);

public Q_SLOTS:
    void startAnimation();
    void stopAnimation();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    int m_index;
    QString m_airport;
    bool m_active;
    bool m_pressed;
    QPoint m_pressPosition;
    QBasicTimer m_timer;
    unsigned int m_counter;
};

#endif
