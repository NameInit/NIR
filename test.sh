#!/bin/bash

DIRECTORY=""

if [ $1 -eq 1 ]; then
	DIRECTORY="tests/logic_text"
elif [ $1 -eq 2 ]; then
	DIRECTORY="tests/repeat_text/$2"
fi


if [ -d "$DIRECTORY" ]; then
	for FILE in "$DIRECTORY"/*; do
		if [ -f "$FILE" ]; then
			./a.out $DIRECTORY/$(basename "$FILE")
			diff $DIRECTORY/$(basename "$FILE") $DIRECTORY/$(basename "$FILE").dec
			
			if [ $? -ne 0 ]; then
				echo "Файлы $FILE и ${FILE}.dec различаются. Выход из цикла."
				break
			fi
			
			rm -f $DIRECTORY/*.dec $DIRECTORY/*.lz* $DIRECTORY/*.myalgs $DIRECTORY/*.optlz* $DIRECTORY/*.huffman $DIRECTORY/*.deflate
		fi
	done
else
	echo "Папка '$DIRECTORY' не найдена."
fi
