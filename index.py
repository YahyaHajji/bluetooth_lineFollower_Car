import serial
import time
from keyboard import on_press_key, on_release_key

# Serial port parameters
bluetooth_port = "COM11"  # Replace with your HC-05 port
baud_rate = 9600

try:
    # Initialize Bluetooth connection
    bluetooth = serial.Serial(bluetooth_port, baud_rate, timeout=1)
    print("Bluetooth connection established.")
    time.sleep(2)

    # Modes: 0 = Bluetooth, 1 = Line Follower
    current_mode = 0

    def switch_to_line_follower(event):
        global current_mode
        if current_mode == 0:
            bluetooth.write("LINE_FOLLOWER\n".encode('utf-8'))
            print("Switched to Line Follower mode.")
            current_mode = 1

    def switch_to_bluetooth(event):
        global current_mode
        if current_mode == 1:
            bluetooth.write("BLUETOOTH_MODE\n".encode('utf-8'))
            print("Switched to Bluetooth control mode.")
            current_mode = 0

    def send_command(command):
        bluetooth.write(command.encode('utf-8'))
        print(f"Sent: {command.strip()}")

    # Directional key press handlers
    def move_up(event):
        if current_mode == 0:
            send_command("up\n")

    def move_down(event):
        if current_mode == 0:
            send_command("down\n")

    def move_left(event):
        if current_mode == 0:
            send_command("left\n")

    def move_right(event):
        if current_mode == 0:
            send_command("right\n")

    
    def stop(event):
        if current_mode == 0:
            send_command("STOP\n")

   
    on_press_key("up", move_up)
    on_press_key("down", move_down)
    on_press_key("left", move_left)
    on_press_key("right", move_right)

   
    on_release_key("up", stop)
    on_release_key("down", stop)
    on_release_key("left", stop)
    on_release_key("right", stop)

   
    on_press_key("a", switch_to_line_follower)
    on_press_key("m", switch_to_bluetooth)

    print("Ready for commands. Default mode: Bluetooth.")

    while True:
        time.sleep(0.01)

except serial.SerialException as e:
    print(f"Serial connection error: {e}")
finally:
    if 'bluetooth' in locals() and bluetooth.is_open:
        bluetooth.close()
        print("Bluetooth connection closed.")
