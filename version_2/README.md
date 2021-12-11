# Instructions
## Step 1
Put rule(s) under *udev* folder into */usr/lib/udev/rules.d/* on your linux device

## Step 2
Put script(s) under *shell_scripts* folder into */bin/* on your linux device

## Step 3
Reload udev rules by running following command on your linux device
```
udevadm control --reload
```

## Step 4
Use Arduino to burn the script in *nRF52840_arduino_scripts* folder on the nRF52840 board, and keep nRF52840 connected with raspberry pi

## Step 5
On raspberry pi, run the script in "pi_scripts" by using the following command
```
sudo python3 mouse2tinyusb.py
```

## Step 6
Pair Your Linux device with "Bluefruit52_Joystick" and enjoy the gamepad!
(Note: You may experience high latency)