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

#ifndef COMMENTSWIDGET_H
#define COMMENTSWIDGET_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QTextLayout>
#include <QDateTime>


class CommentsWidget
{
    const QColor CARD_BG_NORMAL { QColor(245, 235, 224) };

    static constexpr int HEADER_FONT_SIZE{10};
    static constexpr int SUB_HEADER_FONT_SIZE{HEADER_FONT_SIZE-2};
    static constexpr int BODY_FONT_SIZE{14};

    static constexpr int CARD_BORDER_RADIUS{8};
    static constexpr int SIDE_MARGINS{12};
    static constexpr int TOP_MARGIN{20};

    static constexpr int SPACING{8};
    static constexpr int LINE_LENGHT{30};

    const QColor DEFAULT_COLOR{ QColor(Qt::black) };
    const QColor SELECTED_COLOR{ QColor(Qt::white) };
    const QColor EDIT_DATE_DEFAULTL_COLOR{ QColor(129, 18, 18) };
    const QColor EDIT_DATE_SELECTED_COLOR{ QColor(49, 15, 218) };

public:
    enum class EditMode { Editable, ReadOnly };
    enum class DateType { EditDate, PostDate };

    explicit CommentsWidget(QString& timestamp, QString& modified, QString& body_txt);
    explicit CommentsWidget() {};

    CommentsWidget(const CommentsWidget& other);
    CommentsWidget& operator=(const CommentsWidget& other);

    QSize sizeHint() const ;

    void paint( QPainter *painter, const QRect &rect, const QPalette &palette, EditMode mode = EditMode::ReadOnly) const;

    void setCommentBody( QString& text )       { m_comment_body = text; };
    void setEditingDate( QString& text )   { m_editing_date = text; };
    void setPostDate( QString& text ) { m_comment_date = text; };

    QString bodyText()        const { return m_comment_body; };
    QString editingDateText() const { return m_editing_date; };
    QString postedDateText()  const { return m_comment_date; };

    static QString formatDatetime( QDateTime& dt, QString username = "emptyUser", DateType type = DateType::EditDate);

private:
    QString m_comment_date{};
    QString m_editing_date{};
    QString m_comment_body{};

    const QFont builtFont(int pointSize, QString family, QFont::Weight weight) const;
    void setBrushPenColor(QPainter* p, QColor brush_col = QColor(Qt::transparent), QColor pen_color = QColor(Qt::transparent)) const;

    int drawPostDate(QPainter *painter, const QRect &rect, EditMode mode) const;
    int drawPostBody(QPainter *painter, const QRect &rect, EditMode mode, int y_offset) const;
    int drawEditingDate(QPainter *painter, const QRect &rect, EditMode mode, int y_offset) const;

    int createTextLayout(const QString& text, QPointF& xy_offset, int wrapAtWidth_px, QPainter* painter = nullptr) const;
    int createTextLayout(const QString& text, int y_offset, QFont& font, int wrapAtWidth_px) const;
};

Q_DECLARE_METATYPE(CommentsWidget)

#endif // COMMENTSWIDGET_H
