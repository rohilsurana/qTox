/*
    Copyright (C) 2014 by Project Tox <https://tox.im>

    This file is part of qTox, a Qt-based graphical interface for Tox.

    This program is libre software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    See the COPYING file for more details.
*/

#ifndef IDENTITYFORM_H
#define IDENTITYFORM_H

#include <QLineEdit>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include "src/core/core.h"
#include "src/misc/qrwidget.h"

class CroppingLabel;
class Core;
class MaskablePixmapWidget;

namespace Ui {
class IdentitySettings;
class MainWindow;
}

class ClickableTE : public QLineEdit
{
    Q_OBJECT
public:

signals:
    void clicked();
protected:
    void mouseReleaseEvent(QMouseEvent*) {emit clicked();}    
};

class ProfileForm : public QWidget
{
    Q_OBJECT
public:
    ProfileForm(QWidget *parent = nullptr);
    ~ProfileForm();
    void show(Ui::MainWindow &ui);

signals:
    void userNameChanged(QString);
    void statusMessageChanged(QString);

public slots:
    void onSelfAvatarLoaded(const QPixmap &pic);
    void onStatusSet(Status status);

private slots:
    void setToxId(const QString& id);
    void copyIdClicked();
    void onAvatarClicked();
    void onUserNameEdited();
    void onStatusMessageEdited();
    void onLoadClicked();
    void onRenameClicked();
    void onExportClicked();
    void onDeleteClicked();
    void onImportClicked();
    void onNewClicked();
    void disableSwitching();
    void enableSwitching();
    void on_copyQr_clicked();
    
    void on_saveQr_clicked();
    
protected:
    virtual void showEvent(QShowEvent *);
    bool eventFilter(QObject *o, QEvent *e);

private:
    void refreshProfiles();
    Ui::IdentitySettings* bodyUI;
    MaskablePixmapWidget* profilePicture;
    QWidget *head;
    Core* core;
    QTimer timer;
    bool hasCheck = false;
    QRWidget *qr;

    ClickableTE* toxId;
};

#endif
