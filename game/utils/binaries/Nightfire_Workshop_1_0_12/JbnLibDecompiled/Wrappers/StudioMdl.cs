// Decompiled with JetBrains decompiler
// Type: JbnLib.Wrappers.StudioMdl
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using JbnLib.Qc;
using System.Diagnostics;
using System.IO;

namespace JbnLib.Wrappers
{
  public class StudioMdl
  {
    private FileInfo _Executable;

    public string Path => this._Executable.FullName;

    public string Name => this._Executable.Name;

    public bool Exists => this._Executable.Exists;

    public StudioMdl(string path) => this._Executable = new FileInfo(path);

    public string Move(string newpath)
    {
      this._Executable.MoveTo(newpath);
      this._Executable = new FileInfo(newpath);
      return this._Executable.FullName;
    }

    public string Compile(string qc) => this.Compile(qc, "", true);

    public string Compile(string qc, bool nowindow) => this.Compile(qc, "", nowindow);

    public string Compile(string qc, string args) => this.Compile(qc, args, true);

    public string Compile(string qc, StudioMdlArgs args) => this.Compile(qc, args.ToString(), true);

    public string Compile(string qc, StudioMdlArgs args, bool nowindow) => this.Compile(qc, args.ToString(), nowindow);

    public string Compile(string qc, string args, bool nowindow)
    {
      if (!this._Executable.Exists)
        Messages.ThrowException("Wrappers.StudioMdl.Compile(string, string, bool)", Messages.FILE_NOT_FOUND + this._Executable.FullName);
      FileInfo fileInfo = new FileInfo(qc);
      if (!fileInfo.Exists)
        Messages.ThrowException("Wrappers.StudioMdl.Compile(string, string, bool)", Messages.FILE_NOT_FOUND + qc);
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
          if (str.StartsWith("illegal"))
            Messages.ThrowException("studiomdl.exe", str + "\n" + streamReader.ReadLine());
          if (str.StartsWith("Error: ") && str != "Error: See above messages")
            Messages.ThrowException("studiomdl.exe", str.Replace("Error: ", ""));
        }
        streamReader.Close();
      }
      return fileInfo.DirectoryName + "\\" + QcFile.GetModelName(fileInfo.FullName);
    }
  }
}
