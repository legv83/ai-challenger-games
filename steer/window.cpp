// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "glwidget.h"
#include "widget.h"
#include "window.h"

#include <QGridLayout>
#include <QLabel>
#include <QTimer>
#include <QKeyEvent>

//! [0]
Window::Window(QString ID, int seed)
{
    setWindowTitle(tr("2D Painting on Native and OpenGL Widgets"));
    helper = new Helper(ID, seed);
    Widget *native = new Widget(helper, this);
    QLabel *nativeLabel = new QLabel(tr("Native"));
    nativeLabel->setAlignment(Qt::AlignHCenter);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(native, 0, 0);
    layout->addWidget(nativeLabel, 1, 0);
    setLayout(layout);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, native, &Widget::animate);
    timer->start(50);
}
//! [0]

void Window::keyPressEvent(QKeyEvent *event)
{
    helper->key_handler(event);
}