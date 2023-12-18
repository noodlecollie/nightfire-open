// Decompiled with JetBrains decompiler
// Type: JbnLib.Network.ServerStatus
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;
using System.Collections.Generic;
using System.ComponentModel;
using System.Net;

namespace JbnLib.Network
{
  [DefaultProperty("HostName")]
  public class ServerStatus
  {
    private IPEndPoint _EndPointQuery;
    private IPEndPoint _EndPointHost;
    private string _GameName;
    private string _GameVer;
    private bool _Dedicated;
    private string _GameMode;
    private string _GameType;
    private ushort _HostPort;
    private string _MapName;
    private byte _MaxPlayers;
    private string _HostName;
    private byte _NumPlayers;
    private bool _Password;
    private int _TimeLimit;
    private int _FragLimit;
    private int _CTFLimit;
    private bool _TeamPlay;
    private bool _FriendlyFire;
    private bool _WeaponStay;
    private byte _BotsMode;
    private List<ServerStatusPlayer> _Players = new List<ServerStatusPlayer>();
    private float _QueryID;
    private int _Ping = -1;

    [Category("Connection")]
    [Description("This represents the end point being used to connect to the server's query port.")]
    public IPEndPoint EndPointQuery
    {
      get => this._EndPointQuery;
      set => this._EndPointQuery = value;
    }

    [Category("Connection")]
    [Description("This represents the end point used to connect someone to the server.  This value is updated at the same time the host port is discovered.")]
    public IPEndPoint EndPointHost
    {
      get => this._EndPointHost;
      set => this._EndPointHost = value;
    }

    [Category("Advanced")]
    [Description("This is used by GameSpy to identify how to handle the status query results.")]
    public string GameName
    {
      get => this._GameName;
      set => this._GameName = value;
    }

    [Category("Advanced")]
    [Description("The version of the game as reported by the host.")]
    public string GameVer
    {
      get => this._GameVer;
      set => this._GameVer = value;
    }

    [Category("Basic")]
    [Description("True means that the game is hosted without 3D graphics enabled, false means the host is present in the server.")]
    public bool Dedicated
    {
      get => this._Dedicated;
      set => this._Dedicated = value;
    }

    [Category("Advanced")]
    [Description("States the current mode of the server.")]
    public string GameMode
    {
      get => this._GameMode;
      set => this._GameMode = value;
    }

    [Category("Basic")]
    [Description("The type of game being played be it deathmatch or capture the flag.")]
    public string GameType
    {
      get => this._GameType;
      set => this._GameType = value;
    }

    [Category("Connection")]
    [Description("The port number players actually connect to when playing in this server.")]
    public ushort HostPort
    {
      get => this._HostPort;
      set => this._HostPort = value;
    }

    [Category("Basic")]
    [Description("How many players are currently in the server.")]
    public string MapName
    {
      get => this._MapName;
      set => this._MapName = value;
    }

    [Category("Basic")]
    [Description("The maximum number of players allowed in the server.")]
    public byte MaxPlayers
    {
      get => this._MaxPlayers;
      set => this._MaxPlayers = value;
    }

    [Category("Basic")]
    [Description("The name of the server as it appears in server browsers.")]
    public string HostName
    {
      get => this._HostName;
      set => this._HostName = value;
    }

    [Category("Basic")]
    [Description("How many players are currently in the server.")]
    public byte NumPlayers
    {
      get => this._NumPlayers;
      set => this._NumPlayers = value;
    }

    [Category("Basic")]
    [Description("True if there is a password in place; false if there is not.")]
    public bool Password
    {
      get => this._Password;
      set => this._Password = value;
    }

    [Category("Settings")]
    [Description("Given in minutes.  When the counter reaches zero, the game is ended.")]
    public int TimeLimit
    {
      get => this._TimeLimit;
      set => this._TimeLimit = value;
    }

    [Category("Settings")]
    [Description("The number of kills a player needs to make before a winner is declared.")]
    public int FragLimit
    {
      get => this._FragLimit;
      set => this._FragLimit = value;
    }

    [Category("Settings")]
    [Description("How many flags need to be captured by one team to determine the winner.")]
    public int CTFLimit
    {
      get => this._CTFLimit;
      set => this._CTFLimit = value;
    }

    [Category("Settings")]
    [Description("True means two teams will be created and false means it is a free for all.")]
    public bool TeamPlay
    {
      get => this._TeamPlay;
      set => this._TeamPlay = value;
    }

    [Category("Settings")]
    [Description("True means that players on the same team can hurt each other; false means they cannot.")]
    public bool FriendlyFire
    {
      get => this._FriendlyFire;
      set => this._FriendlyFire = value;
    }

    [Category("Settings")]
    [Description("False means weapons are picked up and respawn later, true means weapons are always available but they cannot be used to supply ammo.")]
    public bool WeaponStay
    {
      get => this._WeaponStay;
      set => this._WeaponStay = value;
    }

    [Category("Settings")]
    [Description("0 means no bots are present in the server, 1 means bots may or may not be present in the server, and 2 means up to 12 bots are continously held in the server.")]
    public byte BotsMode
    {
      get => this._BotsMode;
      set => this._BotsMode = value;
    }

    [Browsable(false)]
    public List<ServerStatusPlayer> Players
    {
      get => this._Players;
      set => this._Players = value;
    }

    [Category("Connection")]
    [Description("Uniquely indentifies this query.")]
    public float QueryID
    {
      get => this._QueryID;
      set => this._QueryID = value;
    }

    [Category("Connection")]
    [Description("How long it took the server to respond to the query in milliseconds.")]
    public int Ping
    {
      get => this._Ping;
      set => this._Ping = value;
    }

    public ServerStatus()
    {
    }

    public ServerStatus(IPEndPoint ip)
    {
      ServerStatus serverStatus = StaticMethods.GetServerStatus(ip);
      this._BotsMode = serverStatus.BotsMode;
      this._CTFLimit = serverStatus.CTFLimit;
      this._Dedicated = serverStatus.Dedicated;
      this._EndPointHost = serverStatus.EndPointHost;
      this._EndPointQuery = serverStatus.EndPointQuery;
      this._FragLimit = serverStatus.FragLimit;
      this._FriendlyFire = serverStatus.FriendlyFire;
      this._GameMode = serverStatus.GameMode;
      this._GameName = serverStatus.GameName;
      this._GameType = serverStatus.GameType;
      this._GameVer = serverStatus.GameVer;
      this._HostName = serverStatus.HostName;
      this._HostPort = serverStatus.HostPort;
      this._Ping = serverStatus.Ping;
      this._MapName = serverStatus.MapName;
      this._MaxPlayers = serverStatus.MaxPlayers;
      this._NumPlayers = serverStatus.NumPlayers;
      this._Password = serverStatus.Password;
      this._Players = serverStatus.Players;
      this._QueryID = serverStatus.QueryID;
      this._TeamPlay = serverStatus.TeamPlay;
      this._TimeLimit = serverStatus.TimeLimit;
      this._WeaponStay = serverStatus.WeaponStay;
    }

    public new string ToString()
    {
      if (this._EndPointHost == null)
        return "";
      return this._HostName + " (" + this._EndPointHost.ToString() + ") " + (object) this._Ping + " ms";
    }
  }
}
