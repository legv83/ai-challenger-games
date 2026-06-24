// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef HELPER_H
#define HELPER_H

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>
#include <QtWebSockets/QWebSocket>

//! [0]
class Helper: public QObject
{
public:
    Helper(QString id, int seed);

public:
    void paint(QPainter *painter, QPaintEvent *event, int elapsed);
    void key_handler(QKeyEvent *event);
    void send_data(QWidget *canvas);

private:
    QBrush background;
    QBrush circleBrush;
    QFont textFont;
    QPen circlePen;
    QPen textPen;
    float cur_x, cur_y;
    float angle, speed;
    float positions[10][2];
    float t_angles[10];

    int cur_enemy;
    QWebSocket m_webSocket;
    int connected = 0;
    QString id;

private slots:
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(QString message);
};
//! [0]

#endif
