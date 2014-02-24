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

QString read(QSettings& s, const char *value) {
  return QString::fromUtf8(s.value(value).toByteArray());
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    qCritical() << "Invalid arguments!";
    return 1;
  }

  QCoreApplication app(argc, argv);

  QSettings s(argv[1], QSettings::IniFormat);
  s.setIniCodec("UTF-8");

  QStringList groups = s.childGroups();

  puts("#ifndef TRANS_META_H");
  puts("#define TRANS_META_H");
  puts("");

  puts("// Automatically generated file. Do not edit!");
  puts("// Command:");
  printf("// %s\n", app.arguments().join(" ").toLatin1().data());
  puts("");

  printf("#define TRANSLATIONS_LEN %d\n", groups.size());
  puts("");

  puts("struct _Translation {");
  puts("  const char *id;");
  puts("  const char *name;");
  puts("  const char *file;");
  puts("  int language;");
  puts("  int rtl;");
  puts("} Ts[] = {");


  foreach (const QString& group, groups) {
    s.beginGroup(group);

    QString id = read(s, "id");
    QString name = read(s, "localizedName");
    QString language = read(s, "language");
    QString file = read(s, "file");
    int rtl = read(s, "rtl") == "rtl" ? 1 : 0;
    QLocale locale(read(s, "language"));

    if (id.isEmpty() || name.isEmpty() || language.isEmpty() || file.isEmpty()) {
      qFatal("Missing meta data");
      return 1;
    }

    int lang = locale.language();
    if (lang == QLocale::C) {
      lang = QLocale::English;
    }

    printf("{\"%s\", \"%s\", \"%s\", %i, %i},\n", encode(id).toLatin1().data(),
	   encode(name).toLatin1().data(), encode(file).toLatin1().data(),
	   lang, rtl);

    s.endGroup();
  }

  puts("};");

  puts("");
  puts("#endif /* TRANS_META_H */");
}
