import serial
import time
import serial.tools.list_ports
from datetime import datetime

VALIDATION_DONE = False
FAILED = False
TARGET_HWID = "1A86:7523"  # Adjust based on your Arduino VID:PID

def log(msg):
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    with open("validation_log.txt", 'a', encoding='utf-8') as f:
        f.write(f"[{timestamp}] {msg}\n")

def find_arduino_port():
    ports = serial.tools.list_ports.comports()
    for port in ports:
        if TARGET_HWID in port.hwid:
            return port.device
    return None

def validate(port):
    global FAILED, VALIDATION_DONE

    try:
        ser = serial.Serial(port, 9600, timeout=2)
        time.sleep(2)

        # Flush startup prints
        while ser.in_waiting:
            ser.readline()

        # ON command
        ser.write(b'1')
        time.sleep(0.5)
        r1 = ser.readline().decode(errors="ignore").strip()

        if "LED ON" not in r1.upper():
            log(f"[FAIL] ON command: Expected 'LED ON', got '{r1}'")
            ser.close()
            FAILED = True
            VALIDATION_DONE = True
            return

        # OFF command
        ser.write(b'0')
        time.sleep(0.5)
        r2 = ser.readline().decode(errors="ignore").strip()

        if "LED OFF" not in r2.upper():
            log(f"[FAIL] OFF command: Expected 'LED OFF', got '{r2}'")
            ser.close()
            FAILED = True
            VALIDATION_DONE = True
            return

        ser.close()
        log(f"[PASS] Validation successful on {port}: ON → {r1}, OFF → {r2}")
        VALIDATION_DONE = True

    except Exception as e:
        log(f"[ERROR] Validation error: {e}")
        FAILED = True
        VALIDATION_DONE = True

# --- Persistent Loop ---
log("Daemon started...")

while True:
    if FAILED:
        # Stop forever if any failure has occurred
        break

    port = find_arduino_port()

    if port and not VALIDATION_DONE:
        log(f"Arduino detected at {port}")
        validate(port)

    time.sleep(2)
