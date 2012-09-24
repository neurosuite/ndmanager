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
#include "channeloffsetspage.h"

// include files for QT
#include <qwidget.h>
#include <QList>

// include files for KDE

//General C++ include files



ChannelOffsetsPage::ChannelOffsetsPage(QWidget* parent)
    : ChannelOffsetsLayout(parent),nbChannels(0),modified(false){
    /*
    for(int i = 0;i<offsetTable->rowCount();++i)
        offsetTable->setColumnStretchable(i,true);
*/
    connect(offsetTable, SIGNAL(itemChanged (QTableWidgetItem *)),this, SLOT(propertyModified()));
    connect(offsetTable, SIGNAL(itemDoubleClicked ( QTableWidgetItem *)),this, SLOT(propertyModified()));

}

ChannelOffsetsPage::~ChannelOffsetsPage(){}

void ChannelOffsetsPage::getOffsets(QMap<int,int>& offsets){
    for(int i =0; i<nbChannels;++i){
        offsets.insert(i,offsetTable->item(i,0)->text().toInt());
    }
}

void ChannelOffsetsPage::setOffsets(QMap<int,int>& offsets){
    QMap<int,int>::Iterator iterator;
    QStringList lst;
    //The iterator gives the keys sorted.
    for(iterator = offsets.begin(); iterator != offsets.end(); ++iterator){
        int channelId = iterator.key();
        lst<<(channelId,QString::fromLatin1("%1").arg(channelId));
        QTableWidgetItem *item = new QTableWidgetItem(QString::fromLatin1("%1").arg(iterator.data()));
        offsetTable->setItem(channelId,0,item);
    }
    offsetTable->setVerticalHeaderLabels(lst);
}


#include "channeloffsetspage.moc"
