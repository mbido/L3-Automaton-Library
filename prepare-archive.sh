#!/bin/sh

FILES="Automaton.cc Automaton.h testfa.cc"
BASE_DIR="$(mktemp -d)"
FILE_DIR="automate"
ARCHIVE=automate.tar.gz

test -f "${ARCHIVE}" && rm -f "${ARCHIVE}"
mkdir "${BASE_DIR}/${FILE_DIR}"

for F in ${FILES}
do
  cp "$F" "${BASE_DIR}/${FILE_DIR}"
done

CURR_DIR="$(pwd)"
cd "${BASE_DIR}"
tar zcvf "${ARCHIVE}" "${FILE_DIR}"
mv "${ARCHIVE}" "${CURR_DIR}"
cd "${CURR_DIR}"
rm -rf "${BASE_DIR}"
