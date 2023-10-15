// Decompiled with JetBrains decompiler
// Type: JbnLib.Assets.FileProgressChangedEventArgs
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Assets
{
  public class FileProgressChangedEventArgs : EventArgs
  {
    private int _FilesDone;
    private int _FilesToGo;

    public int FilesDone => this._FilesDone;

    public int FilesToGo => this._FilesToGo;

    public FileProgressChangedEventArgs(int done, int togo)
    {
      this._FilesDone = done;
      this._FilesToGo = togo;
    }
  }
}
