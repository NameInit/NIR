#!/bin/bash

DIRECTORY="tests"

if [ -d "$DIRECTORY" ]; then
    for FILE in "$DIRECTORY"/*; do
        if [ -f "$FILE" ]; then
            ./a.out tests/$(basename "$FILE")
            diff tests/$(basename "$FILE") tests/$(basename "$FILE").dec
            
            if [ $? -ne 0 ]; then
                echo "Файлы $FILE и ${FILE}.dec различаются. Выход из цикла."
                break
            fi
            
            rm -f tests/*.dec tests/*.lz* tests/*.huffman
        fi
    done
else
    echo "Папка '$DIRECTORY' не найдена."
fi
