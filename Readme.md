# ScriptSDCC 2.0

## Description and general purpose
ScriptSDCC is machine code interpreter compatible with MCS51 and Z180 architectures. It uses 64kB code and data memory, which can be common or separated, but on Z80/Z180 architecture, separated  memory is not usable, because Z80/Z180 processor is not designed to work on separated data and program memory.
ScriptSDCC requires Windows or Linux system with SDCC compiler installed with add SDCC path to environment variable PATH. This compiler is invoked during compiling and running script.
ScriptSDCC is designed to be compatible at the script source level with MCS51 and Z180, so in most cases, the same source can be compiled and run on either MSC51 or Z180 without changes. The only IO interface is the special address space with 256 bytes length called as swap memory. The IO interfaces existing in real processors are not supported. Interrupts are not supported, so interrupt functions can not be called.
ScriptSDCC has three input/output modules, each module exists in 4 independent instances. This modules are console, spreadsheet and graph. You can watch the same instance of I/O module in several windows, especially when you want to display graph in several perspectives.

## Bundle tab
You can define bundle, which consists of maximum 256 independed scripts. This is usable to run one script from another script or run several script simultaneously.

Each script is run in independed virtual machine, but IO and memory buffer is shared, so the first script can generate data, which are aquired by second script.

## Script tab
You can create/edit ScriptSDCC script definition using following data:
* __Description__ - User text intended to describe project, this text does not affect in script compiling and running.
* __View/Edit command__ - System command to view od edit project. For example, using this command you can open source file in text editor or display project folder.
* __Source file name__ - You have to input source *.c file with path. The path can be absolute or relative to project file. The file will be copied into temporary directory before compiling.
* __Binary file name__ - You have to input compiled hex file name without the path.
* __Library files__ - All needed file names from library directory or with path separated by &quot;|&quot;. These files will be copied into temporary directory before compiling.
* __Engine type__ - Select engine type between MSC51 or Z180.
* __Memory__ - Select common CODE/DATA or separated CODE and DATA memory architecture.
* __Compile command__ - Command line used in compilation.

In __Compile Command__ field, you can use wildcard strings from following values:
* __%CODELOC%__ - Code memory location.
* __%CODESIZE%__ - Code memory size.
* __%DATALOC%__ - Data memory location.
* __%DATASIZE%__ - Data memory size.
* __%SWAPLOC%__ - Swap memory location.

All mentioned values must be in hexadecimal form, these value are the most significant byte. For example, if you want to set 4000 to 7FFF range for data, you have to input &quot;40&quot; in __Data memory location__ and &quot;4000&quot; in __Data memory size__.
You can open existing project using __Open__ button and save project using __Save__ or __Save as__ button.

## Compile schemes
You can create compile scheme to save the most frequently used compile configuration. Compile scheme consists of engine type, CODE/DATA memory architecture and compile command. If you want to create scheme, input name of new scheme and click __Add/Modify scheme__. If scheme with entered name exists already, it will be replaced. If you want to remove scheme, select scheme in __Compile scheme__ and click __Remove scheme__.

## Compiling script
If you want to compile project, you have to click the __Compile__ button.
Before compiling, ScriptSDCC creates file &quot;_.h&quot; in temporary directory, which defines macros characteristic for current compilation.
The first macro is &quot;mem_swap&quot; with values, where there is defined swap memory address.
The second macro is &quot;engine_mcs51&quot; when compiling for MCS51 or &quot;engine_z180&quot; when compiling for Z180.
The third macro is &quot;mem_separated&quot; wher CODE and DATA memory are separated or &quot;mem_common&quot; when CODE and DATA are in common memory.
The macros can be used to write one code, which can be used on several configurations without code modification.
During compilation, if source file or library file is without path, the file will be searched in following paths:
1. Project path if project is saved.
2. Source code file path if this path is specified with source file name.
4. Library path.

Path existence is determined by detecting &quot;/&quot; or &quot;\\&quot; character in file name. ScriptSDCC will try the first case, the next following cases will be tryed only if previous case fails. If file is not copied, compilation process will not invoked and appropriate message will be in __Compiler messages__ on the __Compile and run__ tab. After compilation, if compile is not successful, on the __Compile and run__ tab, there are compiler error and warning messages in __Compiler messages__ field. If compile is successful, the field __Compiler messages__ is blank.

