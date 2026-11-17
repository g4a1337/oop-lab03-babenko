# Лабораторна робота № 3

**Тема:** Конструктори та деструктори в C++  
**Варіант:** 1  
**Виконав:** Бабенко Дмитро Володимирович, група 204-ТН

## Запуск у Visual Studio

Відкрийте [Lab03.sln](Lab03.sln) у Visual Studio з компонентом **Розробка класичних програм мовою C++** (MSVC v143 та Windows SDK; склад компонентів — [.vsconfig](.vsconfig)). Виберіть **x64 / Debug** і запустіть консольний проєкт через **Ctrl+F5**. Докладні вказівки: [VISUAL-STUDIO.md](VISUAL-STUDIO.md).

## Завдання

Варіант 1. Клас Complex подає комплексне число парою дійсних компонентів із закритими полями. Реалізувати Init(), Read(), Display(), toString() та операції add(), sub(), mul(), div(), equ(), conj(). На основі проєкту лабораторної №2 реалізувати щонайменше три конструктори та сформувати звіт.

## Файли

- `main.cpp` — вихідний код.
- `build.ps1` — відтворювана збірка у PowerShell.
- `input.txt` — демонстраційні вхідні дані.
- `expected_output.txt` — фактичний вивід перевіреного запуску.

## Збирання

Потрібні GCC/MinGW з підтримкою C++14 та PowerShell. Перевірено GCC 16.2.0 (w64devkit). Запускайте команди в каталозі цієї лабораторної роботи. Для всіх команд `g++` і `ar` мають належати до одного комплекту MinGW.

Якщо каталог `bin` компілятора вже додано до `PATH`:

```powershell
.\build.ps1
```

Або передайте шлях до власного каталогу `bin` компілятора:

```powershell
.\build.ps1 -CompilerDirectory '..\mingw64\bin'
```

Окремі команди збірки:

```text
g++ -std=c++14 -O2 -Wall -Wextra -pedantic main.cpp -o lab03.exe
```

## Вхідні дані

Файл `input.txt` містить демонстраційний ввід:

```text
3 4
1 -2
```

## Запуск і перевірка

Для інтерактивного запуску:

```powershell
.\lab03.exe
```

Для повторення перевіреного запуску та порівняння рядків із записаним результатом:

```powershell
cmd /c ".\lab03.exe < input.txt > actual_output.txt"
if ($LASTEXITCODE -ne 0) { throw 'Program failed' }
Compare-Object (Get-Content .\expected_output.txt) (Get-Content .\actual_output.txt)
```

Порожній результат `Compare-Object` означає збіг рядків. Каталог `bin` MinGW залиште в `PATH`, щоб Windows могла знайти потрібні бібліотеки середовища виконання. Програма перевіряє результати та завершується з кодом `0` після успішного виконання.

Очікувані перевірки:

- Програма завершується з кодом 0; Arithmetic and validation self-checks: PASS.
- Для a=3+4i і b=1−2i результати: сума 4+2i, різниця 2+6i, добуток 11−2i, частка −1+2i.
- equ(a,b) дає false; conj(a) дає 3−4i.
- Ділення на 0+0i викликає domain_error; нескінченна компонента відхиляється без зміни попереднього стану.
- Default constructor: 0 + 0i; Parameter constructor і Copy constructor: 2 + 3i.
- У вкладеному блоці живі 3 об’єкти; після виходу Destructor self-check: PASS.
