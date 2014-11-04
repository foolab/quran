#ifndef MODELS_H
#define MODELS_H

#include <QSortFilterProxyModel>

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

#endif /* MODELS_H */
