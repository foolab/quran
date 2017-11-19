#ifndef MODELS_H
#define MODELS_H

#include <QSortFilterProxyModel>
#include <QAbstractListModel>
#include "translation.h"

class Translations;

class InstalledFilterModel : public QSortFilterProxyModel {
  Q_OBJECT

public:
  InstalledFilterModel(QObject *parent = 0);
  ~InstalledFilterModel();

public slots:
  void init(QAbstractItemModel *model, const QString& installedPropertyName,
	    int installedPropertyValue, const QString& role);

protected:
  virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;

private:
  QString m_name;
  int m_value;
  int m_role;
};

class VisibilityFilterModel : public QSortFilterProxyModel {
  Q_OBJECT
  Q_PROPERTY(QAbstractItemModel *model READ model WRITE setModel NOTIFY modelChanged);

public:
  VisibilityFilterModel(QObject *parent = 0);
  ~VisibilityFilterModel();

  QAbstractItemModel *model();
  void setModel(QAbstractItemModel *model);

signals:
  void modelChanged();

protected:
  virtual bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const;
};

class TranslationsModel : public QAbstractListModel {
  Q_OBJECT
  Q_PROPERTY(Translations *source READ source WRITE setSource NOTIFY sourceChanged);

public:
  enum {
    TranslationRole = Qt::UserRole,
    SectionRole = Qt::UserRole + 1,
    VisibleRole = Qt::UserRole + 2,
  };

  TranslationsModel(QObject *parent = 0);
  ~TranslationsModel();

  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

  Translations *source() const;
  void setSource(Translations *translations);

  QHash<int, QByteArray> roleNames() const;

signals:
  void sourceChanged();

private slots:
  void refresh();
  void translationChanged(Translation *t);

private:
  Translations *m_translations;
};

#endif /* MODELS_H */