## Swap memory
During script running, there is one special memory space, which is used to communicate with I/O and controlling behavior from running script. For swap memory, there is reserved 256 bytes. After compiling or resetting, the first byte is set to zero and this value is followed before running every elementary instruction. When this value is other than 0 (this value can be treated as command), the ScriptSDCC does any action assigned to this value. If desired action needs any parameters or returns some value, the parameters are read from further bytes from swap memory and value is written to further bytes of swap memory. After doing action by ScriptSDCC, in most cases, the first byte is set to 0, so this allows to continue script running. If the first byte has set value other than 0, this script is stopped and cannot be resumed.
For script performance reasons, all commands with I/O ale multiples of 4 and there is added number of instances (from 0 to 3). The other commands are not multiples of any number. If any I/O command is designed to get or set data of miscellaneous types, there types are following numbers:
* __0__ - Signed char as text.
* __1__ - Unsigned char as text.
* __2__ - Signed char as number.
* __3__ - Unsigned char as number.
* __4__ - Signed short.
* __5__ - Unsigned short.
* __6__ - Signed int.
* __7__ - Unsigned int.
* __8__ - Signed long.
* __9__ - Unsigned long.
* __10__ - Float.

The value type byte is at Swap+1 address and the value is at Swap+8 and can occupy 8 bytes. The returned value is in Swap+8.

All commands are used in the standard library (C header files), the usage can be easily dedicated by function code analysis, these function are very simple. All functions are described in comments.

## Script running and performance
After compiling, you can run script by clicking __Run/Continue__ button. During script running, you can click __Stop__ button to stop running, then you can click __Run/Continue__ button to continue script running. The __Reset__ button resets virtual machine to allow run script again from beginning.
During script running, on the __Compile and run__ tab, you can watch current script status, total executed instructions and number of instructions per second.

## Console
The first I/O module is text console. It can print any text and get characters from keyboard using buffer. It is usable to print program running messages and get simple choices as menu selection or working mode.
You can control console using following buttons:
* __0__, __1__, __2__, __3__ - Display instances.
* __Clear__ - Clear console.
* __Copy__ - Copy selected text in console to clipboard.
* __Paste__ - Paste text from clipboard as keystrokes.

## Spreadsheet
The second I/O module it the spreadsheet are a simple table, which consists of 256 rows and 256 columns. This module is not intended to replace spreadsheet application from office suites, so it not supports formula calculation. Each cell contains text value, but it is possible to get or set number value.
Spreadsheet is mainly intended to enter input data and read output data. You can manipulate using buttons:
* __0__, __1__, __2__, __3__ - Display instances.
* __Clear__ - Clear selected area.
* __Copy__ - Copy selected area to clipboard as text with tab used as column separator.
* __Paste__ - Paste text from clipboard into selected area. The tab character in text is treated as column separator.
* __Row add__ - Add one row.
* __Row rem__ - Remove one row.
* __Col add__ - Add one column.
* __Col rem__ - Remove one column.

## Graph
The third I/O module is graph, which can visualize 2D or 3D graphics. It can displays points, polygonal chains including line segments and triangles. There is 256 draw threads, the 0 thread is intended to draw points and triangles. The other threads are intended to draw polygonal chain. Using threads, you can draw a few polygonal chains simultaneously. To draw polygonal chain, you have to select any thread other than 0 and reset this thread. Then, you can set points of the chain one by one. If you want to draw another chain in the same thread, you have to reset this thread.
To draw a triangle, tou have to plot at least three points and convert them to triangle using appropiate command. You can use the same points more than once, everytime there will be converted the last three plotted points. For example, you can draw tetragon by plotting the first three points (the second and third point must be a diagonal ends), convert them to first triangle, after that draw fouth point and convert to triangle again. The second triangle will be used second, third and fourth poins, so the second and third points will be used to both triangles.

