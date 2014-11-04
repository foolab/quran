#include "models.h"

InstalledFilterModel::InstalledFilterModel(QObject *parent) :
  QSortFilterProxyModel(parent),
  m_value(-1),
  m_role(-1) {

  setFilterKeyColumn(0);
  setDynamicSortFilter(true);
}

InstalledFilterModel::~InstalledFilterModel() {

}

void InstalledFilterModel::init(QAbstractItemModel *model, const QString& installedPropertyName,
				int installedPropertyValue, const QString& role) {
  m_name = installedPropertyName;
  m_value = installedPropertyValue;


  QHash<int, QByteArray> roles = model->roleNames();
  foreach (int r, roles.keys()) {
    if (roles[r] == role) {
      m_role = r;
      break;
    }
  }

  setSourceModel(model);
}

bool InstalledFilterModel::filterAcceptsRow(int source_row,
					    const QModelIndex& source_parent) const {
  if (!sourceModel()) {
    return false;
  }

  QObject *object =
    sourceModel()->data(sourceModel()->index(source_row, 0, source_parent),
			m_role).value<QObject *>();
  if (!object) {
    return false;
  }

  return object->property(m_name.toUtf8()).toInt() == m_value;
}
