#ifndef SEARCH_H
#define SEARCH_H

#include <QObject>
#include <QVariantList>

class SearchPrivate;

class Search : public QObject {
  Q_OBJECT

public:
  Search(const QString& path, QObject *parent = 0);
  ~Search();

  Q_INVOKABLE QVariantList search(const QString& query, bool matchWholeWords);

  Q_INVOKABLE bool enable();

public slots:
  void disable();

private:
  SearchPrivate *d_ptr;
};

#endif /* SEARCH_H */
