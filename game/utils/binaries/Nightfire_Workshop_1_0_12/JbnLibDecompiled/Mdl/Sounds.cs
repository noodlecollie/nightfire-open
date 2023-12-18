// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.Sounds
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;
using System.Collections.Generic;
using System.Text;

namespace JbnLib.Mdl
{
  public class Sounds : IStructure
  {
    private List<string> _Names;

    public static int Length => 644;

    public List<string> Names
    {
      get => this._Names;
      set => this._Names = value;
    }

    public Sounds(byte[] raw)
    {
      this._Names = new List<string>();
      int index1 = 4;
      for (int index2 = 0; index2 < 10; ++index2)
      {
        string str = Encoding.ASCII.GetString(raw, index1, 64).Trim(new char[1]);
        if (str.Length > 0)
          this._Names.Add(str);
        index1 += 64;
      }
    }

    public Sounds(List<string> names) => this._Names = names;

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[Sounds.Length];
      BitConverter.GetBytes(this._Names.Count).CopyTo((Array) bytes, 0);
      int index1 = 0;
      for (int index2 = 4; index2 < bytes.Length; index2 += 64)
      {
        if (index1 < this._Names.Count)
          Encoding.ASCII.GetBytes(this._Names[index1]).CopyTo((Array) bytes, index2);
        ++index1;
      }
      return bytes;
    }

    public new string ToString()
    {
      if (this._Names.Count <= 0)
        return "";
      string str = this._Names[0];
      for (byte index = 1; (int) index < this._Names.Count; ++index)
        str = str + ", " + this._Names[(int) index];
      return str;
    }
  }
}
