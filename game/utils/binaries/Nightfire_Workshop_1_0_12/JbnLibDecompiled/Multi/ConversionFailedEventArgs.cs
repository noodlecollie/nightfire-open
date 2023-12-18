// Decompiled with JetBrains decompiler
// Type: JbnLib.Multi.ConversionFailedEventArgs
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Multi
{
  public class ConversionFailedEventArgs
  {
    private int _Id;
    private string _File;
    private Exception _Exception;

    public int Id => this._Id;

    public string File => this._File;

    public Exception Exception => this._Exception;

    public ConversionFailedEventArgs(int id, string file, Exception ex)
    {
      this._Id = id;
      this._File = file;
      this._Exception = ex;
    }
  }
}
