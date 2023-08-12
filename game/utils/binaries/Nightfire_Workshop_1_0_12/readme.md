This was made by UltimateSniper and Ford.

It was downloaded from https://onlynoob.webs.com/downloads.htm

Real shame this isn't open source. However, apparently .NET DLLs are just in an intermediate instruction language (see https://stackoverflow.com/questions/3628798/what-is-in-a-dll-and-how-does-it-work for more information), so running Visual Studio's `ildasm` utility can output a fairly readable description of the code within. `*.disassembly.html` and `*.disassembly.txt` files are the result of running this from the VS developer console, eg:

```
ildasm /out=JbnLib.dll.disassembly.txt /text /classlist .\JbnLib.dll
ildasm /out=JbnLib.dll.disassembly.html /html /classlist .\JbnLib.dll
```
