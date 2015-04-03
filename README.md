# Project Spartan Launcher
A simple command line exe to launch Project Spartan at a URL. Handy if you need to launch Project Spartan from the command line or a batch script etc.

##Usage
1. Download the [latest](https://github.com/andysterland/ProjectSpartanLauncher/releases/download/v0.1/ProjectSpartanLauncher.exe) exe from releases 
2. From your favourite shell run `ProjectSpartanLauncher.exe [url]` 

That's it.

###Visual Studio Launching Spartan

1.	Download [latest](https://github.com/andysterland/ProjectSpartanLauncher/releases/download/v0.1/ProjectSpartanLauncher.exe) exe from releases 
2.	In Visual Studio open the “Browse With…” dialog (right click on a html file in solution explorer)
3.	Add a new program
4.	In the “Add Program” dialog
  1. Program: \<path to ProjectSpartanLauncher.exe\>
  2.	Arguments: \<leave empty\>
  3. Friendly Name: Project Spartan
5.	Once added you should now see “Project Spartan” in the F5 drop down

In the Win10 10049 flight Project Spartan can’t access localhost by default so you need to add the loopback permission. To do that from an admin command prompt:

`CheckNetIsolation LoopbackExempt -a -n=Microsoft.Windows.Spartan_cw5n1h2txyewy`
