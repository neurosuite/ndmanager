/***************************************************************************
 *   Copyright (C) 2004 by Lynn Hazan                                      *
 *   lynn.hazan@myrealbox.com                                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
//include files for the application
#include "filepage.h"

// include files for QT
#include <qpushbutton.h>
//Added by qt3to4:
#include <QEvent>
#include <QVector>
#include <QList>

FilePage::FilePage(QWidget *parent)
    : FileLayout(parent),isIncorrectRow(false),modified(false),isInit(true),extension(""){

    mappingTable->setColumnStretchable(0,true);

    connect(extensionLineEdit,SIGNAL(returnPressed()),this,SLOT(changeCaption()));
    connect(extensionLineEdit,SIGNAL(lostFocus()),this,SLOT(changeCaption()));

    connect(addChannelButton,SIGNAL(clicked()),this,SLOT(addChannel()));
    connect(removeChannelButton,SIGNAL(clicked()),this,SLOT(removeChannel()));

    connect(extensionLineEdit,SIGNAL(textChanged(const QString&)),this,SLOT(propertyModified()));
    connect(samplingRateLineEdit,SIGNAL(textChanged(const QString&)),this,SLOT(propertyModified()));

    //install a filter on the mappingTable in order to validate the entries
    mappingTable->installEventFilter(this);

    //For validation
    connect(mappingTable, SIGNAL(valueChanged(int,int)),this, SLOT(mappingChanged(int,int)));
    connect(mappingTable, SIGNAL(pressed(int,int,int,const QPoint&)),this, SLOT(slotValidate()));
    connect(mappingTable, SIGNAL(clicked(int,int,int,const QPoint&)),this, SLOT(slotValidate()));
    connect(mappingTable, SIGNAL(doubleClicked(int,int,int,const QPoint&)),this, SLOT(slotValidate()));

}


FilePage::~FilePage(){
    extensionLineEdit->clearFocus();
    samplingRateLineEdit->clearFocus();
}

bool FilePage::eventFilter(QObject* object,QEvent* event){
    QString name = object->name();
    if(name.indexOf("mappingTable") != -1 && isIncorrectRow){
        mappingTable->editCell(incorrectRow,0);
        return true;
    }
    else if(name.indexOf("mappingTable") != -1 && event->type() == QEvent::Leave){
        if(mappingTable->currentRow() != -1){
            int row = mappingTable->currentRow();
            int column = mappingTable->currentColumn();
            QWidget* widget = mappingTable->cellWidget(row,column);
            if(widget != 0 && widget->metaObject()->className() == ("QLineEdit")){
                Q3TableItem* item = mappingTable->item(row,column);
                item->setContentFromEditor(widget);
                return true;
            }
            else return QWidget::eventFilter(object,event);
        }
        else return QWidget::eventFilter(object,event);
    }
    else return QWidget::eventFilter(object,event);
}

void FilePage::addChannel(){
    if(isIncorrectRow) return;
    modified = true;
    mappingTable->insertRows(mappingTable->numRows());
    //Use of the the 3 parameter constructor to be qt 3.1 compatible
    Q3TableItem* item = new Q3TableItem(mappingTable,Q3TableItem::WhenCurrent,"");
    item->setWordWrap(true);
    int rowId = mappingTable->numRows() - 1;
    mappingTable->setItem(rowId,0,item);
    mappingTable->verticalHeader()->setLabel(rowId,QString::fromLatin1("%1").arg(rowId));
}

void FilePage::removeChannel(){
    if(isIncorrectRow) return;
    modified = true;
    int nbSelections = mappingTable->numSelections();
    if(nbSelections > 0){
        QList< QVector<int> > rowsToRemove;
        //Look up the rows to be removed
        for(int j = 0; j < nbSelections;++j){
            Q3TableSelection selection = mappingTable->selection(j);
            bool active = selection.isActive();
            if(active){
                int nbRows = selection.bottomRow() - selection.topRow() + 1;
                QVector<int> rows(nbRows);
                for(int i = 0; i < nbRows;++i){
                    rows[i] = selection.topRow() + i;
                }
                rowsToRemove.append(rows);
            }
        }
        //Actually remove the rows
        QList< QVector<int> >::iterator iterator;
        for(iterator = rowsToRemove.begin(); iterator != rowsToRemove.end(); ++iterator) mappingTable->removeRows(*iterator);
    }
}

void FilePage::setChannelMapping(const QMap<int, QList<int> >& channels){
    for(int i =0; i<mappingTable->numRows();++i) mappingTable->removeRow(i);
    mappingTable->setNumRows(channels.count());

    QMap<int,QList<int> >::const_iterator iterator;
    //The iterator gives the keys sorted.
    for(iterator = channels.begin(); iterator != channels.end(); ++iterator){
        QList<int> channelIds = iterator.data();
        QList<int>::iterator channelIterator;

        //create the string containing the channel ids
        QString newChannel;
        for(channelIterator = channelIds.begin(); channelIterator != channelIds.end(); ++channelIterator){
            newChannel.append(QString::fromLatin1("%1").arg(*channelIterator));
            newChannel.append(" ");
        }

        Q3TableItem* item = new Q3TableItem(mappingTable,Q3TableItem::WhenCurrent,newChannel);
        item->setWordWrap(true);
        mappingTable->setItem(iterator.key() - 1,0,item);

        mappingTable->adjustColumn(iterator.key() - 1);
    }//end of groups loop
}

QMap<int, QList<int> > FilePage::getChannelMapping()const{
    QMap<int, QList<int> > channelMapping;

    int channelId = 1;
    for(int i =0; i<mappingTable->numRows();++i){
        QList<int> channels;
        QString item = mappingTable->text(i,0);
        QString channelList = item.simplified();
        if(channelList == " " || channelList.isEmpty())
            continue;

        QStringList channelParts = channelList.split(" ", QString::SkipEmptyParts);
        for(uint j = 0;j < channelParts.count(); ++j)
            channels.append(channelParts[j].toInt());
        channelMapping.insert(channelId,channels);
        channelId++;
    }

    return channelMapping;
}

void FilePage::slotValidate(){
    modified = true;
    if(isIncorrectRow){
        mappingTable->selectRow(incorrectRow);
        mappingTable->editCell(incorrectRow,0);
    }
}

/**Validates the current entry in the mapping table.*/
void FilePage::mappingChanged(int row,int column){
    modified = true;
    QString channel = mappingTable->text(row,column);
    //the group entry should only contain digits and whitespaces
    if(channel.contains(QRegExp("[^\\d\\s]")) != 0){
        isIncorrectRow = true;
        incorrectRow = row;
        mappingTable->selectRow(row);
    }
    else{
        if(isIncorrectRow){
            QString incorrectMapping = mappingTable->text(incorrectRow,0);
            if(incorrectMapping.contains(QRegExp("[^\\d\\s]")) != 0) return;
        }
        isIncorrectRow = false;
        mappingTable->adjustColumn(column);
    }
}


#include "filepage.moc"
