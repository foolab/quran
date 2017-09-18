#!/usr/bin/python

import sys

info = []

def fix(line):
    parts = line.split('=')

    if len(parts) != 2:
        return (None, None)

    val = parts[1]
    while val.endswith('\n'):
        val = val[0:len(val) - 1]

    while val.endswith('\r'):
        val = val[0:len(val) - 1]

    return (parts[0].strip(), val.strip())

def fileInfo(file):
    inf = {}
    f = open(file, 'r')
    for line in f.readlines():
        (key, value) = fix(line)
        if key != None:
            if key == "audio.onlineUrl":
                inf[key] = value[0:value.rfind("/")]
            else:
                inf[key] = value

    return inf

for file in sys.argv[1:]:
    info.append(fileInfo(file))

out = {"audio.lastUpdate": "lastUpdate", "audio.quality": "quality", "audio.language": "language",
       "audio.country": "country", "audio.reciter.en": "reciter",
       "audio.reciter.ar": "reciterArabic", "audio.onlineUrl": "audioUrl",
       "orig_url": "downloadUrl"}
for i in info:
    print "["+i['audio.id']+"]"
    for o in out:
        print out[o] + " = " + i[o]

    print ""

