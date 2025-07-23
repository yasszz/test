import serial.tools.list_ports
import serial
import random

def get_ports():
    return list(serial.tools.list_ports.comports())

def list_all_devices(ports):
    devices = []
    for port in ports:
        device_info = {
            'port': port.device,
            'desc': port.description,
            'serial': port.serial_number or "N/A"
        }
        devices.append(device_info)
    return devices

def user_selection_by_serial(devices):
    print("\nPlease select the device by entering its Serial Number:")
    for i, dev in enumerate(devices, start=1):
        print(f"{i}. {dev['desc']} at {dev['port']} [Serial: {dev['serial']}]")

    user_input = input("\nEnter Serial Number (or press Enter to auto-select): ").strip()

    if user_input:
        for dev in devices:
            if dev['serial'] and user_input == dev['serial']:
                print(f"\n✅ You selected: {dev['desc']} at {dev['port']}")
                return dev
        print("❌ Invalid Serial Number entered. Random device will be selected.")
    
    # fallback to random
    selected_device = random.choice(devices)
    print(f"\n🎯 Randomly selected: {selected_device['desc']} at {selected_device['port']}")
    return selected_device

# Main Execution
found_ports = get_ports()
connected_devices = list_all_devices(found_ports)

if len(connected_devices) == 2:
    print("✅ Two serial devices detected.")

    selected = user_selection_by_serial(connected_devices)

    try:
        ser = serial.Serial(selected['port'], baudrate=9600, timeout=1)
        print(f"\n🔌 Connected to {selected['desc']} at {selected['port']}")
    except Exception as e:
        print(f"\n❌ Failed to connect: {e}")

    print("\n🚫 Other device is ignored (blocked).")

else:
    print(f"\n⚠️ This script requires exactly 2 connected devices. Found: {len(connected_devices)}")
    if connected_devices:
        print("\n🔍 Found devices:")
        for dev in connected_devices:
            print(f"- {dev['desc']} at {dev['port']} [Serial: {dev['serial']}]")
    else:
        print("No serial devices detected.")

print("\n✅ DONE")

