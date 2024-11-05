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

#include "commentswidget.h"
#include <QTextEdit>

CommentsWidget::CommentsWidget(
    QString& body_txt,
    QString& timestamp,
    QString& modified
    ) : m_comment_body{ body_txt }, m_comment_date{ timestamp }, m_modified{ modified }
{ }


CommentsWidget::CommentsWidget(const CommentsWidget& other)
{
    this->m_comment_body = other.m_comment_body;
    this->m_comment_date = other.m_comment_date;
    this->m_modified = other.m_modified;
}


CommentsWidget& CommentsWidget::operator=(const CommentsWidget& other)
{
    if (this == &other) return *this;

    this->m_comment_body = other.m_comment_body;
    this->m_comment_date = other.m_comment_date;
    this->m_modified = other.m_modified;
    return *this;
}


void CommentsWidget::paint(QPainter *painter, const QRect &rect, const QPalette &palette, EditMode mode) const
{
    Q_UNUSED(palette)

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);

    /*  transparent background   */
    //painter->fillRect(rect, Qt::transparent);

    /* card bg and border + clipping */
    QPainterPath card_path;
    card_path.addRoundedRect(rect.x(), rect.y(), rect.width(), rect.height(), CARD_BORDER_RADIUS, CARD_BORDER_RADIUS);
    painter->setClipPath(card_path, Qt::IntersectClip);
    if (mode == EditMode::Editable)
    {
        QLinearGradient grad(rect.topLeft(), rect.topRight());
        grad.setColorAt(0, QColor(81, 161, 249));
        grad.setColorAt(0.38, QColor(150, 108, 230));
        grad.setColorAt(0.72, QColor(220, 139, 151));
        grad.setColorAt(1, QColor(234, 166, 73));
        grad.setSpread(QLinearGradient::PadSpread);

        painter->setBrush(grad);
    } else {
        painter->setBrush(CARD_BG_NORMAL);
    }

    painter->drawRoundedRect(rect.x(), rect.y(), rect.width(), rect.height(), CARD_BORDER_RADIUS, CARD_BORDER_RADIUS);

    /*   header    */
    this->setBrushPenColor(painter, QColor(Qt::black), QColor(Qt::black));
    auto headers_font = this->builtFont(HEADER_FONT_SIZE, "Inter", QFont::ExtraBold);

    /* timestamp header */
    painter->drawText(QPoint(SIDE_MARGINS+rect.x(), SPACING+rect.y()+8), m_comment_date);

    /* calculate font bound */
    QFontMetrics header_metrics(headers_font);
    int timestamp_h = header_metrics.boundingRect(m_comment_date).height();


    /* text drawn only if the comment was modified */
    int subheader_m_h{};
    if (m_modified != "")
    {
        this->setBrushPenColor(painter, QColor(129, 18, 18), QColor(129, 18, 18));
        auto subHeaderFont = builtFont(SUB_HEADER_FONT_SIZE, "Inter", QFont::Medium);

        painter->setFont(subHeaderFont);

        int offset = SPACING + timestamp_h + 4;
        painter->drawText(QPoint(SIDE_MARGINS+rect.x(), offset+rect.y()), m_modified);

        QFontMetrics subheader_m(subHeaderFont);
        subheader_m_h = subheader_m.boundingRect(m_modified).height();
    }

    /* BODY TEXT */
    QFont body_font = this->builtFont(BODY_FONT_SIZE, "Inter", QFont::Normal);
    painter->setFont(body_font);

    this->setBrushPenColor(painter, QColor(Qt::black), QColor(Qt::black));

    /* --- word wrapping --- */
    QTextOption txt_opt;
    txt_opt.setAlignment(Qt::AlignLeft);
    txt_opt.setWrapMode(QTextOption::WordWrap);

    auto lines_count = (m_comment_body.length() / LINE_LENGHT) + 1;
    auto body_single_line_h = QFontMetrics(
                    this->builtFont(BODY_FONT_SIZE, "Inter", QFont::Normal)
                ).boundingRect(m_comment_body).height();

    QRectF body_rect(
            SIDE_MARGINS+rect.x(),
            SPACING + timestamp_h + 4 + subheader_m_h + SIDE_MARGINS + rect.y(),
            275,
            lines_count * body_single_line_h
        );

    painter->drawText(body_rect, m_comment_body, txt_opt);

    painter->restore();
}


const QFont CommentsWidget::builtFont(int pointSize, QString family, QFont::Weight weight) const
{
    QFont font;
    font.setPointSize(pointSize);
    font.setFamily(family);
    font.setWeight(weight);
    return font;
}


void CommentsWidget::setBrushPenColor(QPainter* p, QColor brush_col, QColor pen_color) const
{
    p->setBrush(brush_col);
    p->setPen(pen_color);
}


QSize CommentsWidget::sizeHint() const
{
    auto h1 = QFontMetrics(
            this->builtFont(HEADER_FONT_SIZE, "Inter", QFont::DemiBold)
        ).boundingRect(m_comment_date).height();

    auto h2 = QFontMetrics(
            this->builtFont(SUB_HEADER_FONT_SIZE, "Inter", QFont::DemiBold)
        ).boundingRect(m_modified).height();

    auto lines_count = (m_comment_body.length() / LINE_LENGHT) + 1;
    // qDebug() << "body lenght:" << m_comment_body.length() << "\nLine lenght:" << LINE_LENGHT;

    auto body = QFontMetrics(
            this->builtFont(BODY_FONT_SIZE, "Inter", QFont::Normal)
        ).boundingRect(m_comment_body).height();

    body = body * lines_count;

    return QSize(
            (SIDE_MARGINS * 2) + 275,
            SPACING + h1 + 4 + h2 + SIDE_MARGINS + body + SIDE_MARGINS
        );
}

