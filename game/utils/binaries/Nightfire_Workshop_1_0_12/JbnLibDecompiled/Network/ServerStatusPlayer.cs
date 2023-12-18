// Decompiled with JetBrains decompiler
// Type: JbnLib.Network.ServerStatusPlayer
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.ComponentModel;

namespace JbnLib.Network
{
  public class ServerStatusPlayer
  {
    private string _Name;
    private int _Frags;
    private int _Ping;

    [Category("Player")]
    [Description("Name of the player.")]
    public string Name => this._Name;

    [Category("Player")]
    [Description("Number of kills made by the player.")]
    public int Frags => this._Frags;

    [Category("Player")]
    [Description("The latency of the player as reported by the server.")]
    public int Ping => this._Ping;

    public ServerStatusPlayer()
    {
      this._Name = "?";
      this._Frags = -1;
      this._Ping = -1;
    }

    public ServerStatusPlayer(string name, int frags, int ping)
    {
      this._Name = name;
      this._Frags = frags;
      this._Ping = ping;
    }
  }
}
