/*
 *  Copyright (C) 2010 Cutehacks AS. All rights reserved.
 *  info@cutehacks.com
 *  http://cutehacks.com
 *
 */

#include "airportitem.h"
#include "constants.h"

#include <QPainter>
#include <QMouseEvent>

AirportItem::AirportItem(int index, const QString airport, QWidget *parent)
    : QWidget(parent),
      m_index(index),
      m_airport(airport),
      m_active(false),
      m_pressed(false),
      m_counter(0)
{
    setFixedHeight(48);
    QFont fnt(font());
    fnt.setPixelSize(pixelSizes[0][1]);
    setFont(fnt);
}

AirportItem::~AirportItem()
{
}

QString AirportItem::airport() const
{
    return m_airport;
}

bool AirportItem::active() const
{
    return m_pressed;
}

void AirportItem::setActive(bool set)
{
    m_active = set;
    update();
}

void AirportItem::startAnimation()
{
    m_counter = 0;
    m_timer.start(200, this);
}

void AirportItem::stopAnimation()
{
    m_timer.stop();
    m_counter = 0;
}

void AirportItem::mousePressEvent(QMouseEvent *event)
{
    m_pressPosition = event->pos();
    m_pressed = true;
    update();
    event->ignore();
}

void AirportItem::mouseMoveEvent(QMouseEvent *event)
{
    const QPoint movement = event->pos() - m_pressPosition;
    if (movement.manhattanLength() > 16) {
        m_pressed = false;
        update();
    }
    event->ignore();
}

void AirportItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_pressed) {
        emit activated(m_index);
        m_pressed = false;
        update();
    }
    event->ignore();
}

void AirportItem::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    QColor color;
    if (m_active) {
       QFont fnt(font());
       fnt.setBold(true);
       painter.setFont(fnt);
       color = Qt::white;
    } else {
       color = palette().color(QPalette::Text);
    }

    const QRect bounds = rect().adjusted(12, 8, -12, -8);
    if (m_pressed) {
        painter.fillRect(bounds, color);
        painter.setPen(palette().color(QPalette::Base));
    } else {
        painter.setPen(color);
    }
    painter.drawText(rect().adjusted(16, 0, -16, 0), m_airport, QTextOption(Qt::AlignVCenter));

    if (m_timer.isActive()) {
        const int x = width() - 96;
        const int h = height() - 32;
        unsigned int i = (m_counter % 5);
        for (unsigned int j = 0; j < 5; ++j)
            painter.fillRect(x +  (j * 16) + 1, 16, 14, h, i == j ? color : Qt::darkGray);
    }
}

void AirportItem::timerEvent(QTimerEvent *event)
{
    if (m_timer.timerId() == event->timerId()) {
        ++m_counter;
        update();
    }
}
