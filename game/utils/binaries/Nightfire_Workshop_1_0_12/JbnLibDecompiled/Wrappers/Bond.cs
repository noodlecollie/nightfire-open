// Decompiled with JetBrains decompiler
// Type: JbnLib.Wrappers.Bond
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using System.Diagnostics;
using System.IO;

namespace JbnLib.Wrappers
{
  public class Bond
  {
    private FileInfo _Executable;

    public string Path => this._Executable.FullName;

    public string Name => this._Executable.Name;

    public bool Exists => this._Executable.Exists;

    public Bond(string path) => this._Executable = new FileInfo(path);

    public string Move(string newpath)
    {
      this._Executable.MoveTo(newpath);
      this._Executable = new FileInfo(newpath);
      return this._Executable.FullName;
    }

    public void Start() => this.Start("", false);

    public void Start(string args) => this.Start(args, false);

    public void Start(BondArgs args) => this.Start(args.ToString(), false);

    public void Start(BondArgs args, bool waitforexit) => this.Start(args.ToString(), waitforexit);

    public void Start(string args, bool waitforexit)
    {
      if (!this._Executable.Exists)
        Messages.ThrowException("Wrappers.Bond.Start(string, bool)", Messages.FILE_NOT_FOUND + this._Executable.FullName);
      ProcessStartInfo startInfo = new ProcessStartInfo(this._Executable.FullName, args);
      startInfo.WorkingDirectory = this._Executable.DirectoryName;
      startInfo.UseShellExecute = false;
      if (waitforexit)
        Process.Start(startInfo).WaitForExit();
      else
        Process.Start(startInfo);
    }
  }
}
