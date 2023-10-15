// Decompiled with JetBrains decompiler
// Type: JbnLib.Smd.Triangle`1
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.Collections.Generic;

namespace JbnLib.Smd
{
  public class Triangle<T>
  {
    private string _Texture;
    private List<T> _Vertices = new List<T>();

    public string Texture
    {
      get => this._Texture;
      set => this._Texture = value;
    }

    public List<T> Vertices
    {
      get => this._Vertices;
      set => this._Vertices = value;
    }

    public Triangle()
    {
    }

    public Triangle(string texture, List<T> vertices)
    {
      this._Texture = texture;
      this._Vertices = vertices;
    }

    public void Flip()
    {
      T vertex = this._Vertices[0];
      this._Vertices[0] = this.Vertices[2];
      this._Vertices[2] = vertex;
    }
  }
}
