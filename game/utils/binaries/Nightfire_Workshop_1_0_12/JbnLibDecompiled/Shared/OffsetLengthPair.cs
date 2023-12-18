// Decompiled with JetBrains decompiler
// Type: JbnLib.Shared.OffsetLengthPair
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Shared
{
  public class OffsetLengthPair
  {
    private int _Offset;
    private int _Length;

    public int Offset
    {
      get => this._Offset;
      set => this._Offset = value;
    }

    public int Length
    {
      get => this._Length;
      set => this._Length = value;
    }

    public OffsetLengthPair()
    {
      this._Offset = 0;
      this._Length = 0;
    }

    public OffsetLengthPair(int offset, int length)
    {
      this._Offset = offset;
      this._Length = length;
    }

    public new string ToString() => this._Length.ToString() + " @ " + (object) this._Offset;
  }
}
