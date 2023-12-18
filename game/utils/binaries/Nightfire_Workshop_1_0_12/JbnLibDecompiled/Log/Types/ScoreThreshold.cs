// Decompiled with JetBrains decompiler
// Type: JbnLib.Log.Types.ScoreThreshold
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.ComponentModel;

namespace JbnLib.Log.Types
{
  [TypeConverter(typeof (ScoreThresholdConverter))]
  [Description("Contains a most or least list.")]
  public class ScoreThreshold
  {
    private Variable _Scored;
    private Variable _TimeConnected;
    private Variable _TimeReconnect;
    private Variable _Kills;
    private Variable _Deaths;
    private Variable _Suicides;
    private Variable _KillDeathRatio;
    private Variable _KillsPerMinute;
    private Variable _DeathsPerMinute;
    private Variable _SuicidesPerMinute;
    private Variable _CTFPoints;
    private Variable _CTFOffense;
    private Variable _CTFDefense;
    private Variable _TimeAccelerator;
    private Variable _TimeBackPack;
    private Variable _TimePortableBattery;
    private Variable _TimeRegenerator;
    private Variable _TimeSpeed;

    [Category("Score Threshold")]
    [Description("The name and score of the person who scored the most or least.")]
    public Variable Scored
    {
      get => this._Scored;
      set => this._Scored = value;
    }

    [Category("Score Threshold")]
    [Description("The name and score of the person who was connected the most or least.")]
    public Variable TimeConnected
    {
      get => this._TimeConnected;
      set => this._TimeConnected = value;
    }

    [Category("Score Threshold")]
    [Description("The name and score of the person who spent the most or least time reconnecting.")]
    public Variable TimeReconnect
    {
      get => this._TimeReconnect;
      set => this._TimeReconnect = value;
    }

    [Category("Score Threshold")]
    [Description("The name and score of the person who has the most or least number of kills.")]
    public Variable Kills
    {
      get => this._Kills;
      set => this._Kills = value;
    }

    [Category("Score Threshold")]
    [Description("The name and score of the person who has the most or least number of deaths.")]
    public Variable Deaths
    {
      get => this._Deaths;
      set => this._Deaths = value;
    }

    [Category("Score Threshold")]
    [Description("The name and score of the person who has the most or least number of suicides.")]
    public Variable Suicides
    {
      get => this._Suicides;
      set => this._Suicides = value;
    }

    [Category("Score Threshold")]
    [Description("The name and score of the person who has the highest or lowest kill/death ratio.")]
    public Variable KillDeathRatio
    {
      get => this._KillDeathRatio;
      set => this._KillDeathRatio = value;
    }

    [Category("Score Threshold")]
    [Description("The name and score of the person who has the highest or lowest kill per minute.")]
    public Variable KillsPerMinute
    {
      get => this._KillsPerMinute;
      set => this._KillsPerMinute = value;
    }

    [Category("Score Threshold")]
    [Description("The name and score of the person who has the highest or lowest deaths per minute.")]
    public Variable DeathsPerMinute
    {
      get => this._DeathsPerMinute;
      set => this._DeathsPerMinute = value;
    }

    [Category("Score Threshold")]
    [Description("The name and score of the person who has the highest or lowest suicides per minute.")]
    public Variable SuicidesPerMinute
    {
      get => this._SuicidesPerMinute;
      set => this._SuicidesPerMinute = value;
    }

    [Category("Score Threshold")]
    [Description("The name and score of the person who has the most or least CTF points.")]
    public Variable CTFPoints
    {
      get => this._CTFPoints;
      set => this._CTFPoints = value;
    }

    [Category("Score Threshold")]
    [Description("The name and score of the person who has the most or least CTF offensive points.")]
    public Variable CTFOffense
    {
      get => this._CTFOffense;
      set => this._CTFOffense = value;
    }

    [Category("Score Threshold")]
    [Description("The name and score of the person who has the most or least CTF defensive points.")]
    public Variable CTFDefense
    {
      get => this._CTFDefense;
      set => this._CTFDefense = value;
    }

    [Category("Score Threshold")]
    [Description("The name and score of the person who held the skull power up the longest or shortest time.")]
    public Variable TimeAccelerator
    {
      get => this._TimeAccelerator;
      set => this._TimeAccelerator = value;
    }

    [Category("Score Threshold")]
    [Description("The name and score of the person who held the ammo power up the longest or shortest time.")]
    public Variable TimeBackPack
    {
      get => this._TimeBackPack;
      set => this._TimeBackPack = value;
    }

    [Category("Score Threshold")]
    [Description("The name and score of the person who held the battery power up the longest or shortest time.")]
    public Variable TimePortableBattery
    {
      get => this._TimePortableBattery;
      set => this._TimePortableBattery = value;
    }

    [Category("Score Threshold")]
    [Description("The name and score of the person who held the armor power up the longest or shortest time.")]
    public Variable TimeRegenerator
    {
      get => this._TimeRegenerator;
      set => this._TimeRegenerator = value;
    }

    [Category("Score Threshold")]
    [Description("The name and score of the person who held the shoe power up the longest or shortest time.")]
    public Variable TimeSpeed
    {
      get => this._TimeSpeed;
      set => this._TimeSpeed = value;
    }

    public ScoreThreshold()
    {
      this.Scored = new Variable("*Nobody*", 0);
      this.TimeConnected = new Variable("*Nobody*", 0);
      this.TimeReconnect = new Variable("*Nobody*", 0);
      this.Kills = new Variable("*Nobody*", 0);
      this.Deaths = new Variable("*Nobody*", 0);
      this.Suicides = new Variable("*Nobody*", 0);
      this.KillDeathRatio = new Variable("*Nobody*", 0);
      this.KillsPerMinute = new Variable("*Nobody*", 0);
      this.DeathsPerMinute = new Variable("*Nobody*", 0);
      this.SuicidesPerMinute = new Variable("*Nobody*", 0);
      this.CTFPoints = new Variable("*Nobody*", 0);
      this.CTFOffense = new Variable("*Nobody*", 0);
      this.CTFDefense = new Variable("*Nobody*", 0);
      this.TimeAccelerator = new Variable("*Nobody*", 0);
      this.TimeBackPack = new Variable("*Nobody*", 0);
      this.TimePortableBattery = new Variable("*Nobody*", 0);
      this.TimeRegenerator = new Variable("*Nobody*", 0);
      this.TimeSpeed = new Variable("*Nobody*", 0);
    }
  }
}
