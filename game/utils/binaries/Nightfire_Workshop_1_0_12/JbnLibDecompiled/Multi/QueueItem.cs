// Decompiled with JetBrains decompiler
// Type: JbnLib.Multi.QueueItem
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Wrappers;

namespace JbnLib.Multi
{
  public class QueueItem
  {
    private string _Open;
    private string _Save;
    private StudioMdlArgs _Options;

    public string Open
    {
      get => this._Open;
      set => this._Open = value;
    }

    public string Save
    {
      get => this._Save;
      set => this._Save = value;
    }

    public StudioMdlArgs Options
    {
      get => this._Options;
      set => this._Options = value;
    }

    public QueueItem(string open, string save)
    {
      this._Open = open;
      this._Save = save;
      this._Options = new StudioMdlArgs();
    }

    public QueueItem(string open, string save, StudioMdlArgs options)
    {
      this._Open = open;
      this._Save = save;
      this._Options = options;
    }

    public new string ToString() => this._Open + ", " + this._Save + ", " + this._Options.ToString();
  }
}
