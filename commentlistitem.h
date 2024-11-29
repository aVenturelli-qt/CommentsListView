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

#ifndef COMMENTLISTITEM_H
#define COMMENTLISTITEM_H

#include <QObject>
#include <QWidget>
#include <QDateTime>
#include <QStyledItemDelegate>
#include <QDateTime>
#include <QSqlRelationalTableModel>
#include <QSqlError>
#include "commentswidget.h"

class CommentListItem : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CommentListItem();
    CommentListItem(QString& username);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void setUsername(const QString& username) { m_username = username; }

    /*!
     *  Take an index and use the values provided by the list
     *  to target the right Column for retriving the correct data:
     *  es.. QModelIndex.siblingAtColumn( QList[n] ).data()
     *
     *  \fn CommentsWidget* CommentListItem::createCommentWidgetFromIndex(QModelIndex& idx, QList<int> data_at_columns)
     *  \param idx: the QModelIndex where the record receives
     *  \param data_at_columns: a QList<int> containing 3 values -> [0] post_date, [1] editing_date, [2] comment's body

    */
    CommentsWidget* createCommentWidgetFromIndex(const QModelIndex& idx, QList<int> data_at_columns) const;

private slots:
    void commitAndCloseEditor();

private:
    QString m_username;
};



#endif // COMMENTLISTITEM_H
