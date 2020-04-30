#!/usr/bin/env zsh
echo "[+] Запустилось отслеживание файла"
while inotifywait -e close_write main.py; do clear; python3 main.py; done
