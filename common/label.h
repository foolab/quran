#ifndef LABEL_H
#define LABEL_H

#include <QDeclarativeItem>

class QTextDocument;

class Label : public QDeclarativeItem {
  Q_OBJECT

  Q_PROPERTY(QFont font READ font WRITE setFont);
  Q_PROPERTY(QColor color READ color WRITE setColor);
  Q_PROPERTY(QString text READ text WRITE setText);
  Q_PROPERTY(bool center READ center WRITE setCenter);

public:
  Label(QDeclarativeItem *parent = 0);
  ~Label();

  void setFont(const QFont& font);
  QFont font() const;

  void setColor(const QColor& color);
  QColor color() const;

  void setText(const QString& text);
  QString text() const;

  void setCenter(bool center);
  bool center() const;

  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		     QWidget *widget = 0);

protected:
  virtual void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry);

private:
  void updateLayout();

  QTextDocument *m_doc;

  QColor m_color;
};

#endif /* LABEL_H */
