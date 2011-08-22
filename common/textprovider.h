#ifndef TEXT_PROVIDER_H
#define TEXT_PROVIDER_H

#include <QList>
#include <QString>

class TextProviderPrivate;

class TextProvider {
public:
  TextProvider(int id, const QString& dataFile, const QString& indexFile);
  ~TextProvider();

  bool load();

  int id() const;

  QString text(int aya, off_t start) const;

private:
  void unload();

  TextProviderPrivate *d_ptr;
};

#endif /* TEXT_PROVIDER_H */
