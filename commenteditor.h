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

#ifndef COMMENTEDITOR_H
#define COMMENTEDITOR_H
#include <QWidget>
#include <QKeyEvent>
#include <QEvent>
#include "commentswidget.h"

namespace Ui {
class CommentEditor;
}

class CommentEditor : public QWidget
{
    Q_OBJECT

public:
    explicit CommentEditor(CommentsWidget* comment_wdg, QWidget *parent = nullptr);
    explicit CommentEditor(QWidget *parent = nullptr);
    ~CommentEditor();

    QSize sizeHint() const override;
    void setCommentsWidget(CommentsWidget* commentWidget);
    CommentsWidget commentWidget() const { return *m_comment_wgd; };

    /* SETTERS */
    void setBodyText(QString& txt);
    void setBodyText(char* txt);

    void setPostDate(QString& txt);
    void setPostDate(char* txt);

    void setEditingDate(QString& txt);
    void setEditingDate(char* txt);

    /* GETTERS */
    QString bodyText() const;
    QString datetimeStr() const { return m_datetime_str; };
    QString modifiedDatetimeStr() const { return m_mod_datetime_str; };

signals:
    void editingFinished();

private:
    Ui::CommentEditor *ui;
    CommentsWidget* m_comment_wgd{};
    QString m_body_txt{};
    QString m_datetime_str{};
    QString m_mod_datetime_str{};

    bool eventFilter(QObject *object, QEvent *event) override;
};

#endif // COMMENTEDITOR_H
