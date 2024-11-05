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

#include "commenteditor.h"
#include "ui_commenteditor.h"

CommentEditor::CommentEditor(CommentsWidget* comment_wdg, QWidget *parent)
    : QWidget(parent), ui(new Ui::CommentEditor), m_comment_wgd{ comment_wdg }
{
    ui->setupUi(this);
    m_body_txt = m_comment_wgd->bodyText();
    m_datetime_str = m_comment_wgd->postedDateText();
    m_mod_datetime_str = m_comment_wgd->modifiedText();

    /*  Set the values for the widget in the UI  */
    ui->post_date_lbl->setText(m_datetime_str);
    ui->body_txte->setPlainText(m_body_txt);

    /* hide "mod_date_lbl" if it is empty */
    if ( m_mod_datetime_str.length() < 5 )
    {
        ui->mod_date_lbl->hide();
    } else {
        ui->mod_date_lbl->show();
        ui->mod_date_lbl->setText(m_mod_datetime_str);
    }

    /* install eventFilter on QPlainTextEdit to parse Ctrl+Return keys */
    ui->body_txte->installEventFilter(this);
}


CommentEditor::~CommentEditor()
{
    delete ui;
    delete m_comment_wgd;
}


QString CommentEditor::bodyText() const
{
    /*
     * If no text is provided, return the old text
     * Else return PlainTextEdit content
    */
    auto text_edit_content = ui->body_txte->toPlainText();
    if ( text_edit_content == "") return m_body_txt;

    return text_edit_content;
}

void CommentEditor::setCommentsWidget(CommentsWidget* commentWidget)
{
    if (m_comment_wgd != nullptr) delete m_comment_wgd;

    m_comment_wgd = commentWidget;

    /* Update the variables */
    m_body_txt = commentWidget->bodyText();
    m_datetime_str = commentWidget->postedDateText();
    m_mod_datetime_str = commentWidget->modifiedText();

    /* hide/show the modified_date text if given */
    if ( m_mod_datetime_str.length() < 5)
    {
        ui->mod_date_lbl->hide();
    } else {
        ui->mod_date_lbl->show();
    }
}


/* SETTERS */
void CommentEditor::setBodyText(QString& txt)
{
    m_comment_wgd->setBodyText(txt);
    m_body_txt = txt;
}

void CommentEditor::setBodyText(char* txt)
{
    QString text(txt);
    this->setBodyText(text);
}

void CommentEditor::setDatetimeStr(QString& txt)
{
    m_comment_wgd->setPostedDateText(txt);
    m_datetime_str = txt;
}

void CommentEditor::setDatetimeStr(char* txt)
{
    QString text(txt);
    this->setDatetimeStr(text);
}

void CommentEditor::setModifiedDatetimeStr(QString& txt)
{
    m_comment_wgd->setModifiedText(txt);
    m_mod_datetime_str = txt;
}

void CommentEditor::setModifiedDatetimeStr(char* txt)
{
    QString text(txt);
    this->setModifiedDatetimeStr(text);
}
/* END SETTERS */


/*
void CommentEditor::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    m_comment_wgd->paint(&painter, this->rect(), this->palette(), CommentsWidget::EditMode::Editable);
}


void CommentEditor::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Enter)
    {
        emit editingFinished();
    }
}
*/

bool CommentEditor::eventFilter(QObject *object, QEvent *event)
{
    /*
     * Install EventFilter on QPlainTextEdit.
     * When Ctrl+Return keys are pressed, emit editingFinished()
    */
    if ( object == ui->body_txte && event->type() == QEvent::KeyPress)
    {
        qDebug() << "Detected keys for: QPlainTextEdit";

        QKeyEvent* keys = static_cast<QKeyEvent*>(event);
        auto mod_key = keys->modifiers();
        auto key = keys->key();

        if (mod_key == Qt::Modifier::CTRL && key == Qt::Key_Return)
        {
            qDebug() << "Ctrl + Return keys detected\n" << "committing editor...\n\n";
            emit editingFinished();
            return true;
        }
        return false;
    }
    return false;
}


QSize CommentEditor::sizeHint() const
{
    return m_comment_wgd->sizeHint();
}

