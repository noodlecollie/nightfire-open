// Decompiled with JetBrains decompiler
// Type: JbnLib.Wrappers.Rad
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using System.Diagnostics;
using System.IO;

namespace JbnLib.Wrappers
{
  public class Rad
  {
    private FileInfo _Executable;

    public string Path => this._Executable.FullName;

    public string Name => this._Executable.Name;

    public bool Exists => this._Executable.Exists;

    public Rad(string path) => this._Executable = new FileInfo(path);

    public string Move(string newpath)
    {
      this._Executable.MoveTo(newpath);
      this._Executable = new FileInfo(newpath);
      return this._Executable.FullName;
    }

    public string Compile(string map) => this.Compile(map, "", false);

    public string Compile(string map, bool nowindow) => this.Compile(map, "", nowindow);

    public string Compile(string map, string args) => this.Compile(map, args, false);

    public string Compile(string map, SharedArgs shared) => this.Compile(map, shared.ToString(), false);

    public string Compile(string map, SharedArgs shared, bool nowindow) => this.Compile(map, shared.ToString(), nowindow);

    public string Compile(string map, SharedArgs shared, string args) => this.Compile(map, shared.ToString() + args, false);

    public string Compile(string map, SharedArgs shared, string args, bool nowindow) => this.Compile(map, shared.ToString() + args, nowindow);

    public string Compile(string map, SharedArgs shared, RadArgs args) => this.Compile(map, shared.ToString() + args.ToString(), false);

    public string Compile(string map, SharedArgs shared, RadArgs args, bool nowindow) => this.Compile(map, shared.ToString() + args.ToString(), nowindow);

    public string Compile(string map, RadArgs args) => this.Compile(map, args.ToString(), false);

    public string Compile(string map, RadArgs args, bool nowindow) => this.Compile(map, args.ToString(), nowindow);

    public string Compile(string map, string args, bool nowindow)
    {
      if (!this._Executable.Exists)
        Messages.ThrowException("Wrappers.Rad.Compile(string, string, bool)", Messages.FILE_NOT_FOUND + this._Executable.FullName);
      FileInfo fileInfo = new FileInfo(map);
      if (!fileInfo.Exists)
        Messages.ThrowException("Wrappers.Rad.Compile(string, string, bool)", Messages.FILE_NOT_FOUND + map);
      Process.Start(new ProcessStartInfo(this._Executable.FullName, args + "\"" + fileInfo.FullName + "\"")
      {
        WorkingDirectory = fileInfo.DirectoryName,
        UseShellExecute = false,
        CreateNoWindow = nowindow
      }).WaitForExit();
      if (File.Exists(fileInfo.FullName + ".log"))
      {
        StreamReader streamReader = new StreamReader(fileInfo.FullName + ".log");
        while (streamReader.Peek() != -1)
        {
          string str = streamReader.ReadLine();
          if (str.StartsWith("Error: ") || str.StartsWith("ERROR: "))
            Messages.ThrowException("brad.exe", str.Replace("Error: ", "").Replace("ERROR: ", ""));
        }
        streamReader.Close();
      }
      return fileInfo.DirectoryName + "\\" + fileInfo.Name.Substring(0, fileInfo.Name.Length - fileInfo.Extension.Length) + ".bsp";
    }
  }
}
