import serial
import time
from datetime import datetime

# --- Setup ---
arduino = serial.Serial('COM4', 9600)
time.sleep(2)

# --- Log File Setup ---
txt_log_path = "led_log.txt"
html_log_path = "led_log.html"

# --- Initialize HTML log file ---
with open(html_log_path, 'w') as html_file:
    html_file.write("<html><head><title>LED Control Log</title></head><body>")
    html_file.write("<h2>Arduino LED Control Log</h2><table border='1'>")
    html_file.write("<tr><th>Time</th><th>Command</th><th>Response</th></tr>")

# --- Logging Function ---

def log(action):
    timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    msg = f"[{timestamp}] {action}"
    # - Plain text log
    with open(txt_log_path, "a") as f:
        f.write(msg + "\n")
    # - HTML log
    html_msg = f"<tr><td>{timestamp}</td><td>{action}</td></tr>\n"
    try:
        with open(html_log_path, "r") as f:
            content = f.read()
    except FileNotFoundError:
        content = ""
    if "<table" not in content:
        with open(html_log_path, "w") as f:
            f.write("<html><head><title>LED Action Log</title></head><body>\n")
            f.write("<h2>LED Action Log</h2>\n")
            f.write("<table border='1'><tr><th>Timestamp</th><th>Action</th></tr>\n")
            f.write(html_msg)
            f.write("</table></body></html>")
    else:
        new_content = content.replace("</table>", html_msg + "</table>")
        with open(html_log_path, "w") as f:
            f.write(new_content)
# --- Arduino Command Function ---
def send_command(cmd):
    arduino.write(cmd.encode())
    time.sleep(0.1)
    response = arduino.readline().decode().strip()
    print("Arduino response:", response)
    log(cmd)


while True:
    cmd = input("Enter 1 to turn ON, 0 to turn OFF (or q to quit): ")
    if cmd.lower() == 'q':
        break
    elif cmd in ['1', '0']:
        send_command(cmd)
    else:
        response = "Invalid input (not sent to Arduino)"
        print(response)
        log(cmd)

# --- Finish HTML log ---
with open(html_log_path, 'a') as html_file:
    html_file.write("</table></body></html>")

print("Logs saved to led_log.txt and led_log.html")
