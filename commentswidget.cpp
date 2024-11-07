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

/*
 * START construncturs, deconstructor, operetors definitions
*/
CommentsWidget::CommentsWidget(
    QString& timestamp,
    QString& modified,
    QString& body_txt
    ) : m_comment_date{ timestamp }, m_editing_date{ modified }, m_comment_body{ body_txt } {}

CommentsWidget::CommentsWidget(const CommentsWidget& other)
{
    this->m_comment_body = other.m_comment_body;
    this->m_comment_date = other.m_comment_date;
    this->m_editing_date = other.m_editing_date;
}

CommentsWidget& CommentsWidget::operator=(const CommentsWidget& other)
{
    if (this == &other) return *this;

    this->m_comment_body = other.m_comment_body;
    this->m_comment_date = other.m_comment_date;
    this->m_editing_date = other.m_editing_date;
    return *this;
}
/*
 * END construncturs, deconstructor, operetors definitions
*/

void CommentsWidget::paint(QPainter *painter, const QRect &rect, const QPalette &palette, EditMode mode) const
{
    Q_UNUSED(palette)
    QLinearGradient selected_grad(rect.topLeft(), rect.topRight());
    selected_grad.setColorAt(   0, QColor(81, 161, 249));
    selected_grad.setColorAt(0.38, QColor(150, 108, 230));
    selected_grad.setColorAt(0.72, QColor(220, 139, 151));
    selected_grad.setColorAt(   1, QColor(234, 166, 73));
    selected_grad.setSpread(QLinearGradient::PadSpread);

    /* painter setUp */
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);

    /* card bg and border + clipping */
    QPainterPath card_path;
    card_path.addRoundedRect(rect.x(), rect.y(), rect.width(), rect.height(), CARD_BORDER_RADIUS, CARD_BORDER_RADIUS);
    painter->setClipPath(card_path, Qt::IntersectClip);

    /* drawing card background */
    const QBrush BG = (mode == EditMode::Editable) ? QBrush(selected_grad) : QBrush(CARD_BG_NORMAL);
    painter->setBrush(BG);
    painter->drawRoundedRect(
        rect.x(),
        rect.y(),
        rect.width(),
        rect.height(),
        CARD_BORDER_RADIUS, CARD_BORDER_RADIUS
    );

    // drawing the text elements
    auto y_offset1 = this->drawPostDate(painter, rect, mode);
    auto y_offset2 = this->drawEditingDate(painter, rect, mode, y_offset1);
    this->drawPostBody(painter, rect, mode, y_offset2 + SPACING);
    painter->restore();
}

/*
 * START comment's content drawing methods
*/
int CommentsWidget::drawPostDate(QPainter *painter, const QRect &rect, EditMode mode) const
{
    auto font_color = ( mode == EditMode::Editable ) ?  SELECTED_COLOR : DEFAULT_COLOR;

    this->setBrushPenColor( painter, font_color, font_color );
    auto header_font = this->builtFont( HEADER_FONT_SIZE, "Inter", QFont::DemiBold );

    /* timestamp header */
    painter->setFont(header_font);
    painter->drawText(
        QPoint( rect.x() + SIDE_MARGINS, rect.y() + TOP_MARGIN ),
        m_comment_date
    );

    /* calculate font's height */
    int offset_y = QFontMetrics(header_font).boundingRect(m_comment_date).height() + TOP_MARGIN;
    return offset_y;
}

int CommentsWidget::drawEditingDate(QPainter *painter, const QRect &rect, EditMode mode, int y_offset) const
{
    // checks if modified date have more than 5 characters
    // to avoid checking if it is empty "" or similar
    if (m_editing_date.length() < 5) return y_offset;

    auto font_color = ( mode == EditMode::Editable ) ?  EDIT_DATE_SELECTED_COLOR : EDIT_DATE_DEFAULTL_COLOR;
    this->setBrushPenColor(painter, font_color, font_color);
    auto subHeaderFont = builtFont(SUB_HEADER_FONT_SIZE, "Inter", QFont::DemiBold);

    int offset = y_offset + 4;

    painter->setFont(subHeaderFont);
    painter->drawText(
        QPoint(rect.x() + SIDE_MARGINS, rect.y() + offset),
        m_editing_date
    );

    auto new_yOffset = QFontMetrics(subHeaderFont).boundingRect(m_editing_date).height() + offset;
    return new_yOffset;
}

