// Decompiled with JetBrains decompiler
// Type: JbnLib.Sp2.Frame
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;
using System.Text;

namespace JbnLib.Sp2
{
  public class Frame
  {
    private int _Width;
    private int _Height;
    private int _X;
    private int _Y;
    private string _Image;

    public static int Length => 80;

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

    public string Image
    {
      get => this._Image;
      set => this._Image = value;
    }

    public Frame()
    {
    }

    public Frame(byte[] raw)
    {
      this._Width = BitConverter.ToInt32(raw, 0);
      this._Height = BitConverter.ToInt32(raw, 4);
      this._X = BitConverter.ToInt32(raw, 8);
      this._Y = BitConverter.ToInt32(raw, 12);
      this._Image = Encoding.ASCII.GetString(raw, 16, 64).TrimEnd(new char[1]);
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[Frame.Length];
      BitConverter.GetBytes(this._Width).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._Height).CopyTo((Array) bytes, 4);
      BitConverter.GetBytes(this._X).CopyTo((Array) bytes, 8);
      BitConverter.GetBytes(this._Y).CopyTo((Array) bytes, 12);
      if (this._Image.Length > 64)
        this._Image = this._Image.Substring(0, 64);
      Encoding.ASCII.GetBytes(this._Image).CopyTo((Array) bytes, 16);
      return bytes;
    }
  }
}
