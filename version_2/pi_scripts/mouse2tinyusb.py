#!/usr/bin/python
import sys
import time
import serial
import usb.core
import usb.util

# configure the serial connections
ser = serial.Serial(port='/dev/ttyACM0', baudrate=115200)
ser.isOpen()

# hexidecimal vendor and product values
# logitech USB Mouse
dev = usb.core.find(idVendor=0x046d, idProduct=0xc077)

# MIRAD CHIPS, MTA42000 USB Mouse
# dev = usb.core.find(idVendor=0x0a0c, idProduct=0x0001)

# Headset, Dachris Inc, USB Mouse Mover II

# first endpoint
interface = 0
endpoint = dev[0][(0,0)][0]

# if the OS kernel already claimed the device, which is most likely true
# thanks to http://stackoverflow.com/questions/8218683/pyusb-cannot-set-configuration
if dev.is_kernel_driver_active(interface) is True:
  # tell the kernel to detach
  dev.detach_kernel_driver(interface)
  # claim the device
  usb.util.claim_interface(dev, interface)

while True:
    try:
        data = dev.read(endpoint.bEndpointAddress,endpoint.wMaxPacketSize)
        
        # gamepad string map: [lft_btn, rht_btn, mid_btn, left, right, up, down]
        gpd_str = ""
        # left mouse button click
        if data[0] == 1:
            gpd_str += "y"
        else:
            gpd_str += "n"
        # right mouse button click
        if data[0] == 2:
            gpd_str += "y"
        else:
            gpd_str += "n"
        # mid mouse button click
        if data[0] == 4:
            gpd_str += "y"
        else:
            gpd_str += "n"
        # mouse move left
        if data[1] > 200:
            gpd_str += "y"
        else:
            gpd_str += "n"
        # mouse move right
        if data[1] < 50 and data[1] > 0:
            gpd_str += "y"
        else:
            gpd_str += "n"
        # mouse move up
        if data[2] > 200:
            gpd_str += "y"
        else:
            gpd_str += "n"
        # mouse move down
        if data[2] < 50 and data[2] > 0:
            gpd_str += "y"
        else:
            gpd_str += "n"

        print(gpd_str)

        # send the string to the device
        ser.write(gpd_str.encode())

    except usb.core.USBError as e:
        data = None
        if e.args == ('Operation timed out',):
            continue

    except KeyboardInterrupt:
        # release the device
        usb.util.release_interface(dev, interface)
        # reattach the device to the OS kernel
        dev.attach_kernel_driver(interface)

        print("Keyboard Interrupted")
        sys.exit()
