### Removing the fan

I want to remove the internal silent fan from the PowerMac G4 MDD because the board does not provide sufficient current to start the fan rotating. This is okay for the single CPU tower since I can open and push-start it. However, with the dual-CPU tower, this was an issue since there is a short in the ground like around the door and the case causing the computer to shut-off when the door breaches this ground trace.

### Why?

Rather than removing the fan, I can attach a pair of wires with a 3.5mm barrel plug that will connect to an exteral supply. Such as a mobile battery and a 9-12VDC USB convertor supply to a socket just under the bulkhead and shortest path to the fan connector.

## OpenFirmware booting for USB

Follow these steps IN ORDER:

1. Completely power off (not restart) the Mac, then connect your USB stick/drive in a root USB port on your PowerPC Mac.
2. Power the Mac on and immediately hold down those 4 keys together: [ CMD + OPT/ALT + O + F ] until you see the white Open Firmware screen, similar to this

![image](/PowerMacG4-MDD/images/open_firmware.png)

Details are available [here](https://mac-classic.com/articles/open-firmware-basics/). But..before purusing this, what about ...

![image](/PowerMacG4-MDD/images/forthy.png)

`FORTH` _really??_

At the Open Firmware command prompt, type this and press ENTER:

![txbi](/PowerMacG4-MDD/images/tbxi.jpg)

(Yes, there are 2 backslashes before :tbxi)

If it outputs an error, then try the command again but with usb1 instead of usb0.

If it still doesn't work, then your USB drive probably doesn't contain a suitable Mac OS release for your Mac, but you could still try the following generic command, which has very low chances of working, but still is worth trying:

![txbi](/PowerMacG4-MDD/images/tbxi2.jpg)

## Burning the image onto a USB stick

Retrieve the [image](https://www.macintoshrepository.org/1307-mac-os-9-2-2-universal-2002-edition-).  Will need an account.

Burn the [image](https://www.macintoshrepository.org/articles/125-how-to-burn-or-write-or-clone-a-bootable-powerpc-disk-image-onto-an-usb-stick-in-windows)

What is Mac OS 9.2.2 "boot kit" for booting your G3/G4 from an USB stick?

This Mac OS 9.2.2 "boot kit" was made by @that-ben and makes the process of booting (and installing) Mac OS 9 on any G3 and most G4's from an USB stick much easier, because not all the Mac OS 9 install CD images can be used to boot a Mac from an unlocked volume (such as from an USB stick or hard drive).  This disk image does boot the computer into a fully useable desktop (albeit quite slow) and it's loaded with utilities and disk image mounting tools which will enable you to prepare and install Mac OS, everything without the need for a CD-ROM drive at all.  Since the USB drive is unlocked, you can even surf the web using the pre-installed Classilla browser and go get more files, directly from the same computer you're about to install Mac OS on :P

If you're about to install Mac OS 9 on the Mac you'll boot this USB drive with, then it's highly suggested to format the hard drive with the "Drive Setup" app that's on the top/left corner of the desktop before proceeding (or you'll also find it in the Applications folder on the USB drive).  If you don't want to format the whole hard drive, then at the very least run a Disk First Aid repair.  It's located in the Applications folder on the USB drive.  Ignoring both those suggestions could result in the Mac OS 9 installation aborting before it's finished.

What's included?

(v2) NEW! v2 includes everything v1 does (see below) but it has more free space on the bootable drive, plus new goodies that you'll see on the left side of the desktop, such as MR Browser to easily download more apps and games, Serial MReader for unlocking tons of shareware from the 90's, ShrinkWrap to mount .dsk images and even the full ISO CD image of Mac OS 9.2.2 universal (2002) which you can double click to mount, then directly proceed to install Mac OS 9 on your internal hard drive.

(v1) Pre-loaded software installed on the USB stick that you can use when booting off of it include:

* Classilla 9.3.3 (replaced by 9.3.4b in v2)
* Disk Copy 6.5b13
* Disk First Aid
* DiskTracker 2.3.3
* Drive Setup 2.0.7
* GraphicConverter Classic 6.5
* MacErrors 2.0.1
* NativeChecker 1.0
* QuickTime 6.0.3
* Quit Anything 1.6.1
* ResEdit 2.1.3
* SerialReader 2001 w/ Serial Box 08.2005 DB (replaced by Serial MReader w/ 2021 DB in v2)
* SoundApp 2.7.3
* StuffIt Deluxe 7.0.3
* Toast Titanium 5.2.3
* Virtual CD/DVD-ROM Utility 1.0d3

Available [here](https://www.macintoshrepository.org/download.php?id=65608).

### Progress timeline

* Since 2018, these two PowerMacs ... it certainly matters --

![matters](/PowerMacG4-MDD/images/matters.png)