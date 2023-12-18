// Decompiled with JetBrains decompiler
// Type: JbnLib.Wrappers.VisArgs
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.ComponentModel;

namespace JbnLib.Wrappers
{
  public class VisArgs
  {
    private VisArgs.Visability _Visibility = VisArgs.Visability.Normal;

    [Category("BVis")]
    [DefaultValue(VisArgs.Visability.Normal)]
    [Description("Determines how complex the visibility calculations will be.  This can have a major effect on how long it takes to compile a map.")]
    public VisArgs.Visability Visibility
    {
      get => this._Visibility;
      set => this._Visibility = value;
    }

    public new string ToString()
    {
      string str = "";
      switch (this._Visibility)
      {
        case VisArgs.Visability.Fast:
          str += "-fast ";
          break;
        case VisArgs.Visability.Full:
          str += "-full ";
          break;
      }
      return str;
    }

    public enum Visability : byte
    {
      Fast,
      Normal,
      Full,
    }
  }
}
