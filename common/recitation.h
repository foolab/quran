#ifndef RECITATION_H
#define RECITATION_H

#include <QString>
#include <QUrl>

class Recitation {
public:
  static Recitation *create(const QString& id, const QString& dir);

  virtual ~Recitation();

  QString id() const;
  QString name() const;
  QString dir() const;

  bool isValid();

  virtual QUrl mediaUrl(int chapter, int verse) = 0;

protected:
  Recitation(const QString& name, const QString& id, const QString& dir);

private:
  const QString m_name;
  const QString m_id;
  const QString m_dir;
};

#endif /* RECITATION_H */
