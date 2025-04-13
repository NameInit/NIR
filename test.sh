#!/bin/bash

DIRECTORY="tests"

if [ -d "$DIRECTORY" ]; then
    for FILE in "$DIRECTORY"/*; do
        if [ -f "$FILE" ]; then
            ./a.out tests/$(basename "$FILE")
            diff tests/$(basename "$FILE") tests/$(basename "$FILE").dec
            
            # Проверяем статус команды diff
            if [ $? -ne 0 ]; then
                echo "Файлы $FILE и ${FILE}.dec различаются. Выход из цикла."
                break  # Выходим из цикла при обнаружении различий
            fi
            
            rm tests/$(basename "$FILE").dec tests/$(basename "$FILE").huff
        fi
    done
else
    echo "Папка '$DIRECTORY' не найдена."
fi
