### Macintosh Programmer's Workshop

This folder contains the same software and documentation as the "Tool Chest:Development Kits:MPW etc." folder on the August 2001 Developer CD.

The About_MPW_etc.sit.hqx document explains the contents of the "MPW etc." folder, the changes since the February 2001 Dev CD, how to install MPW, and more.

Most of the documents in the Documentation folder are provided in Adobe Acrobat format. Adobe's Acrobat Reader application is available from:
<http://www.adobe.com/acrobat/>

For updates to MPW, visit the MPW web site at: <http://developer.apple.com/tools/mpw-tools/>

For answers to frequently asked questions about MPW, check out the MPW FAQ at: <http://developer.apple.com/tools/mpw-tools/mpw_faq.html>



Getting Started With MPW
========================

Step 1 - You will need the Disk Copy application which is available from
<ftp://ftp.apple.com/developer/Utilities/>

Step 2 - Download the MPW-GM.img.bin file contained in the MPW-GM_Images folder.
This file is a MacBinary-encoded disk image file. The application that you use to
download the file (Internet Explorer, Netscape Communicator, Fetch, etc.) should
automatically decode the MacBinary file. The resulting file should be an image
file named MPW-GM.img.

Step 3 - Double-click on the downloaded image file to mount the disk image.
Open the disk image and copy the MPW-GM folder to your hard drive. You can now 
drag the disk image to the trash and then delete the image file. The MPW-GM folder 
contains a fully configured MPW environment and will work as supplied; do not 
"rearrange" the contents of the folder unless you are experienced with using MPW.

Step 4 - If you are running MPW on a Power Macintosh with a version of system 
software prior to Mac OS 7.6, copy the StdCLibInit file from 
":MPW-GM:Interfaces&Libraries:RuntimeLibraries:Required for MPW:Required for Pre-Mac OS 7.6:" 
to the Extensions folder of your System Folder. After copying the file, 
reboot your system.

Step 5 - To run the MPW environment, launch the application called "MPW Shell"
which is located inside the ":MPW-GM:MPW:" folder on your hard drive. 

Step 6 - Download and read the MPW documentation, specifically 
"Introduction to MPW" 
<ftp://ftp.apple.com/developer/Tool_Chest/Core_Mac_OS_Tools/MPW_etc./Documentation/MPW_Reference/Introduction_to_MPW.sit.hqx> 
and "Building and Managing Programs in MPW"  
<ftp://ftp.apple.com/developer/Tool_Chest/Core_Mac_OS_Tools/MPW_etc./Documentation/MPW_Reference/Building_Progs_In_MPW.sit.hqx>
Note that these manuals are slightly out of date and may contain some 
discrepancies when used with the current version of MPW. Changes to MPW that 
occured after the manuals were written are documented in the Release Notes
<ftp://ftp.apple.com/developer/Tool_Chest/Core_Mac_OS_Tools/MPW_etc./Documentation/MPW-GM_Release_Notes.sit.hqx> 

Example programs are located inside the ":MPW-GM:MPW:Examples:" folder. 
Instructions and makefiles are provided in the example folders.

To build your own programs, use the "Create Build Commands" and "Build" items in the
Build menu. Select the "SIOW App." radio button to build your program as an SIOW 
(Simple Input/Output Window) application. The SIOW package enables your program to 
interact with the user through a simple window and to exhibit, at least partially, 
typical Macintosh appearance and behavior. See Chapter 15 in the "Building and 
Managing Programs in MPW" manual for more information about SIOW applications.

You can also build your program as an MPW tool. To do this, use the "Create Build 
Commands" item in the Build menu and select the "Tool" radio button. Your program 
will be built as an MPW tool which can then be executed from the MPW worksheet by 
simply typing the name of the tool and pressing the Enter key. Since the MPW Shell 
provides a command-line environment, your program can display output and retrieve 
input from the MPW worksheet. Note that you must press the Enter key instead of 
the Return key to send input to your program and that the entire line is sent as 
input. For this reason, program output such as prompts should always end with a 
newline ('\n') character so that any input can be entered on its own line.

Additional information about building programs is contained in Chapter 1 of 
"Building and Managing Programs in MPW".



For assistance with MPW, we recommend you subscribe to the MPW-Dev Internet 
mailing list by visiting: 
<http://lists.apple.com/mailman/listinfo/mpw-dev#Subscribe>

You can browse the MPW-Dev mailing list archives by visiting:
<http://lists.apple.com/mailman/listinfo/mpw-dev#Archives>



The MPW team
August 2001



