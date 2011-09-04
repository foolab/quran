#ifndef SEARCH_H
#define SEARCH_H

#include <QObject>
#include <QVariantList>

class SearchPrivate;

class Search : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY changed);

public:
  Search(const QString& path, QObject *parent = 0);
  ~Search();

  bool isEnabled() const;
  void setEnabled(bool enabled);

  Q_INVOKABLE QVariantList search(const QString& query);

signals:
  void changed();
  void errorOpen();
  void error();

private:
  SearchPrivate *d_ptr;
};

#endif /* SEARCH_H */
