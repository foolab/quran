#!/bin/sh

#set -e

EXEC="lupdate qml/*.qml"
TS=`ls translations/*.ts 2> /dev/null`

#mkdir -p translations/

if [ -z "$1" ] && [ -z "$TS" ] ; then
echo "Cannot find a file to update"
exit 1
elif [ -n "$1" ]; then
$EXEC -ts $@
elif [ -n "$TS" ]; then
$EXEC -ts $TS
fi
