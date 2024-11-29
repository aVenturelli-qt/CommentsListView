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

#include "commentlistitem.h"
#include "commenteditor.h"

CommentListItem::CommentListItem() : m_username{} {}

CommentListItem::CommentListItem(QString& username) : m_username{ username } {}


void CommentListItem::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if ( index.siblingAtColumn(2).data().canConvert<QString>() )
    {
        CommentsWidget* cmm_wdg = this->createCommentWidgetFromIndex(index, QList<int>{3, 4, 5});

        /*  transparent background   */
        painter->fillRect( option.rect, Qt::transparent );

        auto state = CommentsWidget::EditMode::ReadOnly;
        /* highlight the background when item selected */
        if ( option.state & QStyle::State_Selected ) state = CommentsWidget::EditMode::Editable;

        /* delegate the painting to CommentsWidget */
        cmm_wdg->paint( painter, option.rect, option.palette, state );

        // is very stupid allocating memory and after that deleting it
        // but will be optimize in the future (if possible)
        // WARNING boost_scoped_ptr is a much safer solution.
        delete cmm_wdg;
    }
}


QWidget* CommentListItem::createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    Q_UNUSED(option)

    if ( index.siblingAtColumn(0).data().canConvert<int>() )
    {
        if ( m_username != index.siblingAtColumn(2).data().toString())
        {
            return nullptr;
        }
        CommentEditor* editor = new CommentEditor(parent);
        connect( editor, &CommentEditor::editingFinished, this, &CommentListItem::commitAndCloseEditor );

        return editor;
    }

    return nullptr;
}


void CommentListItem::setEditorData( QWidget* editor, const QModelIndex &index ) const
{
    if ( index.siblingAtColumn(0).data().canConvert<int>() )
    {
        CommentsWidget* cmm_wdg = this->createCommentWidgetFromIndex(index, QList<int>{3, 4, 5});

        CommentEditor* my_editor = qobject_cast<CommentEditor*>( editor );
        my_editor->setCommentsWidget( cmm_wdg );
    }
}



void CommentListItem::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{

    auto cast_model = static_cast<QSqlRelationalTableModel*>(model);

    if ( index.siblingAtColumn(0).data().canConvert<int>() )
    {
        CommentEditor* cm_editor = qobject_cast<CommentEditor*>(editor);
        auto cmm_wdg = cm_editor->commentWidget();
        auto body_txt = cmm_wdg.bodyText();

        // check if text was changed from the original and is not empty
        if ( cm_editor->bodyText() != body_txt && !cm_editor->bodyText().isEmpty())
        {
            /* generate the edited_mess_date */
            QDateTime time_now = QDateTime::currentDateTime();

            /* update the model */
            cast_model->setData(
                index.siblingAtColumn(4),
                QVariant::fromValue( time_now )
            );

            cast_model->setData(
                index.siblingAtColumn(5),
                QVariant::fromValue( cm_editor->bodyText() )
            );

            cast_model->submitAll();
        }
    }
}


QSize CommentListItem::sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    Q_UNUSED(option)

    if ( index.siblingAtColumn(2).data().canConvert<QString>() )
    {
        auto cmm_wdg = this->createCommentWidgetFromIndex( index, QList<int>{3, 4, 5} );
        QSize cmm_wdg_sizeHint = cmm_wdg->sizeHint();
        delete cmm_wdg; // free up dinamically allocated memory

        return cmm_wdg_sizeHint;
    }

    return QSize();
}


/* WARNING HERE */
CommentsWidget* CommentListItem::createCommentWidgetFromIndex( const QModelIndex& idx, QList<int> data_at_columns ) const
{

    // retrive the username from column 2 (first column starts from 0)
    auto userName = qvariant_cast<QString>(
        idx.siblingAtColumn( 2 ).data()                 // example: user123name@gmail.com
        );
    auto at_pos = userName.indexOf('@');
    auto userName_formatted = userName.chopped(userName.length() - at_pos);     // example: user123name

    // post's date
    auto post_date = qvariant_cast<QDateTime>(
        idx.siblingAtColumn( data_at_columns[0] ).data()
    );
    auto post_date_str = CommentsWidget::formatDatetime( post_date, userName_formatted, CommentsWidget::DateType::PostDate );

    // editing comment date
    auto editing_date = qvariant_cast<QDateTime>(
        idx.siblingAtColumn( data_at_columns[1] ).data()
    );

    QString editing_date_str{};
    if ( editing_date.isValid() )
    {
        editing_date_str = CommentsWidget::formatDatetime( editing_date );
    }

    /* body comment */
    auto body =  qvariant_cast<QString>(
        idx.siblingAtColumn( data_at_columns[2] ).data()
    );

    return new CommentsWidget{post_date_str, editing_date_str, body};
}


void CommentListItem::commitAndCloseEditor()
{
    CommentEditor* editor = qobject_cast<CommentEditor*>( sender() );
    emit commitData( editor );
    emit closeEditor( editor );
}

