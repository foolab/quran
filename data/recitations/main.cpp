#include <QCoreApplication>
#include <QDebug>
#include <QSettings>
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

  puts("#ifndef RECITE_META_H");
  puts("#define RECITE_META_H");
  puts("");

  puts("// Automatically generated file. Do not edit!");
  puts("// Command:");
  printf("// %s\n", app.arguments().join(" ").toLatin1().data());
  puts("");
  puts("#include <array>");
  puts("");
  printf("#define _RECITATIONS_LEN %d\n", groups.size());
  puts("");

  puts("struct _Recitation {");
  puts("  const char *id;");
  puts("  const char *name;");
  puts("  const char *translated_name;");
  puts("  const char *quality;");
  puts("  const char *url;");
  puts("  int language;");
  puts("};");
  puts("");
  puts("static const std::array<const _Recitation, _RECITATIONS_LEN> Rs = {{");
  foreach (const QString& group, groups) {
    s.beginGroup(group);
    QString id = group;
    QString reciter = read(s, "reciter");
    QString translated_recitor = read(s, "reciterArabic");
    QString quality = read(s, "quality");
    QString url = read(s, "audioUrl");
    QString language = read(s, "language");
    QLocale locale(language);
    url = url.left(url.lastIndexOf('/') + 1);

    if (id.isEmpty() || reciter.isEmpty() || language.isEmpty() || url.isEmpty() || translated_recitor.isEmpty() || quality.isEmpty()) {
      qFatal("Missing meta data");
      return 1;
    }

    int lang = locale.language();
    if (lang == QLocale::C) {
      lang = QLocale::English;
    }

    printf("{\"%s\", \"%s\", \"%s\", \"%s\", \"%s\", %i},\n", encode(id).toLatin1().data(),
	   encode(reciter).toLatin1().data(), encode(translated_recitor).toLatin1().data(),
	   encode(quality).toLatin1().data(), encode(url).toLatin1().data(), lang);

    s.endGroup();
  }

  puts("}};");

  puts("");
  puts("#endif /* RECITE_META_H */");

  //    printf("{\"%s\", \"%s\", \"%s\"},\n", encode(r.name).toLatin1().data(), encode(r.subfolder).toLatin1().data(), encode(r.bitrate).toLatin1().data());

  return 0;
}