The graph module has following display controls:
* __0__, __1__, __2__, __3__ - Display instances.
* __Basic__, __Scale__, __Rotate__, __Light__ - Show or hide toolbar.

Basic toolbar:
* __Clear__ - Clear graph.
* __Color__ - Set background color.
* __Point size__ - Set point size in pixels of plotted points.
* __Line size__ - Set line size in pixels of plotted line segments.
* __Surfaces__ - Fill triangle surfaces.
* __Text__ - Diaplay text labels.
* __Inv__ - Invert draw order at the same X, Y, Z display coordinates, this option can change artifacts in some cases.
* __Value change__ - Set value change step in all spin controls in other toolbars.
* __Render to file__ - Render picture to specified PNG file. The picture size can be specified.

Scale/pan toolbar, the following parameters has three values, each is for X, Y and Z coordinates:
* __Scale X__ - Scale coordinate value to X value on screen.
* __Scale Y__ - Scale coordinate value to Y value on screen.
* __Scale Z__ - Scale coordinate value to Z value in Z-buffer.
* __Scale XYZ__ - Scale coordinate proportionally on screen to change a scale. Values Scale X, Scale Y and Scale Z are multiplied by ScaleXYZ value. The product is in one millionth parts.
* __Pan data__ - Offset in one millionths of source to plot.
* __Pan render__ - Offset in pixels on screen of rendering graphics.

Rotate toolbar:
* __Rot data__ - Transform source data by specified axis order and rotate around axes in degrees.
* __Rot draw__ - Transform drawing data by specified axis order and rotate around axes in degrees.

Light toolbar:
* __Lighting__ - Specify pseudo-lighting algorithm.
* __Azimuth__, __Elevation__ - Set position of lighting line.
* __Color position__ - Set position on lighting line (distance algorithm) or on angle (angle algorithm), where objects has original colors.
* __Distance 1__, __Distance 2__ - Set position/angle in front of object and behind the object, where color is faded to background (positive value) or faded to inverted background (negative value). When value is set to 0, color will not be faded.

## Graph lighting - Distance algorithm
This algorithm uses color fade settings on lighting line. This line always contains the zero point (X=0, Y=0, Z=0) on screen. For each pixel in space, the color will be based on distance between zero point and plane perpendicular to lighting line.
The color position and distance parameters defines pixel color related to position on lighting line.

## Graph lighting - Angle algorithm
This algorithm user color fade related to object angle to lighting line. The color of points will be intact, because point has no dimensions.
For lines, lighting angle is measured in plane, which is both parallel to calculated line and lighting line. User observes line as always lit.
For triangles, lighting angle is measuret between triangle plane and lighting line. User can observe lit or unlit side. Shadows projected by other triangles are not simulated.
Color position 0 means color of line or triangle, which is parallel to lighting line. Color position means object angle, where has original color. The distances determines angle, where color fades to background or inverted background.

## Memory buffer
ScriptSDCC allows to use 256 memory buffer items, each are 65536 bytes long. Script can copy data from internal data memory to buffer and from buffer to internal data memory. Script can also copy data from file to buffer and from buffer to file. In whole ScriptSDCC session, there can be open 256 files simultaneously, each script can access to all memory buffer items and all file handles.

## Memory map
On the __Memory map__ tab you can see CODE or DATA or BUFF memory occupation. Each pixel on the map represents one byte. Using __H+__, __V+__, __H-__ and __V-__ buttons, you can zoom the map horizontally and vertically. If you click __CODE__ button, the script code occupation and code reads will be displayed, and if you click __DATA__ button, it will be displayed all reads and writes made during script running.
The colors of blank bytes means following:
* __Blank__ - Byte out of swap, code and data space.
* __Gray__ - Byte in swap space.
* __Dark blue__ - Byte in code space.
* __Dark yellow__ - Byte in data space.
If you see BUFF memory, the will be displayed shared memory buffer at the same number, whis is selected on Bundle tab. Below memory map, you can see file name attached to file handler. For example, if you select third item on __Bundle__ tab, you can see third memory buffer occuparion and third file handle assignment despite of script, item, memory buffer item and file handle are independed.

