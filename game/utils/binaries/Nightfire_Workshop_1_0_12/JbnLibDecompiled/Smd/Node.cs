// Decompiled with JetBrains decompiler
// Type: JbnLib.Smd.Node
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Smd
{
  public class Node
  {
    private sbyte _Index;
    private string _Name;
    private sbyte _Parent;

    public sbyte Index
    {
      get => this._Index;
      set => this._Index = value;
    }

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public sbyte Parent
    {
      get => this._Parent;
      set => this._Parent = value;
    }

    public Node()
    {
    }

    public Node(string line)
    {
      string[] strArray = line.Split(new char[1]{ ' ' }, StringSplitOptions.RemoveEmptyEntries);
      this._Index = Convert.ToSByte(strArray[0]);
      this._Name = strArray[1];
      for (int index = 2; index < strArray.Length - 1; ++index)
        this._Name = this._Name + " " + strArray[index];
      this._Name = this._Name.Trim('"');
      this._Parent = Convert.ToSByte(strArray[strArray.Length - 1]);
    }

    public Node(sbyte index, string name, sbyte parent)
    {
      this._Index = index;
      this._Name = name;
      this._Parent = parent;
    }

    public new string ToString() => this._Index.ToString().PadLeft(3) + " \"" + this._Name + "\" " + this._Parent.ToString().PadLeft(3);
  }
}
