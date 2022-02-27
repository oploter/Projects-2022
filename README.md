# Projects-2022


### Скачать репозиторий
* git clone https://github.com/oploter/Projects-2022 -- скачать репозитоий в текущую папку
* git clone https://github.com/oploter/Projects-2022 destination_path_to_dir -- скачать репозиторий в путь destination_path_to_dir

### Запустить
1) g++ -c main.cpp game_cycle.cpp load_funcs.cpp
2) g++ main.o game_cycle.o load_funcs.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system


### Состояние файлов
* untracked, unmodified, staged, modified
---
Если надо закоммитить файл file.txt, необходимо, чтобы file.txt был staged.
git status -- покажет все staged файлы
git add file_name -- добавит файл в состояние staged
git add * -- добавить все файлы
git restore <file_name> -- удалить изменения после последнего коммита
git rebase commit_hash

### Возможно надо
* git log -- посмотреть историю коммитов
* git log -p -- вывести имторию коммитов с изменениями, добавленными в этом коммите
* git log -p -2 -- не более 2 записей
* git commit --ammend -- если надо что-то исправить, но сохранить не в новый коммит, а с предыдущий
* git reset HEAD <file> -- удалить файл из staged-статус

### Как загрузить на гитхаб
* git pull / git pull - обновить локальный репозиторий
* git status - проверить правильная ли ветка
* git checkout branch_name - перейти в новую ветку 
    * git checkout -b branch_name origin/remote_branch_name - если удаленной ветки не окзалось локально
* Редактировать файлы
* git add \[file_names\] -- добавить файлы в новый коммит
* git commit -m "message" -- закоммитить изменения
* git push -- добавить изменения на гитхаб