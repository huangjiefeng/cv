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

#include <util/OWPicture.h>

#include <util/File.h>

using namespace std;

OWPicture::OWPicture() {
}

OWPicture::OWPicture(const OWPicture & picture) {
	copy(picture);
}

OWPicture & OWPicture::operator=(const OWPicture & picture) {
	copy(picture);

	return *this;
}

void OWPicture::copy(const OWPicture & picture) {
	_pictureData = picture._pictureData;
	_filename = picture._filename;
}

OWPicture OWPicture::pictureFromData(const std::string & data) {
	OWPicture result;

	result._pictureData = data;

	return result;
}

OWPicture OWPicture::pictureFromFile(const std::string & filename) {
	string data;
	FileReader file(filename);
	if (file.open()) {
		data = file.read();
		file.close();
	}

	OWPicture result = pictureFromData(data);
	result.setFilename(filename);

	return result;
}

string OWPicture::getData() const {
	return _pictureData;
}

string OWPicture::getFilename() const {
	return _filename;
}

void OWPicture::setFilename(const string & filename) {
	string path = filename;
	path = File::convertPathSeparators(path);
	string::size_type pos = path.rfind(File::getPathSeparator());

	_filename = path.substr(pos + 1);
}
