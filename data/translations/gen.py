#!/usr/bin/python

import zipfile
import sys

info = []

def fix(line):
    parts = line.split('=')

    if len(parts) != 2:
        return (None, None)

    val = parts[1]
    while val.endswith('\n'):
        val = val[0:len(val) - 1]

    return (parts[0], val)

def fileInfo(file):
    inf = {}
    z = zipfile.ZipFile(file, 'r')
    f = z.open("translation.properties", 'r')
    for line in f.readlines():
        (key, value) = fix(line)
        if key != None:
            inf[key] = value

#    id language country direction file localizedName
    return inf

for file in sys.argv[1:]:
    info.append(fileInfo(file))

out = ["id", "language", "country", "direction", "file", "localizedName"]
for i in info:
    print "["+i['id']+"]"
    for o in out:
        print o + " = " + i[o]

    print ""

