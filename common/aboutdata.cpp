/*
 * Copyright (c) 2011-2012 Mohammed Sameer <msameer@foolab.org>.
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

#include "aboutdata.h"
#include <QStringList>

AboutData::AboutData(QObject *parent) : QObject(parent) {

}

AboutData::~AboutData() {

}

QString AboutData::copyright() const {
  return tr("Copyright &copy; 2011-2012 <a href=\"mailto:msameer@foolab.org\">Mohammed Sameer</a>.");
}

QString AboutData::appLicense() const {
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

QString AboutData::textLicense() const {
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

QString AboutData::transLicense() const {
  return tr("The translations provided are for non-commercial purposes only. If used otherwise, you need to obtain necessary permission from the translator or the publisher.<br />"
	    "Translations are being downloaded from <a href=\"http://tanzil.net/trans/\">http://tanzil.net/trans/</a>");
}

QString AboutData::contributors() const {
  QStringList list;

  list << tr("<a href=\"mailto:ayman@aymansmail.co.cc\">Ayman Siddiqui</a>: The default theme.");
  list << tr("<a href=\"mailto:khaledhosny@eglug.org\">Khaled Hosny</a>: Great help with the font and text.");
  list << tr("<a href=\"mailto:cloud596@hotmail.com\">Alfiansyah Rahman</a>: Creating the great logo.");

  return list.join("<br />");
}

QString AboutData::platform() const {
#ifdef Q_WS_MAEMO_5
  return "Maemo5";
#else
  return "Harmattan";
#endif
}
