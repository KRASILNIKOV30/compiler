@echo off
setlocal EnableDelayedExpansion

:: Получаем текущую директорию
set "base=%cd%"

:: Если program.txt существует, удаляем его
if exist program.txt del program.txt

:: Рекурсивно перебираем все файлы
for /R %%F in (*) do (
    set "full=%%F"
    :: Получаем относительный путь: удаляем из полного пути базовую директорию и обратный слеш
    set "rel=!full:%base%\=!"
    echo ^<^<^<FILE^>^>^>!rel!>> program.txt
    type "%%F" >> program.txt
    echo ^<^<^<END^>^>^> >> program.txt
)

echo Архив проекта создан в файле program.txt.
pause
