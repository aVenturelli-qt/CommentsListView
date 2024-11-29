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
#include "db_connection_info.h"

/* WARNING HERE*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // establish a connection tòthe DB
    this->setUpDbConnection();

    // set the model to QList and QTable
    m_model = new QSqlRelationalTableModel(this, m_db);
    m_model->setTable("comments_table");
    m_model->setRelation(1, QSqlRelation("vin_table", "id", "vin"));
    m_model->setRelation(2, QSqlRelation("users_table", "id", "mail"));
    m_model->setJoinMode(QSqlRelationalTableModel::InnerJoin);

    // sorts ascending by ID column
    m_model->sort(0, Qt::AscendingOrder);
    m_model->select();


    qDebug() << "address of model inside MainWindow::MainWindow(): " << m_model;


    // set model and delegate to the view
    ui->tb_view->setModel(m_model);
    ui->tb_view->setItemDelegate(new QSqlRelationalDelegate(ui->tb_view));

    // resize column to fit content
    auto header = ui->tb_view->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tb_view->show();

    QString username("123user@gmail.com");     /// this must be removed

    CommentListItem* commentItem = new CommentListItem{ username };
    ui->lst_vw->setItemDelegate( commentItem );
    ui->lst_vw->setEditTriggers(QAbstractItemView::EditTrigger::DoubleClicked);

    ui->lst_vw->setModel(m_model);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_search_le_returnPressed()
{
    // get the content of QLineEdit
    auto txt = this->ui->search_le->text();

    // set filter whereas txt is empty or not
    auto filter = (!txt.isEmpty())? QString("vin = '%1'").arg(txt) : "";
    this->m_model->setFilter(filter);
    this->m_model->select();
}


void MainWindow::setUpDbConnection()
{
    // connecting to the postgres database
    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setHostName(db_info::HOST);
    m_db.setDatabaseName(db_info::DB_NAME);
    m_db.setUserName(db_info::USER_NAME);
    m_db.setPassword(db_info::PASSWORD);
    m_db.setPort(db_info::PORT);

    if (!m_db.open()) {
        qDebug() << "Error: Could not open database";
    }
}


