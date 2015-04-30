#!/bin/bash

for i in "$@"
	do iconv -f utf8 -t ascii//TRANSLIT "$i" > "noac$i"
done
