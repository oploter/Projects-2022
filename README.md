# Projects-2022


### Скачать репозиторий
* ```git clone https://github.com/oploter/Projects-2022``` -- скачать репозитоий в текущую папку
* ```git clone https://github.com/oploter/Projects-2022 destination_path_to_dir``` -- скачать репозиторий в путь destination_path_to_dir

### Как скачать библиотеку
* Linux
    1) ```sudo apt-get install libsfml-dev```
    2) ```g++ -c ....cpp ....cpp ```
    3) ```g++ ....o ....o ....o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system```
    4) ```./sfml-app```

* Windows
    1) https://www.sfml-dev.org/download/sfml/2.5.1/ - скачать sdk под компилятор 
    2) В директорию с проектом положить разархивированную директорию(SFML-2.5.1-....)
    |
    main.cpp
    SFML-2.5.1-....
    file.cpp
    |
    3) Заменить все ```#include <SFML/Graphics.hpp> ``` на ```#include "SFML-2.5.1-..../include/SFML/Graphics.hpp"```
    4) ```g++ -c ....cpp ....cpp -ISFML-2.5.1-.../include``` -- скомпилировать своим компилятором
    5) ```g++ ....o ....o ....o -o sfml-app -LSFML-2.5.1-.../lib -lsfml-graphics -lsfml-window -lsfml-system``` -- собрать в одно
    6) ```./sfml-app``` -- запустить приложение 


### Состояние файлов
* untracked, unmodified, staged, modified
---
Если надо закоммитить файл file.txt, необходимо, чтобы file.txt был staged.
```git status``` -- покажет все staged файлы
```git add file_name``` -- добавит файл в состояние staged
```git add *``` -- добавить все файлы
```git restore <file_name>``` -- удалить изменения после последнего коммита
```git rebase commit_hash```

### Возможно надо
* ```git log``` -- посмотреть историю коммитов
* ```git log -p``` -- вывести имторию коммитов с изменениями, добавленными в этом коммите
* ```git log -p -2``` -- не более 2 записей
* ```git commit --ammend``` -- если надо что-то исправить, но сохранить не в новый коммит, а с предыдущий
* ```git reset HEAD <file>``` -- удалить файл из staged-статус

### Как загрузить на гитхаб
* ```git pull / git fetch``` - обновить локальный репозиторий
* ```git status``` - проверить правильная ли ветка
* ```git checkout branch_name``` - перейти в новую ветку 
    * ```git checkout -b branch_name origin/remote_branch_name``` - если удаленной ветки не окзалось локально
* Редактировать файлы
* ```git add [file_names]``` -- добавить файлы в новый коммит
* ```git commit -m "message"``` -- закоммитить изменения
* ```git push``` -- добавить изменения на гитхаб