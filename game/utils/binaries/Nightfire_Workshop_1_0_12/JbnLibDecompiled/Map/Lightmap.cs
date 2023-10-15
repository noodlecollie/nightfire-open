// Decompiled with JetBrains decompiler
// Type: JbnLib.Map.Lightmap
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.Collections.Generic;

namespace JbnLib.Map
{
  public class Lightmap : List<int>
  {
    public int Scale
    {
      get => this[0];
      set => this[0] = value;
    }

    public int Rotation
    {
      get => this[1];
      set => this[1] = value;
    }

    public Lightmap()
    {
      this.Add(16);
      this.Add(0);
    }

    public Lightmap(int scale, int rotation)
    {
      this.Add(scale);
      this.Add(rotation);
    }

    public new string ToString() => string.Format("{0} {1}", (object) this[0], (object) this[1]);
  }
}
