// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.AnimationValue
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.Runtime.InteropServices;

namespace JbnLib.Mdl
{
  [StructLayout(LayoutKind.Explicit)]
  public struct AnimationValue
  {
    [FieldOffset(0)]
    public short Value;
    [FieldOffset(0)]
    public byte Valid;
    [FieldOffset(1)]
    public byte Total;

    public AnimationValue(short value)
    {
      this.Valid = (byte) 0;
      this.Total = (byte) 0;
      this.Value = value;
    }

    public AnimationValue(byte valid, byte total)
    {
      this.Value = (short) 0;
      this.Valid = valid;
      this.Total = total;
    }
  }
}
