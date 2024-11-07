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
    /*
    qDebug() << "\n\nthe model index contains:"
             << index.siblingAtColumn(0).data()         // qulonglong   ID
             << index.siblingAtColumn(1).data()         // QDatetime    Posting_Datetime
             << index.siblingAtColumn(2).data()         // QString      Chassis_ID
             << index.siblingAtColumn(3).data()         // int          User_ID
             << index.siblingAtColumn(4).data()         // QString      Comment's body
             << index.siblingAtColumn(5).data();        // QDateTime    Modified_datetime
    */

    if ( index.siblingAtColumn(2).data().canConvert<QString>() )
    {
        CommentsWidget* cmm_wdg = this->createCommentWidgetFromIndex(index, QList<int>{1, 5, 4});

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
    if ( index.siblingAtColumn(2).data().canConvert<QString>() )
    {
        CommentEditor* editor = new CommentEditor(parent);
        connect( editor, &CommentEditor::editingFinished, this, &CommentListItem::commitAndCloseEditor );

        return editor;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}


void CommentListItem::setEditorData( QWidget* editor, const QModelIndex &index ) const
{
    if ( index.siblingAtColumn(2).data().canConvert<QString>() )
    {
        CommentsWidget* cmm_wdg = this->createCommentWidgetFromIndex(index, QList<int>{1, 5, 4});

        CommentEditor* my_editor = qobject_cast<CommentEditor*>( editor );
        my_editor->setCommentsWidget( cmm_wdg );
    }
}


/* WARNING HERE */
void CommentListItem::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    /*
     * WARNING: this part do not work
     * the problem is located in the model.
     *
     * To provide editing capabilities try to implement a customModel sub-classing QAbstractModel
    */

    if ( index.siblingAtColumn(2).data().canConvert<QString>() )
    {
        CommentEditor* cm_editor = qobject_cast<CommentEditor*>(editor);
        auto cmm_wdg = cm_editor->commentWidget();
        auto body_txt = cmm_wdg.bodyText();

        if ( cm_editor->bodyText() != body_txt )
        {
            /* generate the edited_mess_date */
            QDateTime time_now = QDateTime::currentDateTime();
            auto edited_mess_date = QString("Last Modification: ");
            edited_mess_date += time_now.toString("ddd, MMM dd HH:mm");

            /* update the model */
            qDebug() << "index.siblingAtColumn(4):\t" << index.siblingAtColumn(4);
            qDebug() << "index.siblingAtColumn(5):\t" << index.siblingAtColumn(5);
            model->setData( index.siblingAtColumn(4), QVariant::fromValue( body_txt ) );
            model->setData( index.siblingAtColumn(5), QVariant::fromValue( edited_mess_date ) );

        }
    }
}


QSize CommentListItem::sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
    Q_UNUSED(option)

    if ( index.siblingAtColumn(2).data().canConvert<QString>() )
    {
        auto cmm_wdg = this->createCommentWidgetFromIndex( index, QList<int>{1, 5, 4} );
        QSize cmm_wdg_sizeHint = cmm_wdg->sizeHint();
        delete cmm_wdg; // free up dinamically allocated memory

        return cmm_wdg_sizeHint;
    }

    return QSize();
}


/* WARNING HERE */
CommentsWidget* CommentListItem::createCommentWidgetFromIndex( const QModelIndex& idx, QList<int> data_at_columns ) const
{
    /* post's date */
    auto post_date = qvariant_cast<QDateTime>(
        idx.siblingAtColumn( data_at_columns[0] ).data()
    );

    /*
     * WARNING: the second parameter of "formatDatetime()" should be retrieved
     * from the index! not from the member
     * this version provides a static one for test purpose
    */
    auto post_date_str = CommentsWidget::formatDatetime( post_date, m_username, CommentsWidget::DateType::PostDate );

    /* editing comment date */
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
