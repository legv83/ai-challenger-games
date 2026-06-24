// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "helper.h"
#include "math.h"

#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include <QtCore/QUrl>
#include <QBuffer>

//! [0]
Helper::Helper(QString id, int seed)
{
    QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
    connect(&m_webSocket, &QWebSocket::connected, this, &Helper::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &Helper::onDisconnected);
    this->id = id;
    m_webSocket.open(QUrl("ws://localhost:5555/steer/out?ID=" + this->id));

    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(1.0, Qt::blue);
    angle=0;
    speed=.50;
    cur_x=0;
    cur_y=0;
    background = QBrush(QColor(64, 32, 64));
    circleBrush = QBrush(gradient);
    circlePen = QPen(Qt::black);
    circlePen.setWidth(1);
    textPen = QPen(Qt::white);
    textFont.setPixelSize(50);
    srand(seed);
    for (int i=0;i<10;i++) {
        positions[i][0] = (rand() % 200) - 100;
        positions[i][1] = (rand() % 200) - 100;
        t_angles[i] = (rand() % 360);
    }
    cur_enemy=0;
}
//! [0]

//! [1]
void Helper::paint(QPainter *painter, QPaintEvent *event, int elapsed)
{
    painter->fillRect(event->rect(), background);
    painter->translate(100, 100);
//! [1]

//! [2]
    painter->save();
    painter->setBrush(circleBrush);
    painter->setPen(circlePen);

//! [2]

//! [3]}
//!

    painter->setBrush(Qt::cyan);
    painter->setPen(Qt::darkCyan);

    painter->drawEllipse(QRectF(cur_x - 5, cur_y - 5,
                                10, 10));

    painter->setBrush(Qt::white);
    painter->setPen(Qt::white);


    painter->drawEllipse(QRectF(positions[cur_enemy][0] - 5, positions[cur_enemy][1] - 5,
                                10, 10));
    painter->restore();

    if (fabs(cur_x - positions[cur_enemy][0]) < 5 && fabs(cur_y - positions[cur_enemy][1]) < 5) {
        if (cur_enemy<9)
            cur_enemy++;
        else
            exit(0);

    }

    //painter->drawText(QRect(-50, -50, 100, 100), Qt::AlignCenter, QStringLiteral("Qt"));

    painter->setPen(textPen);
    painter->setFont(textFont);

    cur_x = cur_x + sin(angle/180.0 * M_PI) * speed;
    if (cur_x > 100) {cur_x = 100; angle = -angle;}
    if (cur_x < -100) {cur_x = -100; angle = -angle;}

    cur_y = cur_y - cos(angle/180.0 * M_PI) * speed;
    if (cur_y > 100) {cur_y = 100; angle = 180 - angle;}
    if (cur_y < -100) {cur_y = -100; angle = 180 - angle;}

    float enemy_speed = .25;
    positions[cur_enemy][0] = positions[cur_enemy][0] + sin(t_angles[cur_enemy]/180.0 * M_PI) * enemy_speed;
    if (positions[cur_enemy][0] > 100) {positions[cur_enemy][0] = 100; t_angles[cur_enemy] = -t_angles[cur_enemy];}
    if (positions[cur_enemy][0] < -100) {positions[cur_enemy][0] = -100; t_angles[cur_enemy] = -t_angles[cur_enemy];}

    positions[cur_enemy][1] = positions[cur_enemy][1] - cos(t_angles[cur_enemy]/180.0 * M_PI) * enemy_speed;
    if (positions[cur_enemy][1] > 100) {positions[cur_enemy][1] = 100; t_angles[cur_enemy] = 180 - t_angles[cur_enemy];}
    if (positions[cur_enemy][1] < -100) {positions[cur_enemy][1] = -100; t_angles[cur_enemy] = 180 - t_angles[cur_enemy];}

}

void Helper::send_data(QWidget *canvas) {

    // 3. Save the image into a QByteArray via QBuffer
    QPixmap pixmap = canvas->grab();
    QImage image = pixmap.toImage();

    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");

    QString base64String = buffer.data().toBase64();
    QString dataUri = "data:image/png;base64,"+ base64String;

    if (connected == 1) {
        m_webSocket.sendTextMessage(("pos:" + QString::number(cur_x, 'f', 2) + "," + QString::number(cur_y, 'f', 2) + "\nSpeed:" + QString::number(speed, 'f', 2) + "\nAngle:" + QString::number(angle, 'f', 2)) + "\ntarget_pos:" + QString::number(positions[cur_enemy][0], 'f', 2) + "," + QString::number(positions[cur_enemy][1], 'f', 2) + "\nsnap:"+dataUri);
    }
}

void Helper::key_handler(QKeyEvent *event)
{
    qDebug()<<  event->key() ;
    if (event->key() == 16777234) {
        angle-=2;
    }
    if (event->key() == 16777236) {
        angle+=2;
    }
    if (event->key() == 16777237) {
        speed--;
    }
    if (event->key() == 16777235) {
        speed++;
    }
    qDebug()<< angle << ", "<<speed;
    qDebug()<< cur_x << ", "<<cur_y;

}

void Helper::onConnected()
{
    qDebug() << "WebSocket secure connection established!";

    // Listen for incoming text payloads from the server
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &Helper::onTextMessageReceived);

    // Broadcast a payload to the remote host
    m_webSocket.sendTextMessage(QStringLiteral("Hello from Qt 6 WebSockets!"));
    connected=1;
}

void Helper::onDisconnected()
{
    connected=0;
}

void Helper::onTextMessageReceived(QString message)
{
    qDebug()<<message;
    if (message.contains("j")) {
        angle-=5;
    }
    if (message.contains("l")) {
        angle+=5;
    }
    if (message.contains("i")) {
        speed++;
    }
    if (message.contains("k")) {
        speed--;
    }
    if (message.contains("x")) {
        exit(0);
    }
}
//! [3]
