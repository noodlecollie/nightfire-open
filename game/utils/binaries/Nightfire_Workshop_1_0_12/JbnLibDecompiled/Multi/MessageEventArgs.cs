// Decompiled with JetBrains decompiler
// Type: JbnLib.Multi.MessageEventArgs
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Multi
{
  public class MessageEventArgs : EventArgs
  {
    private string _File;
    private int _LogLine;
    private string _Message;

    public string File => this._File;

    public int LogLine => this._LogLine;

    public string Message => this._Message;

    public MessageEventArgs(string file, int logline, string message)
    {
      this._File = file;
      this._LogLine = logline;
      this._Message = message;
    }

    public new string ToString() => this._File + " (Line: " + (object) this._LogLine + "): " + this._Message;
  }
}
