// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandLodTemp
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.Collections.Generic;

namespace JbnLib.Qc
{
  public class CommandLodTemp
  {
    public const string Command = "$lodtemp";
    private int _Index;
    private List<int> _Distances = new List<int>();

    public int Index
    {
      get => this._Index;
      set => this._Index = value;
    }

    public List<int> Distances
    {
      get => this._Distances;
      set => this._Distances = value;
    }

    public CommandLodTemp()
    {
    }

    public CommandLodTemp(int index, params int[] distances)
    {
      this._Index = index;
      foreach (int distance in distances)
        this._Distances.Add(distance);
    }

    public CommandLodTemp(int index, List<int> distances)
    {
      this._Index = index;
      this._Distances = distances;
    }

    public new string ToString()
    {
      string str = "$lodtemp " + (object) this._Index + " " + (object) this._Distances.Count;
      for (int index = 0; index < this._Distances.Count; ++index)
        str = str + " " + (object) this._Distances[index];
      return str;
    }
  }
}
