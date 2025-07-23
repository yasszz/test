import serial
import time

# Replace with your Arduino's COM port
port = 'COM4'  # or '/dev/ttyACM0' on Linux
baud = 57600   # StandardFirmata uses 57600 baud

# Firmata constants
START_SYSEX = 0xF0
END_SYSEX = 0xF7
REPORT_FIRMWARE = 0x79

# Create the command to request firmware info
report_firmware_cmd = bytes([START_SYSEX, REPORT_FIRMWARE, END_SYSEX])

# Open serial port
with serial.Serial(port, baud, timeout=2) as ser:
    # Wait a moment for Arduino reset
    time.sleep(2)
    
    # Flush any junk
    ser.reset_input_buffer()

    # Send the request
    ser.write(report_firmware_cmd)

    # Read response
    response = ser.read(64)
    print("Raw response:", response)

    # Check if response contains firmware name
    if bytes([START_SYSEX, REPORT_FIRMWARE]) in response:
        print("✅ StandardFirmata is running and responded correctly.")
    else:
        print("❌ No valid response from Firmata.")
