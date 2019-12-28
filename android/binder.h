/*
 * Copyright (c) 2019 Mohammed Sameer <msameer@foolab.org>.
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

#ifndef BINDER_H
#define BINDER_H

#include <QAndroidBinder>
#include <QVariant>
#include <QMap>
#include <functional>

#include <QMetaObject>
#include <QPointer>
#include <QObject>

class QAndroidParcel;

typedef std::function<QVariant(const QAndroidParcel& data)> BinderHandler;

class Binder : public QAndroidBinder {
public:
  class MethodInvoker {
  public:
    MethodInvoker(QObject *obj, const QLatin1String& method) :
      m_obj(obj),
      m_method(method) {}

    virtual ~MethodInvoker() {}

    virtual QVariant operator()(const QAndroidParcel& data) {
      Q_UNUSED(data);
      Q_ASSERT(m_obj != 0);

      return QMetaObject::invokeMethod(m_obj, m_method.data(), Qt::QueuedConnection);
    }

  protected:
    QPointer<QObject> m_obj;
    QLatin1String m_method;
  };

  // For whatever reason having those as a class template is not working.
  // We end up getting true for at least the bool instance even if the
  // invoked method returns false.
  class BoolPropertyGetter : public MethodInvoker {
  public:
    using MethodInvoker::MethodInvoker;

    virtual QVariant operator()(const QAndroidParcel& data) override {
      Q_UNUSED(data);
      Q_ASSERT(m_obj != 0);
      bool result;

      bool st = QMetaObject::invokeMethod(m_obj, m_method.data(),
					  Qt::BlockingQueuedConnection, Q_RETURN_ARG(bool, result));
      Q_ASSERT(st != false);

      return result;
    }
  };

  class UnsignedIntPropertyGetter : public MethodInvoker {
  public:
    using MethodInvoker::MethodInvoker;

    virtual QVariant operator()(const QAndroidParcel& data) override {
      Q_UNUSED(data);
      Q_ASSERT(m_obj != 0);
      uint result;

      bool st = QMetaObject::invokeMethod(m_obj, m_method.data(),
					  Qt::BlockingQueuedConnection, Q_RETURN_ARG(uint, result));
      Q_ASSERT(st != false);

      return result;
    }
  };

  Binder();
  ~Binder();

  void addHandler(int code, const BinderHandler& func);

  bool onTransact(int code, const QAndroidParcel& data, const QAndroidParcel& reply,
		  QAndroidBinder::CallType flags) override;

private:
  QMap<int, BinderHandler> m_handlers;
};

#endif /* BINDER_H */
