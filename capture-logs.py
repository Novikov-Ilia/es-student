"""Run the course's board checks from their project directory."""

import argparse
from pathlib import Path
import subprocess
import sys
import time

from serial.tools import list_ports


GROUPS = {
    "131-hello-usb": ["1.3.1", "1.3.2"],
    "133-led-button-usb": ["1.3.3", "1.3.4"],
    "134-led-module": ["1.3.6", "1.3.7"],
    "211-command-usb": [f"2.1.{i}" for i in range(1, 7)],
    "221-command-time": [f"2.2.{i}" for i in range(1, 6)],
}


def main():
    parser = argparse.ArgumentParser(description="Снять настоящие логи с Pico после загрузки прошивки.")
    parser.add_argument("project", choices=GROUPS)
    args = parser.parse_args()

    boards = [p for p in list_ports.comports() if p.vid == 0x2E8A and p.pid == 0x000A]
    if len(boards) != 1:
        print("Подключите одну Pico с прошивкой выбранного проекта и закройте терминал COM-порта.")
        return 1

    folder = Path(__file__).resolve().parent / args.project
    print("Проверьте, что на плате прошивка " + args.project, flush=True)
    if args.project == "133-led-button-usb":
        print("В первой проверке нажмите кнопку GP15 шесть раз с паузами не меньше секунды.", flush=True)
    for task in GROUPS[args.project]:
        suffix = task.replace(".", "-")
        started_ns = time.time_ns()
        subprocess.run([sys.executable, "check-" + suffix + ".py"], cwd=folder, check=True)
        log = folder / ("device-" + suffix + ".log")
        if not log.exists() or log.stat().st_mtime_ns < started_ns:
            print("Свежий лог не создан: " + str(log))
            return 1
        if "<-- " not in log.read_text(encoding="utf-8"):
            print("Плата не ответила. Проверьте прошивку и повторите проверку: " + task)
            return 1
    print("Логи записаны в " + str(folder))
    return 0


if __name__ == "__main__":
    sys.exit(main())
