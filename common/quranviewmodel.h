#ifndef QURAN_VIEW_MODEL_H
#define QURAN_VIEW_MODEL_H

#include <QObject>
#include <QMultiMap>

class DataProvider;
class QuranViewModelData;

class QuranViewModel : public QObject {
  Q_OBJECT

  Q_PROPERTY(int page READ page WRITE setPage);
  Q_PROPERTY(DataProvider * data READ data WRITE setData);

public:
  QuranViewModel(QObject *parent = 0);
  ~QuranViewModel();

  void setData(DataProvider *data);
  DataProvider *data() const;

  void setPage(int page);
  int page() const;

  Q_INVOKABLE QList<int> chapters();
  Q_INVOKABLE QList<int> verses(int chapter);

public slots:
  void populate();

private:
  int m_page;
  DataProvider *m_data;

  QMultiMap<int, int> m_frags;
};

#endif /* QURAN_VIEW_MODEL_H */