After compiling and during script running, colors may be changed as follows by setting maximum channel value:
* __Green__ - Read byte.
* __Red__ - Wrote byte.
* __Blue__ - Byte of program code based on HEX file.

These colors can be combined, which means as follows:
* __Yellow__ - Byte, which was wrote and read.
* __Cyan__ - Byte of code, which was read.
* __White__ - Byte of code, which was read and wrote.
* __Magenta__ - Byte of code, which was wrote and not read.

The cast of two cases theoretically can occur, but in most cases, it signals error in compilation parameters or script execution, because SDCC does not support self-modification code.

## Compiled code patching
Directly after compilation, there is applied some patch in beginning of memory to ensure correctly script execution.
* __MCS51__ - There are written three bytes from  &quot;0000&quot; to &quot;0002&quot; to ensure jump to program beginning. The first byte is always &quot;02&quot; (jump operation code) and the next two bytes are the code location beginning address in big endian order. Practically, the 0002 byte is always &quot;00&quot;.
* __Z180__ - There are written two bytes to &quot;0101&quot; and &quot;0102&quot;. These bytes is the stack pointer just after program beginning in little endian order. Practically, the first byte is always &quot;00&quot;. Without the patch, the stack for data will be begin always from &quot;FFFF&quot;.

## Project management
Project can be saved to file. In file, there will be saved all parameters from __Script__ tab. At the top of the tab, there list of favoutite projects, which providest fast and easy access to frequently used projects. You can manage projects using following buttons:
* __New__ - New project (clears all fields and current file name).
* __Open__ - Open project from file.
* __Save__ - Save project to current file. If file name is not specified, ScriptSDCC will ask for file name.
* __Save as__ - Save project to new file.
* __Add to list__ - Add project to favourite list. This project must be saved to file and in list field you have to specify display name before clicking this button.
* __Rem from list__ - Remove selected project from list.

## Settings
At the first run, you have to set settings on the __Settings__ tab. These settings are following:
* __Timer interval__ - Time in milliseconds between two refreshes. To take effect of changing this value, you have to restart SDCCScript.
* __SDCC command__ - Set SDCC compiler invocation command. This setting usually can be &quot;sdcc&quot;, but in certain installations, you have to input this command with full path.
* __Library directory__ - Path to directory, which contains files, which can be included as library (mentioned in __Library files__ field on __Script__ tab).
* __Temporary directory__ - Path to directory with read and write permission, which is used during compilation. Before compilation, all files placed in temporary directory will be removed.
* __Console font__ - Font name and size used to display text in console window.
* __Spreadsheet font__ - Font name and size used to display text in spreadsheet window.
* __Graph font file__ - Bitmap file containing font, which is used in text rendering in graph window.

## Graph font file
The font file, which can be used for labels in Graph, is a bitmap file. The bitmap width must be a multiplier of 256, the bitmap height has nor constraint. The font is monospaced font, which consists of 256 glyphs, the Unicode or similar standards are not supported. The pixels can be white and black, but the color is detected based on color brightness in picture. To get 8x8 font, you have to prepare picture 2048 pixels width and 8 pixels height. If picture is not valid, the labels will not displayed.

## Default project template
You can create default template, which will used for new projects. To create this, tou can have to save current project as &quot;default.sdc&quot; and plate it in application directory. After this, if you start ScriptSDCC or create new project using __New__ button, this file will be loaded, but project will be treated as new, unsaved project.

## Compiling and deploying source code
ScriptSDCC is prepared for Windows and Linux using Qt5 library and project is created with Qt Creator. The other operating systems are not supported and tested. ScriptSDCC works only if &quot;Q_OS_WIN&quot; or &quot;Q_OS_LINUX&quot; is defined in Qt library. To compile in both systems, you have to install standard Qt5 library and Qt Creator. Then, you have to build project as release.
To deploy in Windows, you have to use windeployqt.exe supplied with Qt library.
To deploy in Linux, you have to use &quot;linuxdeployqt-continuous-x86_64.AppImage&quot; from https://github.com/probonopd/linuxdeployqt/releases.
After deploying, application should run on another machine, without Qt preinstalled.
