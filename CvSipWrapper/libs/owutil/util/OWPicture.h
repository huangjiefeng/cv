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

#ifndef OWPICTURE_H
#define OWPICTURE_H

#include <util/owutildll.h>

#include <string>

/**
 * Represents a picture.
 *
 * Named OWPicture rather than Picture because of a conflict under MacOSX.
 *
 * TODO
 * - Detect the picture format from header.
 * - Return the format of the loaded picture and its size
 *
 * @author Philippe Bernery
 */
class OWUTIL_API OWPicture {
public:

	/**
	 * Constructs an empty OWPicture.
	 */
	 OWPicture();

	 OWPicture(const OWPicture & picture);

	 OWPicture & operator=(const OWPicture & picture);

	/**
	 * Constructs a OWPicture from data in memory.
	 *
	 * These data must include the header available in the file from where
	 * it has been extracted. The construtor will then guess the picture format
	 * from this header.
	 *
	 * @param data OWPicture data
	 */
	 static OWPicture pictureFromData(const std::string & data);

	/**
	 * Constructs a OWPicture from a file.
	 *
	 * The data are loaded and copied in memory.
	 *
	 * @param data OWPicture data
	 */
	 static OWPicture pictureFromFile(const std::string & filename);

	/**
	 * Gets the data with format header.
	 *
	 * @return the data
	 */
	 std::string getData() const;

	/**
	 * Gets the name of the file from which the OWPicture has been created.
	 *
	 * This does not return the complete path of the file.
	 *
	 * @return the filename
	 */
	 std::string getFilename() const;

	/**
	 * Set the filename of the OWPicture.
	 *
	 * Only the last part of the past is saved.
	 */
	 void setFilename(const std::string & filename);

private:

	void copy(const OWPicture & picture);

	std::string _pictureData;

	std::string _filename;
};

#endif	//OWPICTURE_H
