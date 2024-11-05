/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the QtSql module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "commentlistitem.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setUpDb();

    // set the model to QList and QTable
    // _m_model = new QSqlQueryModel;
    _m_model = new QSqlTableModel;
    _m_model->setQuery("SELECT * FROM veh_comments;");
    _m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    ui->tb_view->setModel(_m_model);

    auto header = ui->tb_view->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tb_view->show();

    ui->lst_vw->setItemDelegate( new CommentListItem );
    ui->lst_vw->setEditTriggers(QAbstractItemView::EditTrigger::DoubleClicked);

    ui->lst_vw->setModel(_m_model);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_serach_le_returnPressed()
{
    // read the id from the lineEdit
    auto id = ui->serach_le->text();

    if (id == "")
    {
        QSqlQuery query("SELECT * FROM veh_comments");
        _m_model->setQuery(std::move(query));
    } else {
        // UPDATE VIEW
        auto query = commentsPerChassisId(id);
        _m_model->setQuery(std::move(query));
    }
    qDebug() << "Line Edit \"Return_Key\" pressed..\n\n";
}

QSqlQuery MainWindow::commentsPerChassisId(const QString& id)
{
    QSqlQuery my_query(_m_db);
    my_query.prepare(
        "SELECT "
            "veh.*, us.user_name "

            "FROM veh_comments AS veh "
                "INNER JOIN "
                    "users AS us "
                "ON us.id = veh.user_id "
        "WHERE "
            "veh_serial = ? "

        "ORDER BY "
            "comment_datetime DESC "
        );

    my_query.addBindValue(id);
    my_query.exec();

    return my_query;
}

void MainWindow::setUpDb()
{
    _m_db = QSqlDatabase::addDatabase("QPSQL");
    _m_db.setHostName("localhost");
    _m_db.setDatabaseName("test");
    _m_db.setUserName("postgres");
    _m_db.setPassword("SomeBasicPassword");   // <-- dummy password


    if (!_m_db.open()) {
        qDebug() << "Error: Could not open database";
    }
}


