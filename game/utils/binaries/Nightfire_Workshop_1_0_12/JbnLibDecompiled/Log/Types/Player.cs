// Decompiled with JetBrains decompiler
// Type: JbnLib.Log.Types.Player
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Log.Types
{
  public class Player
  {
    private bool _IsConnecting;
    private bool _InGame;
    private bool _HasFlag;
    private bool _IsBot;
    private string _Name;
    private string _IP = "";
    private Team _Team = Team.Unknown;
    private string _Character = "";
    private int _Wins;
    private int _Losses;
    private int _Points;
    private PowerUp _PowerUp = PowerUp.None;
    private int _PlayerID;
    private long _WonID;

    public bool IsConnecting
    {
      get => this._IsConnecting;
      set => this._IsConnecting = value;
    }

    public bool InGame
    {
      get => this._InGame;
      set => this._InGame = value;
    }

    public bool HasFlag
    {
      get => this._HasFlag;
      set => this._HasFlag = value;
    }

    public bool IsBot
    {
      get => this._IsBot;
      set => this._IsBot = value;
    }

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public string IP
    {
      get => this._IP;
      set => this._IP = value;
    }

    public Team Team
    {
      get => this._Team;
      set => this._Team = value;
    }

    public string Character
    {
      get => this._Character;
      set => this._Character = value;
    }

    public int Wins
    {
      get => this._Wins;
      set => this._Wins = value;
    }

    public int Losses
    {
      get => this._Losses;
      set => this._Losses = value;
    }

    public int Points
    {
      get => this._Points;
      set => this._Points = value;
    }

    public PowerUp PowerUp
    {
      get => this._PowerUp;
      set => this._PowerUp = value;
    }

    public int PlayerID
    {
      get => this._PlayerID;
      set => this._PlayerID = value;
    }

    public long WonID => this._WonID;

    public Player(string name, int playerid, long wonid, Team team)
    {
      this._Name = name;
      this._Team = team;
      this._PlayerID = playerid;
      this._WonID = wonid;
      if (wonid != 0L)
        return;
      this._IsBot = true;
    }

    public new string ToString() => this._Name;
  }
}