int CommentsWidget::drawPostBody(QPainter* painter, const QRect &rect, EditMode mode, int y_offset) const
{
    auto font_color = ( mode == EditMode::Editable ) ?  SELECTED_COLOR : DEFAULT_COLOR;

    this->setBrushPenColor(painter, font_color, font_color);
    auto body_font = this->builtFont(BODY_FONT_SIZE, "Inter", QFont::Normal);
    painter->setFont( body_font );

    QPointF offsets_xy(rect.x() + SIDE_MARGINS, rect.y() + y_offset);
    return this->createTextLayout(
        m_comment_body,
        offsets_xy,
        300,
        painter
    );
}

int CommentsWidget::createTextLayout(const QString& text, QPointF& xy_offset, int wrapAtWidth_px, QPainter* painter) const
{
    int lineSpacing = painter->fontMetrics().lineSpacing();

    auto y{ xy_offset.y() };

    QTextLayout txtLayout(text, painter->font());
    txtLayout.beginLayout();

    while( true )
    {
        QTextLine line = txtLayout.createLine();
        if ( !line.isValid() ) break;

        line.setLineWidth( wrapAtWidth_px );
        const auto nextLineY = y + lineSpacing;

        /* draw line if painter is provided */
        if ( painter ) line.draw( painter, QPointF( xy_offset.x(), y) );

        /* Update the y offset adding the line's height  */
        y = nextLineY;
    }
    txtLayout.endLayout();

    return y;
}

int CommentsWidget::createTextLayout(const QString& text, int y_offset, QFont& font, int wrapAtWidth_px) const
{
    int lineSpacing = 23;
    auto y{ y_offset };

    QTextLayout txtLayout(text, font);
    txtLayout.beginLayout();

    while( true )
    {
        QTextLine line = txtLayout.createLine();
        if ( !line.isValid() ) break;

        line.setLineWidth( wrapAtWidth_px );
        const auto nextLineY = y + lineSpacing;

        /* Update the y offset adding the line's height  */
        y = nextLineY;
    }
    txtLayout.endLayout();

    return y;
}

QSize CommentsWidget::sizeHint() const
{
    auto h1 = QFontMetrics(
        this->builtFont(HEADER_FONT_SIZE, "Inter", QFont::DemiBold)
    ).boundingRect(m_comment_date).height();

    // calculated only if m_editing_date has a value
    int h2 = 0;
    if ( m_editing_date.length() > 5)
    {
        QFont h2_font = this->builtFont(SUB_HEADER_FONT_SIZE, "Inter", QFont::DemiBold);
        h2 = QFontMetrics( h2_font ).boundingRect(m_editing_date).height();
    }

    auto bodyFont = this->builtFont(BODY_FONT_SIZE, "Inter", QFont::Normal);
    auto offset_y = ( h2 == 0 ) ? ( TOP_MARGIN + h1 + SPACING ) : ( TOP_MARGIN + h1 + 4 + h2 + SPACING );
    int body_h = this->createTextLayout( m_comment_body, offset_y, bodyFont, 300);

    return QSize(
            (SIDE_MARGINS * 2) + 300,
            body_h + SPACING
        );
}
/*
 * END comment's content drawing
*/

// support methods for painting
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

/*
 * START datetime format
 * for editing_date and post_date
*/
QString CommentsWidget::formatDatetime( QDateTime& dt,  QString username, DateType type )
{
    QString date_txt{};

    if ( type ==  DateType::EditDate )
    {
        date_txt = "modified: ";
        date_txt += dt.toString("dd/MM/yyyy ► hh:mm");
    } else {
        date_txt = dt.toString("ddd, dd MMM yyyy @%1 ► hh:mm").arg(username);
    }

    return date_txt;
}
