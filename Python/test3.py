import time
from pyfirmata2 import Arduino, util

# Set the port (replace with your actual COM port)
port = Arduino.AUTODETECT  # or 'COM3', '/dev/ttyUSB0', etc.

# Connect to the board
board = Arduino(port)

# Optional: Wait a moment for Firmata to be ready
time.sleep(2)

# Use digital pin 13
led_pin = board.get_pin('d:13:o')  # 'd' = digital, 13 = pin number, 'o' = output

try:
    while True:
        led_pin.write(1)   # Turn ON LED
        print("LED ON")
        time.sleep(5)

        led_pin.write(0)   # Turn OFF LED
        print("LED OFF")
        time.sleep(10)

except KeyboardInterrupt:
    print("Exiting...")
    board.exit()
