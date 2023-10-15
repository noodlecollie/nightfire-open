// Decompiled with JetBrains decompiler
// Type: JbnLib.Spr.Frame
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;
using System.Drawing;

namespace JbnLib.Spr
{
  public class Frame
  {
    private int _X;
    private int _Y;
    private int _Width;
    private int _Height;
    private Image _Image;

    public static int Length => 16;

    public int X
    {
      get => this._X;
      set => this._X = value;
    }

    public int Y
    {
      get => this._Y;
      set => this._Y = value;
    }

    public int Width
    {
      get => this._Width;
      set => this._Width = value;
    }

    public int Height
    {
      get => this._Height;
      set => this._Height = value;
    }

    public Image Image
    {
      get => this._Image;
      set => this._Image = value;
    }

    public Frame()
    {
    }

    public Frame(byte[] raw) => this.SetV2(raw);

    public void SetV1(byte[] raw)
    {
      this._X = BitConverter.ToInt32(raw, 0);
      this._Y = BitConverter.ToInt32(raw, 4);
      this._Height = BitConverter.ToInt32(raw, 8);
      this._Width = BitConverter.ToInt32(raw, 12);
    }

    public void SetV2(byte[] raw)
    {
      this._X = BitConverter.ToInt32(raw, 0);
      this._Y = BitConverter.ToInt32(raw, 4);
      this._Width = BitConverter.ToInt32(raw, 8);
      this._Height = BitConverter.ToInt32(raw, 12);
    }

    public byte[] GetBytes() => this.GetV2();

    public byte[] GetV1()
    {
      byte[] v1 = new byte[Frame.Length];
      BitConverter.GetBytes(this._X).CopyTo((Array) v1, 0);
      BitConverter.GetBytes(this._Y).CopyTo((Array) v1, 4);
      BitConverter.GetBytes(this._Width).CopyTo((Array) v1, 12);
      return v1;
    }

    public byte[] GetV2()
    {
      byte[] v2 = new byte[Frame.Length];
      BitConverter.GetBytes(this._X).CopyTo((Array) v2, 0);
      BitConverter.GetBytes(this._Y).CopyTo((Array) v2, 4);
      BitConverter.GetBytes(this._Width).CopyTo((Array) v2, 8);
      BitConverter.GetBytes(this._Height).CopyTo((Array) v2, 12);
      return v2;
    }
  }
}
