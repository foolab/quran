/*
 * Copyright (c) 2011 Mohammed Sameer <msameer@foolab.org>. All rights reserved.
 *
 * This package is free software; you can redistribute it and/or modify
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
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "legal.h"

Legal::Legal(QObject *parent) : QObject(parent) {

}

Legal::~Legal() {

}

QString Legal::copyright() {
  return tr("Copyright &copy; 2011 <a href=\"mailto:msameer@foolab.org\">Mohammed Sameer</a>. All rights reserved.");
}

QString Legal::appLicense() {
  return tr("This program is free software; you can redistribute it and/or modify "
	    "it under the terms of the GNU General Public License as published by "
	    "the Free Software Foundation; either version 2 of the License, or "
	    "(at your option) any later version.<br />"
	    "<br />"
	    "This program is distributed in the hope that it will be useful, "
	    "but WITHOUT ANY WARRANTY; without even the implied warranty of "
	    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
	    "GNU General Public License for more details.<br />"
	    "<br />"
	    "You should have received a copy of the GNU General Public License "
	    "along with this program. If not, see <a href=\"http://www.gnu.org/licenses/\">http://www.gnu.org/licenses</a>.");
}

QString Legal::textLicense() {
  return tr("Tanzil Quran Text<br />"
	    "Copyright &copy; 2008-2011 Tanzil Project<br />"
	    "License: Creative Commons Attribution 3.0<br />"
	    "<br />"
	    "This copy of the Quran text is carefully produced, highly "
	    "verified and continuously monitored by a group of specialists "
	    "in Tanzil Project.<br />"
	    "<br />"
	    "TERMS OF USE:<br />"
	    "<br />"
	    "- Permission is granted to copy and distribute verbatim copies "
	    "of this text, but CHANGING IT IS NOT ALLOWED.<br />"
	    "<br />"
	    "- This Quran text can be used in any website or application, "
	    "provided that its source (Tanzil Project) is clearly indicated, "
	    "and a link is made to <a href=\"http://tanzil.net\">tanzil.net</a> to enable users to keep "
	    "track of changes.<br />"
	    "<br />"
	    "- This copyright notice shall be included in all verbatim copies "
	    "of the text, and shall be reproduced appropriately in all files "
	    "derived from or containing substantial portion of this text.<br />"
	    "<br />"
	    "Please check updates at: <a href=\"http://tanzil.net/updates/\">http://tanzil.net/updates</a>");
}

QString Legal::fontLicense() {
  return tr("Font used is me_quran. Copyright &copy; Meor Ridzuan Meor Yahaya. All rights reserved.<br />"
	    "This is an arabic font, specifically design for rendering the Quran like the Madinah Mushaf.<br />The font contains arabic glyph only.<br />"
	    "<br />"
	    "The font comes with the following restriction:<br />"
	    "- It is for private and non-commercial use only.<br />"
	    "- The Opentype and VOLT table can be use as a learning material.<br />"
	    "- Redistribution is not permitted without the consent from the author.<br />"
	    "<br />"
	    "Permission from the author has been granted for this application.");
}
