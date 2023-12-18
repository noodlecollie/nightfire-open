// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.OptionNodeV10
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Qc
{
  public class OptionNodeV10
  {
    public const string Option = "node";
    private sbyte _EntryBone;

    public sbyte EntryBone
    {
      get => this._EntryBone;
      set => this._EntryBone = value;
    }

    public OptionNodeV10()
    {
    }

    public OptionNodeV10(sbyte entrybone) => this._EntryBone = entrybone;

    public new string ToString() => string.Format("{0} {1}", (object) "node", (object) this._EntryBone);
  }
}
