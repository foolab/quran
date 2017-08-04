#!/usr/bin/python

import zipfile
import sys
import requests
import pyquery

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

def get_info(files):
    info = []
    for file in files:
        info.append(fileInfo(file))
    return info

def write_output(info):
    out = ["id", "language", "country", "direction", "file", "localizedName"]

    output = []
    for i in info:
        output.append("["+i['id']+"]")
        for o in out:
            output.append(o + " = " + i[o])
        output.append("")

    open("trans.ini", "w").write("\n".join(output))

def is_valid(a):
    return "href" in a.attrib \
           and "title" in a.attrib \
           and a.attrib["title"] == "Download" \
           and "class" in a.attrib \
           and a.attrib["class"] == "icon download"

def get_filename(header):
    # This is a very simple parser
    i = header.find("filename=")
    file = header[i:].split('=')[1]
    i = file.find(".zip")
    file = file[:i+4]
    return file

def download(links):
    files = []
    for link in links:
        r = requests.get(link + "&type=zekr")
        file = get_filename(r.headers["Content-Disposition"])
        open(file, "w").write(r.content)
        print file
        files.append(file)
    return files

def main():
    files = []
    if len(sys.argv) > 1:
        files = sys.argv[1:]
    else:
        r = requests.get("http://tanzil.net/trans/")
        if r.status_code != 200:
            sys.exit('Failed to download index: %s' % r.reason)
        p = pyquery.PyQuery(r.text)
        links = []
        links.extend([a.attrib["href"] for a in p('a') if is_valid(a)])
        files = download(links)

    info = get_info(files)
    write_output(info)

if __name__ == '__main__':
    main()
