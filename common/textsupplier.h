#ifndef TEXT_SUPPLIER_H
#define TEXT_SUPPLIER_H

#include <QObject>

class DataProvider;

class TextSupplier : public QObject {
  Q_OBJECT
  Q_PROPERTY(DataProvider * data READ data WRITE setData NOTIFY dataChanged);
  Q_PROPERTY(int chapter READ chapter WRITE setChapter NOTIFY chapterChanged);
  Q_PROPERTY(int verse READ verse WRITE setVerse NOTIFY verseChanged);
  Q_PROPERTY(QString primaryText READ primaryText NOTIFY primaryTextChanged);
  Q_PROPERTY(QString secondaryText READ secondaryText NOTIFY secondaryTextChanged);

public:
  TextSupplier(QObject *parent = 0);
  ~TextSupplier();

  void setData(DataProvider *data);
  DataProvider *data() const;

  int chapter() const;
  void setChapter(int chapter);

  int verse() const;
  void setVerse(int verse);

  QString primaryText() const;
  QString secondaryText() const;

signals:
  void dataChanged();
  void verseChanged();
  void chapterChanged();
  void primaryTextChanged();
  void secondaryTextChanged();

private slots:
  void recheck();
  void recheckPrimary();
  void recheckSecondary();

private:
  DataProvider *m_data;
  int m_chapter;
  int m_verse;
};

#endif /* TEXT_SUPPLIER_H */
