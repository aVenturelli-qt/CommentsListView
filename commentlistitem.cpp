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


CommentListItem::CommentListItem() { }


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
        auto state = CommentsWidget::EditMode::ReadOnly;

        /* main content */
        auto body_txt = qvariant_cast<QString>( index.siblingAtColumn(4).data() );

        /* comment date */
        auto  post_dt = qvariant_cast<QDateTime>( index.siblingAtColumn(1).data() );
        auto post_str = post_dt.toString("ddd, MMM dd HH:mm");                                              // \u25BA

        /* modified comment date */
        auto  modified_dt = qvariant_cast<QDateTime>( index.siblingAtColumn(5).data() );
        auto modified_str = ( modified_dt.isValid() ) ? modified_dt.toString("ddd, MMM dd HH:mm" ) : QString("");

        /* Comment widget construction */
        CommentsWidget cmm_wdg = CommentsWidget( body_txt, post_str, modified_str );

        /*  transparent background   */
        painter->fillRect( option.rect, Qt::transparent );

        if ( option.state & QStyle::State_Selected )
        {
            state = CommentsWidget::EditMode::Editable;
            // qDebug() << index.flags();
        }

        cmm_wdg.paint( painter, option.rect, option.palette, state );
    }
}


QWidget* CommentListItem::createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const
{
    qDebug() << index.siblingAtColumn(2).data().toString();

    if ( index.siblingAtColumn(2).data().canConvert<QString>() )
    {
        qDebug() << "[CommentListItem::createEditor] << correct call";

        /* main content */
        auto body_txt = qvariant_cast<QString>( index.siblingAtColumn(4).data() );

        /* comment date */
        auto  post_dt = qvariant_cast<QDateTime>( index.siblingAtColumn(1).data() );
        auto post_str = post_dt.toString("ddd, MMM dd HH:mm");                                              // \u25BA

        /* modified comment date */
        auto  modified_dt = qvariant_cast<QDateTime>( index.siblingAtColumn(5).data() );
        auto modified_str = ( modified_dt.isValid() ) ? modified_dt.toString("ddd, MMM dd HH:mm") : QString("");

        /* Comment widget construction */
        CommentsWidget* cmm_wdg = new CommentsWidget(body_txt, post_str, modified_str);

        CommentEditor* editor = new CommentEditor(cmm_wdg, parent);
        connect( editor, &CommentEditor::editingFinished, this, &CommentListItem::commitAndCloseEditor );

        return editor;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}


void CommentListItem::commitAndCloseEditor()
{
    CommentEditor* editor = qobject_cast<CommentEditor*>( sender() );
    emit commitData( editor );
    emit closeEditor( editor );
}


void CommentListItem::setEditorData( QWidget* editor, const QModelIndex &index ) const
{
    if ( index.siblingAtColumn(2).data().canConvert<QString>() )
    {
        auto body_txt = qvariant_cast<QString>( index.siblingAtColumn(4).data() );

        auto  post_dt = qvariant_cast<QDateTime>( index.siblingAtColumn(1).data() );
        auto post_str = post_dt.toString("ddd, MMM dd HH:mm");                                              // \u25BA

        auto  modified_dt = qvariant_cast<QDateTime>( index.siblingAtColumn(5).data() );
        auto modified_str = ( modified_dt.isValid() ) ? modified_dt.toString("ddd, MMM dd HH:mm") : QString("");

        CommentsWidget* cmm_wdg = new CommentsWidget(body_txt, post_str, modified_str);

        CommentEditor* my_editor = qobject_cast<CommentEditor*>( editor );
        my_editor->setCommentsWidget( cmm_wdg );
    }
}


void CommentListItem::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
    if ( index.siblingAtColumn(2).data().canConvert<QString>() )
    {
        qDebug() << "\n\t[::setModelData()] being called..";

        CommentEditor* cm_editor = qobject_cast<CommentEditor*>(editor);
        auto cmm_wdg = cm_editor->commentWidget();
        auto body_txt = cmm_wdg.bodyText();

        if ( cm_editor->bodyText() != body_txt )
        {
            qDebug() << "\t[::setModelData()--> body_text different detected] inside if call..";

            /* generate the edited_mess_date */
            QDateTime time_now = QDateTime::currentDateTime();
            auto edited_mess_date = QString("Last Modification: ");
            edited_mess_date += time_now.toString("ddd, MMM dd HH:mm");

            qDebug() << "\t - 'edited_mess_date' = " << edited_mess_date;

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
    if ( index.siblingAtColumn(2).data().canConvert<QString>() )
    {
        auto body_txt = qvariant_cast<QString>( index.siblingAtColumn(4).data() );

        auto  post_dt = qvariant_cast<QDateTime>( index.siblingAtColumn(1).data() );
        auto post_str = post_dt.toString("ddd, MMM dd HH:mm");                                              // \u25BA

        auto  modified_dt = qvariant_cast<QDateTime>( index.siblingAtColumn(5).data() );
        auto modified_str = ( modified_dt.isValid() ) ? modified_dt.toString("ddd, MMM dd HH:mm") : QString("");

        CommentsWidget cmm_wdg = CommentsWidget(body_txt, post_str, modified_str);

        return cmm_wdg.sizeHint();
    } else {
        return QStyledItemDelegate::sizeHint(option, index);
    }
}
