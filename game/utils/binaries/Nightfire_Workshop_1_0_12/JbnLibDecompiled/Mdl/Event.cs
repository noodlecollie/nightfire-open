// Decompiled with JetBrains decompiler
// Type: JbnLib.Mdl.Event
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Interfaces;
using System;
using System.Text;

namespace JbnLib.Mdl
{
  [Serializable]
  public class Event : IStructure
  {
    private int _Frame;
    private int _Event;
    private int _Type;
    private string _Options;

    public static int Length => 76;

    public int Frame
    {
      get => this._Frame;
      set => this._Frame = value;
    }

    public int EventValue
    {
      get => this._Event;
      set => this._Event = value;
    }

    public int Type
    {
      get => this._Type;
      set => this._Type = value;
    }

    public string Options
    {
      get => this._Options;
      set => this._Options = value;
    }

    public Event(byte[] raw)
    {
      this._Frame = BitConverter.ToInt32(raw, 0);
      this._Event = BitConverter.ToInt32(raw, 4);
      this._Type = BitConverter.ToInt32(raw, 8);
      this._Options = Encoding.ASCII.GetString(raw, 12, 64).Trim(new char[1]);
    }

    public Event(int frame, int @event, int type, string options)
    {
      this._Frame = frame;
      this._Event = @event;
      this._Type = type;
      this._Options = options;
    }

    public byte[] GetBytes()
    {
      byte[] bytes = new byte[Event.Length];
      BitConverter.GetBytes(this._Frame).CopyTo((Array) bytes, 0);
      BitConverter.GetBytes(this._Event).CopyTo((Array) bytes, 4);
      BitConverter.GetBytes(this._Type).CopyTo((Array) bytes, 8);
      Encoding.ASCII.GetBytes(this._Options).CopyTo((Array) bytes, 12);
      return bytes;
    }

    public new string ToString() => this._Frame.ToString() + ", " + (object) this._Event;
  }
}
