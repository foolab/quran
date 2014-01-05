#include <QCoreApplication>
#include <QDebug>
#include <QScriptValue>
#include <QScriptEngine>
#include <QFile>
#include <QStringList>

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

class Recitation {
public:
  QString name;
  QString bitrate;
  QString subfolder;
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    qCritical() << "Invalid arguments!";
    return 1;
  }

  QCoreApplication app(argc, argv);

  QFile f(argv[1]);
  if (!f.open(QFile::ReadOnly)) {
    qCritical() << "Error opening" << argv[1] << f.errorString();
    return 1;
  }

  QString data = f.readAll();
  if (f.error() != QFile::NoError) {
    qCritical() << "Error reading" << argv[1] << f.errorString();
    return 1;
  }

  f.close();

  QScriptEngine e;
  QScriptValue json = e.evaluate("JSON.parse").call(QScriptValue(), QScriptValueList() << data);

  QVariantMap map = json.toVariant().toMap();
  if (map.remove("ayahCount") != 1) {
    qCritical() << "Invalid input";
    return 1;
  }

  puts("#ifndef RECITE_META_H");
  puts("#define RECITE_META_H");
  puts("");

  puts("// Automatically generated file. Do not edit!");
  puts("// Command:");
  printf("// %s\n", app.arguments().join(" ").toLatin1().data());
  puts("");

  QMap<int, Recitation> recitations;

  foreach (const QString& key, map.keys()) {
    Recitation r;
    int k = key.toInt();
    if (k == 0) {
      qCritical() << "Got zero key!";
      return 1;
    }

    QVariantMap values = map[key].toMap();
    r.bitrate = values["bitrate"].toString();
    r.name = values["name"].toString();
    r.subfolder = values["subfolder"].toString();

    if (recitations.contains(k)) {
      qCritical() << "Duplicate key!";
      return 1;
    }

    recitations[k] = r;
  }

  printf("#define RECITATIONS_LEN      %i\n\n", recitations.size());
  puts("struct _Recitation {");
  puts("  const char *name;");
  puts("  const char *subfolder;");
  puts("  const char *bitrate;");
  puts("} Rs[] = {");

  foreach (Recitation r, recitations.values()) {
    printf("{\"%s\", \"%s\", \"%s\"},\n", encode(r.name).toLatin1().data(), encode(r.subfolder).toLatin1().data(), encode(r.bitrate).toLatin1().data());
  }

  puts("};");

  printf("\n");

  puts("#endif /* RECITE_META_H */");

  return 0;
}
