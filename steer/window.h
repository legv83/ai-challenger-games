// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef WINDOW_H
#define WINDOW_H

#include "helper.h"

#include <QWidget>

//! [0]
class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QString ID = "", int seed = 1);

private:
    Helper *helper;

protected:
    void keyPressEvent(QKeyEvent *event) override;
};
//! [0]

#endif
