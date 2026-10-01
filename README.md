# es-student

Решения заданий курса «Встраиваемые системы», опубликованных в пособии до п2.2.5.
Исходники и скрипты подготовлены для всех заданий. Проверки с логами устройства
требуют реального запуска на Raspberry Pi Pico: файлы `device-*.log` здесь не выдуманы
и появятся после прогона скриптов на компьютере с платой.

| Проект | Задания | Проверка на плате |
| --- | --- | --- |
| `112-blink` | п1.1.2–п1.1.3 | Мигание встроенного светодиода |
| `121-blink-reg` | п1.2.1 | Мигание через регистры SIO |
| `122-led-button` | п1.2.2 | Кнопка между GP15 и GND переключает светодиод |
| `123-led-button-debounce` | п1.2.3 | Десять нажатий — десять переключений |
| `131-hello-usb` | п1.3.1–п1.3.2 | Строка Hello, world! раз в секунду |
| `133-led-button-usb` | п1.3.3–п1.3.4 | Кнопка, вывод состояния и команды e/d |
| `134-led-module` | п1.3.5–п1.3.7 | Модули led/logging/device, команды e/d/v/i |
| `211-command-usb` | п2.1.1–п2.1.6 | Строковые команды, таблица и исследование памяти |
| `221-command-time` | п2.2.1–п2.2.5 | Частоты, мигание без пауз, π и профилирование |

## Сборка

Pico SDK **2.3.0**, ARM GCC, CMake и picotool **2.3.0** — версии для проверки курса.
Переменная `PICO_SDK_PATH` должна указывать на SDK. Пример из корня репозитория:

```bash
cmake -S 211-command-usb -B 211-command-usb/build -G "Unix Makefiles"
cmake --build 211-command-usb/build
```

Файл `211-command-usb/build/211_command_usb.uf2` загрузить на Pico через BOOTSEL.
Для остальных проектов команды аналогичны.

## Снятие логов

Запускайте Python там, где виден COM-порт Pico. Если WSL не видит USB-плату,
скрипты можно запустить в Windows с установленным Python.
Установить библиотеку один раз:

```bash
python -m pip install -r requirements.txt
```

Последовательно загрузить прошивки пяти проектов и после каждой выполнить
соответствующую команду из корня репозитория:

```bash
python capture-logs.py 131-hello-usb
python capture-logs.py 133-led-button-usb
python capture-logs.py 134-led-module
python capture-logs.py 211-command-usb
python capture-logs.py 221-command-time
```

Закройте терминал COM-порта перед запуском скриптов. Для п1.3.3 нажмите кнопку
шесть раз с паузами не меньше секунды; остальные скрипты отправляют команды сами.
Перед последней группой перезагрузите Pico: п2.2.1 измеряет начальные частоты.
Группа п2.2 занимает несколько минут из-за расчётов π и пауз измерения.
Отдельную проверку можно выполнить из папки проекта командой `python check-X-Y-Z.py`.

После записи логов отправить их на GitHub:

```bash
git add --all
git commit -m "Сняты логи проверки на Pico"
git push
```

В `.github/workflows` находятся штатные вызовы проверок всех 26 заданий.
Они используют существующий секрет `ES_ACCOUNT_SECRET`. Без него результаты
остаются в Actions, с ним передаются в кабинет курса.

## Проверки GitHub Actions

[![1.1.1](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-1-1.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-1-1.yml)
[![1.1.2](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-1-2.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-1-2.yml)
[![1.1.3](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-1-3.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-1-3.yml)
[![1.1.4](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-1-4.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-1-4.yml)
[![1.1.5](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-1-5.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-1-5.yml)
[![1.2.1](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-2-1.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-2-1.yml)
[![1.2.2](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-2-2.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-2-2.yml)
[![1.2.3](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-2-3.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-2-3.yml)
[![1.3.1](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-3-1.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-3-1.yml)
[![1.3.2](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-3-2.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-3-2.yml)
[![1.3.3](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-3-3.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-3-3.yml)
[![1.3.4](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-3-4.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-3-4.yml)
[![1.3.5](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-3-5.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-3-5.yml)
[![1.3.6](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-3-6.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-3-6.yml)
[![1.3.7](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-3-7.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-1-3-7.yml)
[![2.1.1](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-1-1.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-1-1.yml)
[![2.1.2](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-1-2.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-1-2.yml)
[![2.1.3](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-1-3.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-1-3.yml)
[![2.1.4](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-1-4.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-1-4.yml)
[![2.1.5](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-1-5.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-1-5.yml)
[![2.1.6](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-1-6.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-1-6.yml)
[![2.2.1](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-2-1.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-2-1.yml)
[![2.2.2](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-2-2.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-2-2.yml)
[![2.2.3](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-2-3.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-2-3.yml)
[![2.2.4](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-2-4.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-2-4.yml)
[![2.2.5](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-2-5.yml/badge.svg)](https://github.com/Novikov-Ilia/es-student/actions/workflows/check-2-2-5.yml)
