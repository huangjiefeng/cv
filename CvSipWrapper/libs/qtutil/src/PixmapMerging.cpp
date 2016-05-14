/*
 * WengoPhone, a voice over Internet phone
 * Copyright (C) 2004-2006  Wengo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <qtutil/PixmapMerging.h>

#include <QtGui/QtGui>

QPixmap PixmapMerging::merge(const std::string & foregroundPixmapData, const std::string & backgroundPixmapFilename) {
	QImage foregroundImage;
	foregroundImage.loadFromData((uchar *) foregroundPixmapData.c_str(), foregroundPixmapData.size());

	QPixmap backgroundPixmap = QPixmap(QString::fromStdString(backgroundPixmapFilename));

	if (!foregroundImage.isNull()) {
		QPainter painter(&backgroundPixmap);
		painter.drawImage(0, 0,
				foregroundImage.scaled(backgroundPixmap.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
		painter.end();
	}

	return backgroundPixmap;
}
