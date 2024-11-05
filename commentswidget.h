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


class CommentsWidget
{
public:
    enum class EditMode { Editable, ReadOnly };

    explicit CommentsWidget(QString& body_txt, QString& timestamp, QString& modified);
    explicit CommentsWidget() {};

    CommentsWidget(const CommentsWidget& other);
    CommentsWidget& operator=(const CommentsWidget& other);

    QSize sizeHint() const ;

    void paint(QPainter *painter, const QRect &rect, const QPalette &palette, EditMode mode = EditMode::ReadOnly) const;
    void setBodyText(QString& text)     { m_comment_body = text; };
    void setModifiedText(QString& text) { m_modified = text; };
    void setPostedDateText(QString& text) { m_modified = text; };

    QString bodyText()       const { return m_comment_body; };
    QString modifiedText()   const { return m_modified; };
    QString postedDateText() const { return m_comment_date; };

    QLinearGradient gradient() {
        QLinearGradient grad(QPointF(0, 0), QPointF(1, 1));
        grad.setColorAt(0, QColor(81, 161, 249));
        grad.setColorAt(0.38, QColor(150, 108, 230));
        grad.setColorAt(0.72, QColor(220, 139, 151));
        //grad.setColorAt(1, QColor(234, 166, 73));
        grad.setSpread(QLinearGradient::PadSpread);
        return grad;
    };

private:
    QString m_comment_body{};
    QString m_comment_date{};
    QString m_modified{};

    int m_card_h{};

    const QColor CARD_BG_NORMAL { QColor(245, 235, 224) };
    const QLinearGradient CARD_BG_SELECTED { this->gradient() };

    static constexpr int HEADER_FONT_SIZE{12};
    static constexpr int SUB_HEADER_FONT_SIZE{HEADER_FONT_SIZE-4};
    static constexpr int BODY_FONT_SIZE{14};
    static constexpr int CARD_BORDER_RADIUS{8};
    static constexpr int SIDE_MARGINS{12};
    static constexpr int SPACING{8};
    static constexpr int LINE_LENGHT{15};

    const QFont builtFont(int pointSize, QString family, QFont::Weight weight) const;
    void setBrushPenColor(QPainter* p, QColor brush_col = QColor(Qt::transparent), QColor pen_color = QColor(Qt::transparent)) const;
};

Q_DECLARE_METATYPE(CommentsWidget)

#endif // COMMENTSWIDGET_H
