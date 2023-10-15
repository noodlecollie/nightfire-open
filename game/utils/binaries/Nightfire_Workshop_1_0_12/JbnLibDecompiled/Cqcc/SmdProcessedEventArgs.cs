// Decompiled with JetBrains decompiler
// Type: JbnLib.Cqcc.SmdProcessedEventArgs
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Cqcc
{
  public class SmdProcessedEventArgs : EventArgs
  {
    private string _Name;
    private string[] _Textures;
    private int _Frames;
    private int _Triangles;

    public string Name => this._Name;

    public string[] Textures => this._Textures;

    public int Frames => this._Frames;

    public int Triangles => this._Triangles;

    public SmdProcessedEventArgs(string name, string[] textures, int frames, int triangles)
    {
      this._Name = name;
      this._Textures = textures;
      this._Frames = frames;
      this._Triangles = triangles;
    }

    public new string ToString() => this._Name;
  }
}
