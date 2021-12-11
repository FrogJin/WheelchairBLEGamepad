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
Run following commands inside *keyboard_mouse_emulate_on_raspberry* folder on the raspberry pi
```
sudo ./setup.sh
sudo ./boot.sh
./mouse/mouse_client.py
```

## Step 5
Pair Your Linux device with "Tim_Multiinterfacing_Device" and enjoy the gamepad!