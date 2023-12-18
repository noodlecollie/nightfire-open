// Decompiled with JetBrains decompiler
// Type: JbnLib.Shared.ValueCountPair
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Shared
{
  public class ValueCountPair
  {
    private short _Value;
    private byte _Count;

    public short Value
    {
      get => this._Value;
      set => this._Value = value;
    }

    public byte Count
    {
      get => this._Count;
      set => this._Count = value;
    }

    public ValueCountPair()
    {
    }

    public ValueCountPair(short value)
    {
      this._Value = value;
      this._Count = (byte) 1;
    }

    public ValueCountPair(short value, byte count)
    {
      this._Value = value;
      this._Count = count;
    }
  }
}
