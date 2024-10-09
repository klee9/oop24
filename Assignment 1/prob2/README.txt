OS: Microsoft Windows 11
Compiler: Microsoft (R) C/C++ Optimizing Compiler Version 19.41.34123 for x86

[How to compile]
1. Open Visual Studio.
2. Open the included .sln file (File > Open > project/solution > choose the provided .sln file).
3. Build the solution (Build > Build solution).

[How to execute]
1. Click run button in Visual Studio.

[Side note]
I've included a vscproj.user file to specify the command line argument(file1.txt). However, if there are no arguemts, please follow the steps below.
1. In Visual Studio, go to the Solution Explorer (usually on the right pane).
2. Right-click on your project (not the solution) and select Properties.
3. In the Properties window, expand the Configuration Properties section.
4. Select Debugging from the list.
5. Look for the Command Arguments field on the right side of the Debugging page and add "file1.txt".
