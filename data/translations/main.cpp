#include <QCoreApplication>
#include <QSettings>
#include <QDebug>
#include <QStringList>
#include <QLocale>

QString encode(const QString& in) {
  QByteArray a(in.toUtf8());
  const char *data = a.constData();
  QString out;
  QTextStream s(&out);

  while (*data) {
    s << "\\x" << QByteArray(1, *data).toHex();
    ++data;
  }

  return out;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    qCritical() << "Invalid arguments!";
    return 1;
  }

  QCoreApplication app(argc, argv);

  QSettings s(argv[1], QSettings::IniFormat);

  QStringList groups = s.childGroups();

  puts("#ifndef TRANS_META_H");
  puts("#define TRANS_META_H");
  puts("");

  puts("// Automatically generated file. Do not edit!");
  puts("// Command:");
  printf("// %s\n", app.arguments().join(" ").toLatin1().data());
  puts("");

  puts("struct _Translation {");
  puts("  const char *id;");
  puts("  const char *name;");
  puts("  const char *language;");
  puts("  const char *file;");
  puts("  int rtl;");
  puts("} Translations[] = {");


  foreach (const QString& group, groups) {
    s.beginGroup(group);

    QString id = s.value("id").toString();
    QString name = s.value("localizedName").toString();
    QString language = QLocale::languageToString(QLocale(s.value("language").toString()).language());
    QString file = s.value("file").toString();
    int rtl = s.value("rtl").toString() == "rtl" ? 1 : 0;
    if (id.isEmpty() || name.isEmpty() || language.isEmpty() || file.isEmpty()) {
      qFatal("Missing meta data");
    }

    printf("{\"%s\", \"%s\", \"%s\", \"%s\", %i},\n", encode(id).toLatin1().data(),
	   encode(name).toLatin1().data(), encode(language).toLatin1().data(),
	   encode(file).toLatin1().data(), rtl);

    s.endGroup();
  }

  puts("};");

  puts("");
  puts("#endif /* TRANS_META_H */");
}
