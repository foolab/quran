/*
 * Copyright (c) 2011-2017 Mohammed Sameer <msameer@foolab.org>.
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

#ifndef CORE_APPLICATION_H
#define CORE_APPLICATION_H

class QGuiApplication;
class QQuickView;
class QUrl;

class CoreApplication {
public:
  CoreApplication();
  virtual ~CoreApplication();

  void init();

  bool load(const QUrl& url);

  int exec();

protected:
  void setApplication(QGuiApplication *app, const char *name);
    void setView(QQuickView *view);
  virtual void registerQmlTypes() = 0;

private:
  QGuiApplication *m_app;
  QQuickView *m_view;
};

#endif /* CORE_APPLICATION_H */
