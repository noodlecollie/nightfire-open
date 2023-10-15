// Decompiled with JetBrains decompiler
// Type: JbnLib.Log.Types.Score
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.ComponentModel;

namespace JbnLib.Log.Types
{
  [TypeConverter(typeof (ScoreConverter))]
  [Description("Detailed scores of this player.")]
  public class Score
  {
    [Category("Score")]
    [Description("The name of the player.")]
    public string Name;
    private int _Scored;
    private int _TimeConnected;
    private int _TimeReconnect;
    private int _Kills;
    private int _Deaths;
    private int _Suicides;
    private double _KillDeathRatio;
    private double _KillsPerMinute;
    private double _DeathsPerMinute;
    private double _SuicidesPerMinute;
    private int _CTFPoints;
    private int _CTFOffense;
    private int _CTFDefense;
    private int _TimeAccelerator;
    private int _TimeBackPack;
    private int _TimePortableBattery;
    private int _TimeRegenerator;
    private int _TimeSpeed;

    [Category("Score")]
    [Description("How many times this person scored.")]
    public int Scored
    {
      get => this._Scored;
      set => this._Scored = value;
    }

    [Category("Score")]
    [Description("The amount of time (in seconds) this player was connected.")]
    public int TimeConnected
    {
      get => this._TimeConnected;
      set => this._TimeConnected = value;
    }

    [Category("Score")]
    [Description("The amount of time (in seconds) this player spent reconnecting to the server.")]
    public int TimeReconnect
    {
      get => this._TimeReconnect;
      set => this._TimeReconnect = value;
    }

    [Category("Score")]
    [Description("How many kills this player made.")]
    public int Kills
    {
      get => this._Kills;
      set => this._Kills = value;
    }

    [Category("Score")]
    [Description("How many times this player died.")]
    public int Deaths
    {
      get => this._Deaths;
      set => this._Deaths = value;
    }

    [Category("Score")]
    [Description("How many times this player commited suicide.")]
    public int Suicides
    {
      get => this._Suicides;
      set => this._Suicides = value;
    }

    [Category("Score")]
    [Description("How many kills relative to the number of deaths.")]
    public double KillDeathRatio
    {
      get => this._KillDeathRatio;
      set => this._KillDeathRatio = value;
    }

    [Category("Score")]
    [Description("How many kills, on average, this player made per minute.")]
    public double KillsPerMinute
    {
      get => this._KillsPerMinute;
      set => this._KillsPerMinute = value;
    }

    [Category("Score")]
    [Description("How many deaths, on average, this player experienced per minute.")]
    public double DeathsPerMinute
    {
      get => this._DeathsPerMinute;
      set => this._DeathsPerMinute = value;
    }

    [Category("Score")]
    [Description("How many times, on average, this player commited suicide per minute.")]
    public double SuicidesPerMinute
    {
      get => this._SuicidesPerMinute;
      set => this._SuicidesPerMinute = value;
    }

    [Category("Score")]
    [Description("How many flag-related events this player triggered.  This value is weighted depending on the action.")]
    public int CTFPoints
    {
      get => this._CTFPoints;
      set => this._CTFPoints = value;
    }

    [Category("Score")]
    [Description("How many points this player earned by being on the offensive.")]
    public int CTFOffense
    {
      get => this._CTFOffense;
      set => this._CTFOffense = value;
    }

    [Category("Score")]
    [Description("How many points this player earned by being on the defensive.")]
    public int CTFDefense
    {
      get => this._CTFDefense;
      set => this._CTFDefense = value;
    }

    [Category("Score")]
    [Description("The amount of time (in seconds) that this player carried the skull power up.")]
    public int TimeAccelerator
    {
      get => this._TimeAccelerator;
      set => this._TimeAccelerator = value;
    }

    [Category("Score")]
    [Description("The amount of time (in seconds) that this player carried the ammo power up.")]
    public int TimeBackPack
    {
      get => this._TimeBackPack;
      set => this._TimeBackPack = value;
    }

    [Category("Score")]
    [Description("The amount of time (in seconds) that this player carried the battery power up.")]
    public int TimePortableBattery
    {
      get => this._TimePortableBattery;
      set => this._TimePortableBattery = value;
    }

    [Category("Score")]
    [Description("The amount of time (in seconds) that this player carried the armor power up.")]
    public int TimeRegenerator
    {
      get => this._TimeRegenerator;
      set => this._TimeRegenerator = value;
    }

    [Category("Score")]
    [Description("The amount of time (in seconds) that this player carried the shoe power up.")]
    public int TimeSpeed
    {
      get => this._TimeSpeed;
      set => this._TimeSpeed = value;
    }
  }
}
