### Removing the fan

I want to remove the internal silent fan from the PowerMac G4 MDD because the board does not provide sufficient current to start the fan rotating. This is okay for the single CPU tower since I can open and push-start it. However, with the dual-CPU tower, this was an issue since there is a short in the ground like around the door and the case causing the computer to shut-off when the door breaches this ground trace.

### Why?

Rather than removing the fan, I can attach a pair of wires with a 3.5mm barrel plug that will connect to an exteral supply. Such as a mobile battery and a 9-12VDC USB convertor supply to a socket just under the bulkhead and shortest path to the fan connector.

## OpenFirmware booting for USB

Follow these steps IN ORDER:

1. Completely power off (not restart) the Mac, then connect your USB stick/drive in a root USB port on your PowerPC Mac.
2. Power the Mac on and immediately hold down those 4 keys together: [ CMD + OPT/ALT + O + F ] until you see the white Open Firmware screen, similar to this

![image](/PowerMacG4-MDD/images/open_firmware.png)

At the Open Firmware command prompt, type this and press ENTER:

![txbi](/PowerMacG4-MDD/images/tbxi.jpg)

(Yes, there are 2 backslashes before :tbxi)

If it outputs an error, then try the command again but with usb1 instead of usb0.

If it still doesn't work, then your USB drive probably doesn't contain a suitable Mac OS release for your Mac, but you could still try the following generic command, which has very low chances of working, but still is worth trying:

![txbi](/PowerMacG4-MDD/images/tbxi2.jpg)

### Progress timeline

* Since 2018, these two PowerMacs ... it certainly matters --

![matters](/PowerMacG4-MDD/images/matters.png)