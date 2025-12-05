import serial
import time
import keyboard  # pip install keyboard

# HM-10 Serial
bt_port = 'COM5'
bt_baud = 9600
bt = serial.Serial(bt_port, bt_baud)
time.sleep(2)
print("Teleoperation mode: Press keys to control BillyBass. Press ESC to quit.")

try:
    while True:
        if keyboard.is_pressed('w'):
            bt.write(b'MOUTH\n')
            print("MOUTH")
            time.sleep(0.1)
        elif keyboard.is_pressed('a'):
            bt.write(b'RIGHT\n')
            print("RIGHT")
            time.sleep(0.1)
        elif keyboard.is_pressed('d'):
            bt.write(b'TAIL\n')
            print("TAIL")
            time.sleep(0.1)
        elif keyboard.is_pressed('s'):
            bt.write(b'DANCE\n')
            print("DANCE")
            time.sleep(0.1)
        elif keyboard.is_pressed('p'):
            bt.write(b'HEADTURN\n')
            print("HEADTURN")
            time.sleep(0.1)                       
        elif keyboard.is_pressed('q'):
            bt.write(b'STOP\n')
            print("STOP")
            time.sleep(0.1)
        elif keyboard.is_pressed('esc'):
            break
except KeyboardInterrupt:
    pass
finally:
    bt.close()
    print("Teleoperation ended.")