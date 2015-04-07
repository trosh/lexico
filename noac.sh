for i in "$@"
	do export OUTFILE=noac$(echo $i | iconv -f utf8 -t ascii//TRANSLIT)
	echo $OUTFILE
	iconv -f utf8 -t ascii//TRANSLIT "$i" > "$OUTFILE"
done
