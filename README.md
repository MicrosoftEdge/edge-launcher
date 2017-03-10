# Microsoft Edge Launcher
A simple command line exe to launch the Microsoft Edge browser at a URL. Handy if you need to launch Microsoft Edge from the command line or a batch script etc.

## Getting started

### Binary download:
Download the [latest](https://github.com/MicrosoftEdge/MicrosoftEdgeLauncher/releases/tag/1.0.0.0) exe from releases.

### From NPM:
1. Run `npm install edge-launcher`
2. Open ./node_modules/edge-launcher/bin/MicrosoftEdgeLauncher.exe

## Usage
```
MicrosoftEdgeLauncher.exe [url] -k

    url - The URL to open in Microsoft Edge.
    -k  - Keep this program alive for as long as the launched process is alive.
```

### Visual Studio Launching Microsoft Edge
If you're using Visual Studio 2015 you don't need to do this as VS 2015 has built in support for Edge. If you are using an older version of VS and want to be able to F5 launch Edge you'll need to follow the steps below.

1.	Download [latest](https://github.com/MicrosoftEdge/MicrosoftEdgeLauncher/releases/tag/1.0.0.0) exe from releases 
2.	In Visual Studio open the “Browse With…” dialog (right click on a html file in solution explorer)
3.	Add a new program
4.	In the “Add Program” dialog
  1. Program: \<path to MicrosoftEdgeLauncher.exe\>
  2.	Arguments: \<leave empty\>
  3. Friendly Name: Edge
5.	Once added you should now see “Microsoft Edge” in the F5 drop down 
  1. This will not enable debugging of Script from VS, just launching

## Code of Conduct
This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/). For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.
