// Decompiled with JetBrains decompiler
// Type: JbnLib.Log.LogFile
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using JbnLib.Log.Collections;
using JbnLib.Log.Types;
using JbnLib.Log.Types.DateData;
using JbnLib.Log.Types.DateID;
using JbnLib.Log.Types.DateIDTeam;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Globalization;
using System.IO;
using System.Text;
using System.Threading;

namespace JbnLib.Log
{
  public class LogFile
  {
    public const string NOBODY = "*Nobody*";
    private readonly CultureInfo CULTURE = new CultureInfo("en-US");
    private readonly Encoding ENCODING = Encoding.UTF8;
    private PlayerCollection _Players;
    private DateTime _TimeOpened;
    private DateTime _TimeClosed;
    private string _OriginalFilename;
    private string _Filename;
    private string _Game;
    private string _Version;
    private string _Map;
    private int _CRC;
    private Team _WinningTeam;
    private SortableBindingList<Variable> _Variables = new SortableBindingList<Variable>();
    private SortableBindingList<VariableChanged> _VariableChanges = new SortableBindingList<VariableChanged>();
    private SortableBindingList<JbnLib.Log.Types.RCon> _RCon = new SortableBindingList<JbnLib.Log.Types.RCon>();
    private SortableBindingList<JbnLib.Log.Types.Say.SayServer> _SayServer = new SortableBindingList<JbnLib.Log.Types.Say.SayServer>();
    private SortableBindingList<JbnLib.Log.Types.Say.SayPhoenix> _SayPhoenix = new SortableBindingList<JbnLib.Log.Types.Say.SayPhoenix>();
    private SortableBindingList<JbnLib.Log.Types.Say.SayMI6> _SayMI6 = new SortableBindingList<JbnLib.Log.Types.Say.SayMI6>();
    private SortableBindingList<JbnLib.Log.Types.Say.SayGeneral> _SayGeneral = new SortableBindingList<JbnLib.Log.Types.Say.SayGeneral>();
    private SortableBindingList<JbnLib.Log.Types.DateIDTeam.JoinedTeam> _JoinedTeam = new SortableBindingList<JbnLib.Log.Types.DateIDTeam.JoinedTeam>();
    private SortableBindingList<JbnLib.Log.Types.DateID.ChangedRole> _ChangedRole = new SortableBindingList<JbnLib.Log.Types.DateID.ChangedRole>();
    private SortableBindingList<JbnLib.Log.Types.DateID.ChangedName> _ChangedName = new SortableBindingList<JbnLib.Log.Types.DateID.ChangedName>();
    private SortableBindingList<Killed> _Killed = new SortableBindingList<Killed>();
    private SortableBindingList<Suicide> _Suicides = new SortableBindingList<Suicide>();
    private SortableBindingList<TriggeredEvent> _Triggered = new SortableBindingList<TriggeredEvent>();
    private SortableBindingList<TriggeredAgainstEvent> _TriggeredAgainst = new SortableBindingList<TriggeredAgainstEvent>();
    private SortableBindingList<WorldTriggered> _WorldTriggered = new SortableBindingList<WorldTriggered>();
    private SortableBindingList<JbnLib.Log.Types.DateID.Connected> _Connected = new SortableBindingList<JbnLib.Log.Types.DateID.Connected>();
    private SortableBindingList<JbnLib.Log.Types.DateID.EnteredGame> _EnteredGame = new SortableBindingList<JbnLib.Log.Types.DateID.EnteredGame>();
    private SortableBindingList<JbnLib.Log.Types.DateID.Disconnected> _Disconnected = new SortableBindingList<JbnLib.Log.Types.DateID.Disconnected>();
    public Score[] ScoreMI6;
    public Score[] ScorePhoenix;
    public Score[] ScoreOther;
    public Score[] ReconnectMI6;
    public Score[] ReconnectPhoenix;
    public Score[] ReconnectOther;
    public ScoreThreshold MostMI6 = new ScoreThreshold();
    public ScoreThreshold LeastMI6 = new ScoreThreshold();
    public ScoreThreshold MostPhoenix = new ScoreThreshold();
    public ScoreThreshold LeastPhoenix = new ScoreThreshold();
    public ScoreThreshold MostOther = new ScoreThreshold();
    public ScoreThreshold LeastOther = new ScoreThreshold();
    public ScoreThreshold MostOverall = new ScoreThreshold();
    public ScoreThreshold LeastOverall = new ScoreThreshold();
    private bool _Loop = true;
    private bool _IsStopped = true;
    private volatile bool _Stop;
    private Thread _Looper;
    private int _Delay = 500;
    private SortableBindingList<string> _Errors = new SortableBindingList<string>();
    private SortableBindingList<string> _Unknown = new SortableBindingList<string>();

    public LogFile(string path)
    {
      Messages.SetEnglish();
      this._Filename = path;
      this._Players = new PlayerCollection();
      this._Players.Insert(0, new Player("Server", -1, -1L, Team.NotApplicable));
    }

    public LogFile(string path, bool loop)
    {
      Messages.SetEnglish();
      this._Filename = path;
      this._Players = new PlayerCollection();
      this._Players.Insert(0, new Player("Server", -1, -1L, Team.NotApplicable));
      this._Loop = loop;
    }

    public event MapChangedHandler MapChanged;

    public event ServerShutdownHandler ServerShutdown;

    public PlayerCollection Players => this._Players;

    public DateTime TimeOpened => this._TimeOpened;

    public DateTime TimeClosed => this._TimeClosed;

    public string OriginalFilename => this._OriginalFilename;

    public string Filename => this._Filename;

    public string Game => this._Game;

    public string Version => this._Version;

    public string Map => this._Map;

    public int CRC => this._CRC;

    public Team WinningTeam => this._WinningTeam;

    public SortableBindingList<Variable> Variables => this._Variables;

    public Variable[] GetVariables()
    {
      Variable[] array = new Variable[this._Variables.Count];
      this._Variables.CopyTo(array, 0);
      return array;
    }

    public SortableBindingList<VariableChanged> VariableChanges => this._VariableChanges;

    public VariableChanged[] GetVariableChanges()
    {
      VariableChanged[] array = new VariableChanged[this._VariableChanges.Count];
      this._VariableChanges.CopyTo(array, 0);
      return array;
    }

    public VariableChanged[] GetVariableChangeList(string name)
    {
      SortableBindingList<VariableChanged> sortableBindingList = new SortableBindingList<VariableChanged>();
      foreach (VariableChanged variableChange in (Collection<VariableChanged>) this._VariableChanges)
      {
        if (variableChange.Name == name)
          sortableBindingList.Add(variableChange);
      }
      VariableChanged[] array = new VariableChanged[sortableBindingList.Count];
      sortableBindingList.CopyTo(array, 0);
      return array;
    }

    public SortableBindingList<JbnLib.Log.Types.RCon> RCon => this._RCon;

    public JbnLib.Log.Types.RCon[] GetRCon()
    {
      JbnLib.Log.Types.RCon[] array = new JbnLib.Log.Types.RCon[this._RCon.Count];
      this._RCon.CopyTo(array, 0);
      return array;
    }

    public SortableBindingList<JbnLib.Log.Types.Say.SayServer> SayServer => this._SayServer;

    public JbnLib.Log.Types.Say.SayServer[] GetSayServer()
    {
      JbnLib.Log.Types.Say.SayServer[] array = new JbnLib.Log.Types.Say.SayServer[this._SayServer.Count];
      this._SayServer.CopyTo(array, 0);
      return array;
    }

    public SortableBindingList<JbnLib.Log.Types.Say.SayPhoenix> SayPhoenix => this._SayPhoenix;

    public JbnLib.Log.Types.Say.SayPhoenix[] GetSayPhoenix()
    {
      JbnLib.Log.Types.Say.SayPhoenix[] array = new JbnLib.Log.Types.Say.SayPhoenix[this._SayPhoenix.Count];
      this._SayPhoenix.CopyTo(array, 0);
      return array;
    }

    public SortableBindingList<JbnLib.Log.Types.Say.SayMI6> SayMI6 => this._SayMI6;

    public JbnLib.Log.Types.Say.SayMI6[] GetSayMI6()
    {
      JbnLib.Log.Types.Say.SayMI6[] array = new JbnLib.Log.Types.Say.SayMI6[this._SayMI6.Count];
      this._SayMI6.CopyTo(array, 0);
      return array;
    }

    public SortableBindingList<JbnLib.Log.Types.Say.SayGeneral> SayGeneral => this._SayGeneral;

    public JbnLib.Log.Types.Say.SayGeneral[] GetSayGeneral()
    {
      JbnLib.Log.Types.Say.SayGeneral[] array = new JbnLib.Log.Types.Say.SayGeneral[this._SayGeneral.Count];
      this._SayGeneral.CopyTo(array, 0);
      return array;
    }

    public SortableBindingList<JbnLib.Log.Types.DateIDTeam.JoinedTeam> JoinedTeam => this._JoinedTeam;

    public JbnLib.Log.Types.DateIDTeam.JoinedTeam[] GetJoinedTeams()
    {
      JbnLib.Log.Types.DateIDTeam.JoinedTeam[] array = new JbnLib.Log.Types.DateIDTeam.JoinedTeam[this._JoinedTeam.Count];
      this._JoinedTeam.CopyTo(array, 0);
      return array;
    }

    public SortableBindingList<JbnLib.Log.Types.DateID.ChangedRole> ChangedRole => this._ChangedRole;

    public JbnLib.Log.Types.DateID.ChangedRole[] GetChangedRoles()
    {
      JbnLib.Log.Types.DateID.ChangedRole[] array = new JbnLib.Log.Types.DateID.ChangedRole[this._ChangedRole.Count];
      this._ChangedRole.CopyTo(array, 0);
      return array;
    }

    public SortableBindingList<JbnLib.Log.Types.DateID.ChangedName> ChangedName => this._ChangedName;

    public JbnLib.Log.Types.DateID.ChangedName[] GetChangedNames()
    {
      JbnLib.Log.Types.DateID.ChangedName[] array = new JbnLib.Log.Types.DateID.ChangedName[this._ChangedName.Count];
      this._ChangedName.CopyTo(array, 0);
      return array;
    }

    public SortableBindingList<Killed> Kills => this._Killed;

    public Killed[] GetKills()
    {
      Killed[] array = new Killed[this._Killed.Count];
      this._Killed.CopyTo(array, 0);
      return array;
    }

    public Killed[] GetKillList(int id)
    {
      SortableBindingList<Killed> sortableBindingList = new SortableBindingList<Killed>();
      foreach (Killed killed in (Collection<Killed>) this._Killed)
      {
        if (killed.ID == id)
          sortableBindingList.Add(killed);
      }
      Killed[] array = new Killed[sortableBindingList.Count];
      sortableBindingList.CopyTo(array, 0);
      return array;
    }

    public Killed[] GetKillList(Team team)
    {
      SortableBindingList<Killed> sortableBindingList = new SortableBindingList<Killed>();
      foreach (Killed killed in (Collection<Killed>) this._Killed)
      {
        if (killed.Team == team)
          sortableBindingList.Add(killed);
      }
      Killed[] array = new Killed[sortableBindingList.Count];
      sortableBindingList.CopyTo(array, 0);
      return array;
    }

    public int GetKillCount(int id) => this.GetKillList(id).Length;

    public Killed[] GetVictimList(int id)
    {
      SortableBindingList<Killed> sortableBindingList = new SortableBindingList<Killed>();
      foreach (Killed killed in (Collection<Killed>) this._Killed)
      {
        if (killed.VictimID == id)
          sortableBindingList.Add(killed);
      }
      Killed[] array = new Killed[sortableBindingList.Count];
      sortableBindingList.CopyTo(array, 0);
      return array;
    }

    public int GetDeathCount(int id) => this.GetVictimList(id).Length;

    public Killed[] GetTeamKillList(int id)
    {
      SortableBindingList<Killed> sortableBindingList = new SortableBindingList<Killed>();
      foreach (Killed killed in (Collection<Killed>) this._Killed)
      {
        if (killed.ID == id && killed.TeamKill)
          sortableBindingList.Add(killed);
      }
      Killed[] array = new Killed[sortableBindingList.Count];
      sortableBindingList.CopyTo(array, 0);
      return array;
    }

    public Killed[] GetTeamKillList(Team team)
    {
      SortableBindingList<Killed> sortableBindingList = new SortableBindingList<Killed>();
      foreach (Killed killed in (Collection<Killed>) this._Killed)
      {
        if (killed.Team == team && killed.TeamKill)
          sortableBindingList.Add(killed);
      }
      Killed[] array = new Killed[sortableBindingList.Count];
      sortableBindingList.CopyTo(array, 0);
      return array;
    }

    public int GetTeamKillCount(int id) => this.GetTeamKillList(id).Length;

    public SortableBindingList<Suicide> Suicides => this._Suicides;

    public Suicide[] GetSuicides()
    {
      Suicide[] array = new Suicide[this._Suicides.Count];
      this._Suicides.CopyTo(array, 0);
      return array;
    }

    public Suicide[] GetSuicideList(int id)
    {
      SortableBindingList<Suicide> sortableBindingList = new SortableBindingList<Suicide>();
      foreach (Suicide suicide in (Collection<Suicide>) this._Suicides)
      {
        if (suicide.ID == id)
          sortableBindingList.Add(suicide);
      }
      Suicide[] array = new Suicide[sortableBindingList.Count];
      sortableBindingList.CopyTo(array, 0);
      return array;
    }

    public int GetSuicideCount(int id) => this.GetSuicideList(id).Length;

    public SortableBindingList<TriggeredEvent> TriggeredEvents => this._Triggered;

    public TriggeredEvent[] GetTriggeredEvents()
    {
      TriggeredEvent[] array = new TriggeredEvent[this._Triggered.Count];
      this._Triggered.CopyTo(array, 0);
      return array;
    }

    public int GetPowerupTime(int id, string powerup)
    {
      SortableBindingList<TriggeredEvent> sortableBindingList = new SortableBindingList<TriggeredEvent>();
      foreach (TriggeredEvent triggeredEvent in (Collection<TriggeredEvent>) this._Triggered)
      {
        if (triggeredEvent.ID == id && (triggeredEvent.Event == "take_" + powerup + "_Powerup" || triggeredEvent.Event == "drop_" + powerup + "_Powerup"))
          sortableBindingList.Add(triggeredEvent);
      }
      if (sortableBindingList.Count % 2 != 0)
        sortableBindingList.Add(new TriggeredEvent(this._TimeClosed, id, this._Players[id].Team, "drop_" + powerup + "_Powerup"));
      int powerupTime = 0;
      for (int index = sortableBindingList.Count - 1; index >= 0; index -= 2)
      {
        TimeSpan timeSpan = sortableBindingList[index].Date - sortableBindingList[index - 1].Date;
        powerupTime += timeSpan.Hours * 3600 + timeSpan.Minutes * 60 + timeSpan.Seconds;
      }
      return powerupTime;
    }

    public int GetTriggerCount(int id, string trigger)
    {
      SortableBindingList<TriggeredEvent> sortableBindingList = new SortableBindingList<TriggeredEvent>();
      foreach (TriggeredEvent triggeredEvent in (Collection<TriggeredEvent>) this._Triggered)
      {
        if (triggeredEvent.ID == id && triggeredEvent.Event == trigger)
          sortableBindingList.Add(triggeredEvent);
      }
      return sortableBindingList.Count;
    }

    public int GetTriggerCount(Team team, string trigger)
    {
      int triggerCount = 0;
      foreach (TriggeredEvent triggeredEvent in (Collection<TriggeredEvent>) this._Triggered)
      {
        if (triggeredEvent.Team == team && triggeredEvent.Event == trigger)
          ++triggerCount;
      }
      return triggerCount;
    }

    public SortableBindingList<TriggeredAgainstEvent> TriggeredAgainstEvents => this._TriggeredAgainst;

    public TriggeredAgainstEvent[] GetTriggeredAgainstEvents()
    {
      TriggeredAgainstEvent[] array = new TriggeredAgainstEvent[this._TriggeredAgainst.Count];
      this._TriggeredAgainst.CopyTo(array, 0);
      return array;
    }

    public int GetTriggerAgainstCount(int id, string trigger)
    {
      SortableBindingList<TriggeredAgainstEvent> sortableBindingList = new SortableBindingList<TriggeredAgainstEvent>();
      foreach (TriggeredAgainstEvent triggeredAgainstEvent in (Collection<TriggeredAgainstEvent>) this._TriggeredAgainst)
      {
        if (triggeredAgainstEvent.ID == id && triggeredAgainstEvent.Event == trigger)
          sortableBindingList.Add(triggeredAgainstEvent);
      }
      return sortableBindingList.Count;
    }

    public SortableBindingList<WorldTriggered> WorldTriggeredEvents => this._WorldTriggered;

    public WorldTriggered[] GetWorldTriggeredEvents()
    {
      WorldTriggered[] array = new WorldTriggered[this._WorldTriggered.Count];
      this._WorldTriggered.CopyTo(array, 0);
      return array;
    }

    public SortableBindingList<JbnLib.Log.Types.DateID.Connected> Connected => this._Connected;

    public JbnLib.Log.Types.DateID.Connected[] GetConnects()
    {
      JbnLib.Log.Types.DateID.Connected[] array = new JbnLib.Log.Types.DateID.Connected[this._Connected.Count];
      this._Connected.CopyTo(array, 0);
      return array;
    }

    public SortableBindingList<JbnLib.Log.Types.DateID.EnteredGame> EnteredGame => this._EnteredGame;

    public JbnLib.Log.Types.DateID.EnteredGame[] GetEnteredGames()
    {
      JbnLib.Log.Types.DateID.EnteredGame[] array = new JbnLib.Log.Types.DateID.EnteredGame[this._EnteredGame.Count];
      this._EnteredGame.CopyTo(array, 0);
      return array;
    }

    public SortableBindingList<JbnLib.Log.Types.DateID.Disconnected> Disconnected => this._Disconnected;

    public JbnLib.Log.Types.DateID.Disconnected[] GetDisconnects()
    {
      JbnLib.Log.Types.DateID.Disconnected[] array = new JbnLib.Log.Types.DateID.Disconnected[this._Disconnected.Count];
      this._Disconnected.CopyTo(array, 0);
      return array;
    }

    public int GetConnectedTime(int id)
    {
      BindingList<DateTime> bindingList = new BindingList<DateTime>();
      foreach (JbnLib.Log.Types.DateID.EnteredGame enteredGame in (Collection<JbnLib.Log.Types.DateID.EnteredGame>) this._EnteredGame)
      {
        if (enteredGame.ID == id)
          bindingList.Add(enteredGame.Date);
      }
      foreach (JbnLib.Log.Types.DateID.Disconnected disconnected in (Collection<JbnLib.Log.Types.DateID.Disconnected>) this._Disconnected)
      {
        if (disconnected.ID == id)
          bindingList.Add(disconnected.Date);
      }
      if (bindingList.Count % 2 != 0)
        bindingList.Add(this._TimeClosed);
      DateTime[] array = new DateTime[bindingList.Count];
      bindingList.CopyTo(array, 0);
      Array.Sort<DateTime>(array);
      int connectedTime = 0;
      for (int index = array.Length - 1; index >= 0; index -= 2)
      {
        TimeSpan timeSpan = array[index] - array[index - 1];
        connectedTime += timeSpan.Hours * 3600 + timeSpan.Minutes * 60 + timeSpan.Seconds;
      }
      return connectedTime;
    }

    public int GetReconnectTime(int id)
    {
      BindingList<DateTime> bindingList = new BindingList<DateTime>();
      foreach (JbnLib.Log.Types.DateID.EnteredGame enteredGame in (Collection<JbnLib.Log.Types.DateID.EnteredGame>) this._EnteredGame)
      {
        if (enteredGame.ID == id)
          bindingList.Add(enteredGame.Date);
      }
      foreach (JbnLib.Log.Types.DateID.Disconnected disconnected in (Collection<JbnLib.Log.Types.DateID.Disconnected>) this._Disconnected)
      {
        if (disconnected.ID == id)
          bindingList.Add(disconnected.Date);
      }
      if (bindingList.Count % 2 != 0)
        bindingList.Add(this._TimeClosed);
      DateTime[] array = new DateTime[bindingList.Count];
      bindingList.CopyTo(array, 0);
      Array.Sort<DateTime>(array);
      int reconnectTime = 0;
      for (int index = array.Length - 2; index > 0; index -= 2)
      {
        TimeSpan timeSpan = array[index] - array[index - 1];
        reconnectTime += timeSpan.Hours * 3600 + timeSpan.Minutes * 60 + timeSpan.Seconds;
      }
      return reconnectTime;
    }

    private Score AddReconnect(Score input)
    {
      input.Kills = Convert.ToInt32(Math.Round(input.KillsPerMinute * (double) input.TimeReconnect, 0));
      input.Deaths = Convert.ToInt32(Math.Round(input.DeathsPerMinute * (double) input.TimeReconnect, 0));
      input.Suicides = Convert.ToInt32(Math.Round(input.SuicidesPerMinute * (double) input.TimeReconnect, 0));
      return input;
    }

    public void RefreshScores()
    {
      BindingList<Score> bindingList1 = new BindingList<Score>();
      BindingList<Score> bindingList2 = new BindingList<Score>();
      BindingList<Score> bindingList3 = new BindingList<Score>();
      foreach (Player player in (CollectionBase) this._Players)
      {
        int id = this._Players.IndexOf(player.Name);
        if (id > 0)
        {
          Score score = new Score()
          {
            Name = player.Name,
            TimeConnected = this.GetConnectedTime(id),
            TimeReconnect = this.GetReconnectTime(id),
            Kills = this.GetKillCount(id),
            Deaths = this.GetDeathCount(id),
            Suicides = this.GetSuicideCount(id)
          };
          score.KillDeathRatio = LogFile.KillDeathRatio(score.Kills, score.Deaths);
          score.KillsPerMinute = LogFile.KillsPerMinute(score.Kills, score.TimeConnected);
          score.DeathsPerMinute = LogFile.KillsPerMinute(score.Deaths, score.TimeConnected);
          score.SuicidesPerMinute = LogFile.KillsPerMinute(score.Suicides, score.TimeConnected);
          score.CTFOffense = this.GetTriggerCount(id, "CapturedFlag") * 10 + this.GetTriggerCount(id, "TookFlag");
          score.CTFDefense = this.GetTriggerAgainstCount(id, "FlagDefense") * 2;
          score.CTFPoints = score.CTFDefense + score.CTFOffense;
          score.Scored = score.CTFPoints + score.Kills;
          score.TimeAccelerator = this.GetPowerupTime(id, "Damage");
          score.TimeBackPack = this.GetPowerupTime(id, "Ammo");
          score.TimePortableBattery = this.GetPowerupTime(id, "Battery");
          score.TimeRegenerator = this.GetPowerupTime(id, "Health");
          score.TimeSpeed = this.GetPowerupTime(id, "Speed");
          switch (player.Team)
          {
            case Team.MI6:
              bindingList1.Add(score);
              continue;
            case Team.Phoenix:
              bindingList2.Add(score);
              continue;
            case Team.NotApplicable:
              bindingList3.Add(score);
              continue;
            default:
              continue;
          }
        }
      }
      this.ScoreMI6 = new Score[bindingList1.Count];
      this.ScorePhoenix = new Score[bindingList2.Count];
      this.ScoreOther = new Score[bindingList3.Count];
      bindingList1.CopyTo(this.ScoreMI6, 0);
      bindingList2.CopyTo(this.ScorePhoenix, 0);
      bindingList3.CopyTo(this.ScoreOther, 0);
    }

    public void RefreshReconnectScores()
    {
      if (this.ScoreMI6 == null || this.ScorePhoenix == null || this.ScoreOther == null)
      {
        Messages.ThrowException("Log.LogFile.RefreshConnectScore()", Messages.SCORE_TEAM_NULL);
      }
      else
      {
        this.ReconnectMI6 = new Score[this.ScoreMI6.Length];
        for (int index = 0; index < this.ScoreMI6.Length; ++index)
        {
          this.ReconnectMI6[index] = this.ScoreMI6[index];
          if (this.ReconnectMI6[index].TimeReconnect > 0)
          {
            this.ReconnectMI6[index].Deaths += Convert.ToInt32(this.ReconnectMI6[index].DeathsPerMinute / 60.0 * (double) this.ReconnectMI6[index].TimeReconnect);
            this.ReconnectMI6[index].Kills += Convert.ToInt32(this.ReconnectMI6[index].KillsPerMinute / 60.0 * (double) this.ReconnectMI6[index].TimeReconnect);
            this.ReconnectMI6[index].Suicides += Convert.ToInt32(this.ReconnectMI6[index].SuicidesPerMinute / 60.0 * (double) this.ReconnectMI6[index].TimeReconnect);
            this.ReconnectMI6[index].Scored = this.ReconnectMI6[index].Kills + this.ReconnectMI6[index].CTFPoints;
          }
        }
        this.ReconnectPhoenix = new Score[this.ScorePhoenix.Length];
        for (int index = 0; index < this.ScorePhoenix.Length; ++index)
        {
          this.ReconnectPhoenix[index] = this.ScorePhoenix[index];
          if (this.ReconnectPhoenix[index].TimeReconnect > 0)
          {
            this.ReconnectPhoenix[index].Deaths += Convert.ToInt32(this.ReconnectPhoenix[index].DeathsPerMinute / 60.0 * (double) this.ReconnectPhoenix[index].TimeReconnect);
            this.ReconnectPhoenix[index].Kills += Convert.ToInt32(this.ReconnectPhoenix[index].KillsPerMinute / 60.0 * (double) this.ReconnectPhoenix[index].TimeReconnect);
            this.ReconnectPhoenix[index].Suicides += Convert.ToInt32(this.ReconnectPhoenix[index].SuicidesPerMinute / 60.0 * (double) this.ReconnectPhoenix[index].TimeReconnect);
            this.ReconnectPhoenix[index].Scored = this.ReconnectPhoenix[index].Kills + this.ReconnectPhoenix[index].CTFPoints;
          }
        }
        this.ReconnectOther = new Score[this.ScoreOther.Length];
        for (int index = 0; index < this.ScoreOther.Length; ++index)
        {
          this.ReconnectOther[index] = this.ScoreOther[index];
          if (this.ReconnectOther[index].TimeReconnect > 0)
          {
            this.ReconnectOther[index].Deaths += Convert.ToInt32(this.ReconnectOther[index].DeathsPerMinute / 60.0 * (double) this.ReconnectOther[index].TimeReconnect);
            this.ReconnectOther[index].Kills += Convert.ToInt32(this.ReconnectOther[index].KillsPerMinute / 60.0 * (double) this.ReconnectOther[index].TimeReconnect);
            this.ReconnectOther[index].Suicides += Convert.ToInt32(this.ReconnectOther[index].SuicidesPerMinute / 60.0 * (double) this.ReconnectOther[index].TimeReconnect);
            this.ReconnectOther[index].Scored = this.ReconnectOther[index].Kills + this.ReconnectOther[index].CTFPoints;
          }
        }
      }
    }

    public static Score GetTeamScore(Score[] team, string name)
    {
      if (team == null)
      {
        Messages.ThrowException("Log.LogFile.GetTeamScore(Score[], string)", Messages.SCORE_NULL);
        return new Score();
      }
      Score teamScore = new Score();
      teamScore.Name = name;
      foreach (Score score in team)
      {
        teamScore.Scored += score.Scored;
        teamScore.CTFPoints += score.CTFPoints;
        teamScore.CTFOffense += score.CTFOffense;
        teamScore.CTFDefense += score.CTFDefense;
        teamScore.Deaths += score.Deaths;
        teamScore.Kills += score.Kills;
        teamScore.Suicides += score.Suicides;
        teamScore.TimeAccelerator += score.TimeAccelerator;
        teamScore.TimeBackPack += score.TimeBackPack;
        teamScore.TimePortableBattery += score.TimePortableBattery;
        teamScore.TimeRegenerator += score.TimeRegenerator;
        teamScore.TimeSpeed += score.TimeSpeed;
        teamScore.TimeConnected += score.TimeConnected;
        teamScore.TimeReconnect += score.TimeReconnect;
      }
      teamScore.DeathsPerMinute = LogFile.KillsPerMinute(teamScore.Deaths, teamScore.TimeConnected);
      teamScore.KillsPerMinute = LogFile.KillsPerMinute(teamScore.Kills, teamScore.TimeConnected);
      teamScore.SuicidesPerMinute = LogFile.KillsPerMinute(teamScore.Suicides, teamScore.TimeConnected);
      teamScore.KillDeathRatio = LogFile.KillDeathRatio(teamScore.Kills, teamScore.Kills);
      return teamScore;
    }

    public Score[] GetDeathmatchScore()
    {
      Score score1 = (Score) null;
      int num1 = int.MinValue;
      foreach (Score score2 in this.ScoreOther)
      {
        if (score2.Kills >= num1)
        {
          num1 = score2.Kills;
          score1 = score2;
        }
      }
      if (score1 == null)
        return (Score[]) null;
      List<Score> scoreList1 = new List<Score>();
      foreach (Score score3 in this.ScoreOther)
      {
        if (score3.Kills == score1.Kills)
          scoreList1.Add(score3);
      }
      int num2 = int.MaxValue;
      foreach (Score score4 in scoreList1)
      {
        if (score4.Deaths >= num2)
        {
          num2 = score4.Deaths;
          score1 = score4;
        }
      }
      List<Score> scoreList2 = new List<Score>();
      foreach (Score score5 in scoreList1)
      {
        if (score5.Kills == score1.Kills && score5.Deaths == score1.Deaths)
          scoreList2.Add(score5);
      }
      return scoreList2.ToArray();
    }

    private Score GetScoreOfHighest(ref Score[] array, ref int[,] haystack, int index)
    {
      int num = 0;
      int index1 = -1;
      for (int index2 = 0; index2 <= haystack.GetUpperBound(0); ++index2)
      {
        if (haystack[index2, index] > num)
        {
          num = haystack[index2, index];
          index1 = index2;
        }
      }
      return index1 == -1 ? new Score() : array[index1];
    }

    private Score GetScoreOfLowest(ref Score[] array, ref int[,] haystack, int index)
    {
      int index1 = -1;
      int num = haystack.Length == 0 ? 0 : haystack[0, index];
      for (int index2 = 0; index2 <= haystack.GetUpperBound(0); ++index2)
      {
        if (haystack[index2, index] < num)
        {
          num = haystack[index2, index];
          index1 = index2;
        }
      }
      return index1 == -1 ? new Score() : array[index1];
    }

    private Score GetScoreOfHighest(ref Score[] array, ref double[,] haystack, int index)
    {
      double num = 0.0;
      int index1 = -1;
      for (int index2 = 0; index2 <= haystack.GetUpperBound(0); ++index2)
      {
        if (haystack[index2, index] > num)
        {
          num = haystack[index2, index];
          index1 = index2;
        }
      }
      return index1 == -1 ? new Score() : array[index1];
    }

    private Score GetScoreOfLowest(ref Score[] array, ref double[,] haystack, int index)
    {
      int index1 = -1;
      double num = haystack.Length == 0 ? 0.0 : haystack[0, index];
      for (int index2 = 0; index2 <= haystack.GetUpperBound(0); ++index2)
      {
        if (haystack[index2, index] < num)
        {
          num = haystack[index2, index];
          index1 = index2;
        }
      }
      return index1 == -1 ? new Score() : array[index1];
    }

    private void GetThreshold(Score[] array, ref ScoreThreshold most, ref ScoreThreshold least)
    {
      if (array == null)
      {
        Messages.ThrowException("Log.LogFile.GetThreshold(Score[], ScoreThreshold, ScoreThreshold)", Messages.SCORE_NULL);
      }
      else
      {
        int[,] haystack1 = new int[array.Length, 14];
        for (int index = 0; index < array.Length; ++index)
        {
          haystack1[index, 0] = array[index].CTFDefense;
          haystack1[index, 1] = array[index].CTFOffense;
          haystack1[index, 2] = array[index].CTFPoints;
          haystack1[index, 3] = array[index].Deaths;
          haystack1[index, 4] = array[index].Kills;
          haystack1[index, 5] = array[index].Scored;
          haystack1[index, 6] = array[index].Suicides;
          haystack1[index, 7] = array[index].TimeAccelerator;
          haystack1[index, 8] = array[index].TimeBackPack;
          haystack1[index, 9] = array[index].TimeConnected;
          haystack1[index, 10] = array[index].TimePortableBattery;
          haystack1[index, 11] = array[index].TimeReconnect;
          haystack1[index, 12] = array[index].TimeRegenerator;
          haystack1[index, 13] = array[index].TimeSpeed;
        }
        Score scoreOfHighest1 = this.GetScoreOfHighest(ref array, ref haystack1, 0);
        most.CTFDefense = new Variable(scoreOfHighest1.Name, scoreOfHighest1.CTFDefense);
        Score scoreOfHighest2 = this.GetScoreOfHighest(ref array, ref haystack1, 1);
        most.CTFOffense = new Variable(scoreOfHighest2.Name, scoreOfHighest2.CTFOffense);
        Score scoreOfHighest3 = this.GetScoreOfHighest(ref array, ref haystack1, 2);
        most.CTFPoints = new Variable(scoreOfHighest3.Name, scoreOfHighest3.CTFPoints);
        Score scoreOfHighest4 = this.GetScoreOfHighest(ref array, ref haystack1, 3);
        most.Deaths = new Variable(scoreOfHighest4.Name, scoreOfHighest4.Deaths);
        Score scoreOfHighest5 = this.GetScoreOfHighest(ref array, ref haystack1, 4);
        most.Kills = new Variable(scoreOfHighest5.Name, scoreOfHighest5.Kills);
        Score scoreOfHighest6 = this.GetScoreOfHighest(ref array, ref haystack1, 5);
        most.Scored = new Variable(scoreOfHighest6.Name, scoreOfHighest6.Scored);
        Score scoreOfHighest7 = this.GetScoreOfHighest(ref array, ref haystack1, 6);
        most.Suicides = new Variable(scoreOfHighest7.Name, scoreOfHighest7.Suicides);
        Score scoreOfHighest8 = this.GetScoreOfHighest(ref array, ref haystack1, 7);
        most.TimeAccelerator = new Variable(scoreOfHighest8.Name, scoreOfHighest8.TimeAccelerator);
        Score scoreOfHighest9 = this.GetScoreOfHighest(ref array, ref haystack1, 8);
        most.TimeBackPack = new Variable(scoreOfHighest9.Name, scoreOfHighest9.TimeBackPack);
        Score scoreOfHighest10 = this.GetScoreOfHighest(ref array, ref haystack1, 9);
        most.TimeConnected = new Variable(scoreOfHighest10.Name, scoreOfHighest10.TimeConnected);
        Score scoreOfHighest11 = this.GetScoreOfHighest(ref array, ref haystack1, 10);
        most.TimePortableBattery = new Variable(scoreOfHighest11.Name, scoreOfHighest11.TimePortableBattery);
        Score scoreOfHighest12 = this.GetScoreOfHighest(ref array, ref haystack1, 11);
        most.TimeReconnect = new Variable(scoreOfHighest12.Name, scoreOfHighest12.TimeReconnect);
        Score scoreOfHighest13 = this.GetScoreOfHighest(ref array, ref haystack1, 12);
        most.TimeRegenerator = new Variable(scoreOfHighest13.Name, scoreOfHighest13.TimeRegenerator);
        Score scoreOfHighest14 = this.GetScoreOfHighest(ref array, ref haystack1, 13);
        most.TimeSpeed = new Variable(scoreOfHighest14.Name, scoreOfHighest14.TimeReconnect);
        Score scoreOfLowest1 = this.GetScoreOfLowest(ref array, ref haystack1, 0);
        least.CTFDefense = new Variable(scoreOfLowest1.Name, scoreOfLowest1.CTFDefense);
        Score scoreOfLowest2 = this.GetScoreOfLowest(ref array, ref haystack1, 1);
        least.CTFOffense = new Variable(scoreOfLowest2.Name, scoreOfLowest2.CTFOffense);
        Score scoreOfLowest3 = this.GetScoreOfLowest(ref array, ref haystack1, 2);
        least.CTFPoints = new Variable(scoreOfLowest3.Name, scoreOfLowest3.CTFPoints);
        Score scoreOfLowest4 = this.GetScoreOfLowest(ref array, ref haystack1, 3);
        least.Deaths = new Variable(scoreOfLowest4.Name, scoreOfLowest4.Deaths);
        Score scoreOfLowest5 = this.GetScoreOfLowest(ref array, ref haystack1, 4);
        least.Kills = new Variable(scoreOfLowest5.Name, scoreOfLowest5.Kills);
        Score scoreOfLowest6 = this.GetScoreOfLowest(ref array, ref haystack1, 5);
        least.Scored = new Variable(scoreOfLowest6.Name, scoreOfLowest6.Scored);
        Score scoreOfLowest7 = this.GetScoreOfLowest(ref array, ref haystack1, 6);
        least.Suicides = new Variable(scoreOfLowest7.Name, scoreOfLowest7.Suicides);
        Score scoreOfLowest8 = this.GetScoreOfLowest(ref array, ref haystack1, 7);
        least.TimeAccelerator = new Variable(scoreOfLowest8.Name, scoreOfLowest8.TimeAccelerator);
        Score scoreOfLowest9 = this.GetScoreOfLowest(ref array, ref haystack1, 8);
        least.TimeBackPack = new Variable(scoreOfLowest9.Name, scoreOfLowest9.TimeBackPack);
        Score scoreOfLowest10 = this.GetScoreOfLowest(ref array, ref haystack1, 9);
        least.TimeConnected = new Variable(scoreOfLowest10.Name, scoreOfLowest10.TimeConnected);
        Score scoreOfLowest11 = this.GetScoreOfLowest(ref array, ref haystack1, 10);
        least.TimePortableBattery = new Variable(scoreOfLowest11.Name, scoreOfLowest11.TimePortableBattery);
        Score scoreOfLowest12 = this.GetScoreOfLowest(ref array, ref haystack1, 11);
        least.TimeReconnect = new Variable(scoreOfLowest12.Name, scoreOfLowest12.TimeReconnect);
        Score scoreOfLowest13 = this.GetScoreOfLowest(ref array, ref haystack1, 12);
        least.TimeRegenerator = new Variable(scoreOfLowest13.Name, scoreOfLowest13.TimeRegenerator);
        Score scoreOfLowest14 = this.GetScoreOfLowest(ref array, ref haystack1, 13);
        least.TimeSpeed = new Variable(scoreOfLowest14.Name, scoreOfLowest14.TimeReconnect);
        double[,] haystack2 = new double[array.Length, 4];
        for (int index = 0; index < array.Length; ++index)
        {
          haystack2[index, 0] = array[index].DeathsPerMinute;
          haystack2[index, 1] = array[index].KillDeathRatio;
          haystack2[index, 2] = array[index].KillsPerMinute;
          haystack2[index, 3] = array[index].SuicidesPerMinute;
        }
        Score scoreOfHighest15 = this.GetScoreOfHighest(ref array, ref haystack2, 0);
        most.DeathsPerMinute = new Variable(scoreOfHighest15.Name, scoreOfHighest15.DeathsPerMinute);
        Score scoreOfHighest16 = this.GetScoreOfHighest(ref array, ref haystack2, 1);
        most.KillDeathRatio = new Variable(scoreOfHighest16.Name, scoreOfHighest16.KillDeathRatio);
        Score scoreOfHighest17 = this.GetScoreOfHighest(ref array, ref haystack2, 2);
        most.KillsPerMinute = new Variable(scoreOfHighest17.Name, scoreOfHighest17.KillsPerMinute);
        Score scoreOfHighest18 = this.GetScoreOfHighest(ref array, ref haystack2, 3);
        most.SuicidesPerMinute = new Variable(scoreOfHighest18.Name, scoreOfHighest18.SuicidesPerMinute);
        Score scoreOfLowest15 = this.GetScoreOfLowest(ref array, ref haystack2, 0);
        least.DeathsPerMinute = new Variable(scoreOfLowest15.Name, scoreOfLowest15.DeathsPerMinute);
        Score scoreOfLowest16 = this.GetScoreOfLowest(ref array, ref haystack2, 1);
        least.KillDeathRatio = new Variable(scoreOfLowest16.Name, scoreOfLowest16.KillDeathRatio);
        Score scoreOfLowest17 = this.GetScoreOfLowest(ref array, ref haystack2, 2);
        least.KillsPerMinute = new Variable(scoreOfLowest17.Name, scoreOfLowest17.KillsPerMinute);
        Score scoreOfLowest18 = this.GetScoreOfLowest(ref array, ref haystack2, 3);
        least.SuicidesPerMinute = new Variable(scoreOfLowest18.Name, scoreOfLowest18.SuicidesPerMinute);
      }
    }

    private void GetOverallThreshold()
    {
      if (Convert.ToInt32(this.MostMI6.Scored.Value) > Convert.ToInt32(this.MostPhoenix.Scored.Value) && Convert.ToInt32(this.MostMI6.Scored.Value) > Convert.ToInt32(this.MostOther.Scored.Value))
        this.MostOverall.Scored = this.MostMI6.Scored;
      if (Convert.ToInt32(this.MostPhoenix.Scored.Value) > Convert.ToInt32(this.MostMI6.Scored.Value) && Convert.ToInt32(this.MostPhoenix.Scored.Value) > Convert.ToInt32(this.MostOther.Scored.Value))
        this.MostOverall.Scored = this.MostPhoenix.Scored;
      if (Convert.ToInt32(this.MostOther.Scored.Value) > Convert.ToInt32(this.MostPhoenix.Scored.Value) && Convert.ToInt32(this.MostOther.Scored.Value) > Convert.ToInt32(this.MostMI6.Scored.Value))
        this.MostOverall.Scored = this.MostOther.Scored;
      if (Convert.ToInt32(this.MostMI6.TimeConnected.Value) > Convert.ToInt32(this.MostPhoenix.TimeConnected.Value) && Convert.ToInt32(this.MostMI6.TimeConnected.Value) > Convert.ToInt32(this.MostOther.TimeConnected.Value))
        this.MostOverall.TimeConnected = this.MostMI6.TimeConnected;
      if (Convert.ToInt32(this.MostPhoenix.TimeConnected.Value) > Convert.ToInt32(this.MostMI6.TimeConnected.Value) && Convert.ToInt32(this.MostPhoenix.TimeConnected.Value) > Convert.ToInt32(this.MostOther.TimeConnected.Value))
        this.MostOverall.TimeConnected = this.MostPhoenix.TimeConnected;
      if (Convert.ToInt32(this.MostOther.TimeConnected.Value) > Convert.ToInt32(this.MostPhoenix.TimeConnected.Value) && Convert.ToInt32(this.MostOther.TimeConnected.Value) > Convert.ToInt32(this.MostMI6.TimeConnected.Value))
        this.MostOverall.TimeConnected = this.MostOther.TimeConnected;
      if (Convert.ToInt32(this.MostMI6.TimeReconnect.Value) > Convert.ToInt32(this.MostPhoenix.TimeReconnect.Value) && Convert.ToInt32(this.MostMI6.TimeReconnect.Value) > Convert.ToInt32(this.MostOther.TimeReconnect.Value))
        this.MostOverall.TimeReconnect = this.MostMI6.TimeReconnect;
      if (Convert.ToInt32(this.MostPhoenix.TimeReconnect.Value) > Convert.ToInt32(this.MostMI6.TimeReconnect.Value) && Convert.ToInt32(this.MostPhoenix.TimeReconnect.Value) > Convert.ToInt32(this.MostOther.TimeReconnect.Value))
        this.MostOverall.TimeReconnect = this.MostPhoenix.TimeReconnect;
      if (Convert.ToInt32(this.MostOther.TimeReconnect.Value) > Convert.ToInt32(this.MostPhoenix.TimeReconnect.Value) && Convert.ToInt32(this.MostOther.TimeReconnect.Value) > Convert.ToInt32(this.MostMI6.TimeReconnect.Value))
        this.MostOverall.TimeReconnect = this.MostOther.TimeReconnect;
      if (Convert.ToInt32(this.MostMI6.Kills.Value) > Convert.ToInt32(this.MostPhoenix.Kills.Value) && Convert.ToInt32(this.MostMI6.Kills.Value) > Convert.ToInt32(this.MostOther.Kills.Value))
        this.MostOverall.Kills = this.MostMI6.Kills;
      if (Convert.ToInt32(this.MostPhoenix.Kills.Value) > Convert.ToInt32(this.MostMI6.Kills.Value) && Convert.ToInt32(this.MostPhoenix.Kills.Value) > Convert.ToInt32(this.MostOther.Kills.Value))
        this.MostOverall.Kills = this.MostPhoenix.Kills;
      if (Convert.ToInt32(this.MostOther.Kills.Value) > Convert.ToInt32(this.MostPhoenix.Kills.Value) && Convert.ToInt32(this.MostOther.Kills.Value) > Convert.ToInt32(this.MostMI6.Kills.Value))
        this.MostOverall.Kills = this.MostOther.Kills;
      if (Convert.ToInt32(this.MostMI6.Deaths.Value) > Convert.ToInt32(this.MostPhoenix.Deaths.Value) && Convert.ToInt32(this.MostMI6.Deaths.Value) > Convert.ToInt32(this.MostOther.Deaths.Value))
        this.MostOverall.Deaths = this.MostMI6.Deaths;
      if (Convert.ToInt32(this.MostPhoenix.Deaths.Value) > Convert.ToInt32(this.MostMI6.Deaths.Value) && Convert.ToInt32(this.MostPhoenix.Deaths.Value) > Convert.ToInt32(this.MostOther.Deaths.Value))
        this.MostOverall.Deaths = this.MostPhoenix.Deaths;
      if (Convert.ToInt32(this.MostOther.Deaths.Value) > Convert.ToInt32(this.MostPhoenix.Deaths.Value) && Convert.ToInt32(this.MostOther.Deaths.Value) > Convert.ToInt32(this.MostMI6.Deaths.Value))
        this.MostOverall.Deaths = this.MostOther.Deaths;
      if (Convert.ToInt32(this.MostMI6.Suicides.Value) > Convert.ToInt32(this.MostPhoenix.Suicides.Value) && Convert.ToInt32(this.MostMI6.Suicides.Value) > Convert.ToInt32(this.MostOther.Suicides.Value))
        this.MostOverall.Suicides = this.MostMI6.Suicides;
      if (Convert.ToInt32(this.MostPhoenix.Suicides.Value) > Convert.ToInt32(this.MostMI6.Suicides.Value) && Convert.ToInt32(this.MostPhoenix.Suicides.Value) > Convert.ToInt32(this.MostOther.Suicides.Value))
        this.MostOverall.Suicides = this.MostPhoenix.Suicides;
      if (Convert.ToInt32(this.MostOther.Suicides.Value) > Convert.ToInt32(this.MostPhoenix.Suicides.Value) && Convert.ToInt32(this.MostOther.Suicides.Value) > Convert.ToInt32(this.MostMI6.Suicides.Value))
        this.MostOverall.Suicides = this.MostOther.Suicides;
      if (Convert.ToDouble(this.MostMI6.KillDeathRatio.Value) > Convert.ToDouble(this.MostPhoenix.KillDeathRatio.Value) && Convert.ToDouble(this.MostMI6.KillDeathRatio.Value) > Convert.ToDouble(this.MostOther.KillDeathRatio.Value))
        this.MostOverall.KillDeathRatio = this.MostMI6.KillDeathRatio;
      if (Convert.ToDouble(this.MostPhoenix.KillDeathRatio.Value) > Convert.ToDouble(this.MostMI6.KillDeathRatio.Value) && Convert.ToDouble(this.MostPhoenix.KillDeathRatio.Value) > Convert.ToDouble(this.MostOther.KillDeathRatio.Value))
        this.MostOverall.KillDeathRatio = this.MostPhoenix.KillDeathRatio;
      if (Convert.ToDouble(this.MostOther.KillDeathRatio.Value) > Convert.ToDouble(this.MostPhoenix.KillDeathRatio.Value) && Convert.ToDouble(this.MostOther.KillDeathRatio.Value) > Convert.ToDouble(this.MostMI6.KillDeathRatio.Value))
        this.MostOverall.KillDeathRatio = this.MostOther.KillDeathRatio;
      if (Convert.ToDouble(this.MostMI6.KillsPerMinute.Value) > Convert.ToDouble(this.MostPhoenix.KillsPerMinute.Value) && Convert.ToDouble(this.MostMI6.KillsPerMinute.Value) > Convert.ToDouble(this.MostOther.KillsPerMinute.Value))
        this.MostOverall.KillsPerMinute = this.MostMI6.KillsPerMinute;
      if (Convert.ToDouble(this.MostPhoenix.KillsPerMinute.Value) > Convert.ToDouble(this.MostMI6.KillsPerMinute.Value) && Convert.ToDouble(this.MostPhoenix.KillsPerMinute.Value) > Convert.ToDouble(this.MostOther.KillsPerMinute.Value))
        this.MostOverall.KillsPerMinute = this.MostPhoenix.KillsPerMinute;
      if (Convert.ToDouble(this.MostOther.KillsPerMinute.Value) > Convert.ToDouble(this.MostPhoenix.KillsPerMinute.Value) && Convert.ToDouble(this.MostOther.KillsPerMinute.Value) > Convert.ToDouble(this.MostMI6.KillsPerMinute.Value))
        this.MostOverall.KillsPerMinute = this.MostOther.KillsPerMinute;
      if (Convert.ToDouble(this.MostMI6.DeathsPerMinute.Value) > Convert.ToDouble(this.MostPhoenix.DeathsPerMinute.Value) && Convert.ToDouble(this.MostMI6.DeathsPerMinute.Value) > Convert.ToDouble(this.MostOther.DeathsPerMinute.Value))
        this.MostOverall.DeathsPerMinute = this.MostMI6.DeathsPerMinute;
      if (Convert.ToDouble(this.MostPhoenix.DeathsPerMinute.Value) > Convert.ToDouble(this.MostMI6.DeathsPerMinute.Value) && Convert.ToDouble(this.MostPhoenix.DeathsPerMinute.Value) > Convert.ToDouble(this.MostOther.DeathsPerMinute.Value))
        this.MostOverall.DeathsPerMinute = this.MostPhoenix.DeathsPerMinute;
      if (Convert.ToDouble(this.MostOther.DeathsPerMinute.Value) > Convert.ToDouble(this.MostPhoenix.DeathsPerMinute.Value) && Convert.ToDouble(this.MostOther.DeathsPerMinute.Value) > Convert.ToDouble(this.MostMI6.DeathsPerMinute.Value))
        this.MostOverall.DeathsPerMinute = this.MostOther.DeathsPerMinute;
      if (Convert.ToDouble(this.MostMI6.SuicidesPerMinute.Value) > Convert.ToDouble(this.MostPhoenix.SuicidesPerMinute.Value) && Convert.ToDouble(this.MostMI6.SuicidesPerMinute.Value) > Convert.ToDouble(this.MostOther.SuicidesPerMinute.Value))
        this.MostOverall.SuicidesPerMinute = this.MostMI6.SuicidesPerMinute;
      if (Convert.ToDouble(this.MostPhoenix.SuicidesPerMinute.Value) > Convert.ToDouble(this.MostMI6.SuicidesPerMinute.Value) && Convert.ToDouble(this.MostPhoenix.SuicidesPerMinute.Value) > Convert.ToDouble(this.MostOther.SuicidesPerMinute.Value))
        this.MostOverall.SuicidesPerMinute = this.MostPhoenix.SuicidesPerMinute;
      if (Convert.ToDouble(this.MostOther.SuicidesPerMinute.Value) > Convert.ToDouble(this.MostPhoenix.SuicidesPerMinute.Value) && Convert.ToDouble(this.MostOther.SuicidesPerMinute.Value) > Convert.ToDouble(this.MostMI6.SuicidesPerMinute.Value))
        this.MostOverall.SuicidesPerMinute = this.MostOther.SuicidesPerMinute;
      if (Convert.ToInt32(this.MostMI6.CTFPoints.Value) > Convert.ToInt32(this.MostPhoenix.CTFPoints.Value) && Convert.ToInt32(this.MostMI6.CTFPoints.Value) > Convert.ToInt32(this.MostOther.CTFPoints.Value))
        this.MostOverall.CTFPoints = this.MostMI6.CTFPoints;
      if (Convert.ToInt32(this.MostPhoenix.CTFPoints.Value) > Convert.ToInt32(this.MostMI6.CTFPoints.Value) && Convert.ToInt32(this.MostPhoenix.CTFPoints.Value) > Convert.ToInt32(this.MostOther.CTFPoints.Value))
        this.MostOverall.CTFPoints = this.MostPhoenix.CTFPoints;
      if (Convert.ToInt32(this.MostOther.CTFPoints.Value) > Convert.ToInt32(this.MostPhoenix.CTFPoints.Value) && Convert.ToInt32(this.MostOther.CTFPoints.Value) > Convert.ToInt32(this.MostMI6.CTFPoints.Value))
        this.MostOverall.CTFPoints = this.MostOther.CTFPoints;
      if (Convert.ToInt32(this.MostMI6.CTFOffense.Value) > Convert.ToInt32(this.MostPhoenix.CTFOffense.Value) && Convert.ToInt32(this.MostMI6.CTFOffense.Value) > Convert.ToInt32(this.MostOther.CTFOffense.Value))
        this.MostOverall.CTFOffense = this.MostMI6.CTFOffense;
      if (Convert.ToInt32(this.MostPhoenix.CTFOffense.Value) > Convert.ToInt32(this.MostMI6.CTFOffense.Value) && Convert.ToInt32(this.MostPhoenix.CTFOffense.Value) > Convert.ToInt32(this.MostOther.CTFOffense.Value))
        this.MostOverall.CTFOffense = this.MostPhoenix.CTFOffense;
      if (Convert.ToInt32(this.MostOther.CTFOffense.Value) > Convert.ToInt32(this.MostPhoenix.CTFOffense.Value) && Convert.ToInt32(this.MostOther.CTFOffense.Value) > Convert.ToInt32(this.MostMI6.CTFOffense.Value))
        this.MostOverall.CTFOffense = this.MostOther.CTFOffense;
      if (Convert.ToInt32(this.MostMI6.CTFDefense.Value) > Convert.ToInt32(this.MostPhoenix.CTFDefense.Value) && Convert.ToInt32(this.MostMI6.CTFDefense.Value) > Convert.ToInt32(this.MostOther.CTFDefense.Value))
        this.MostOverall.CTFDefense = this.MostMI6.CTFDefense;
      if (Convert.ToInt32(this.MostPhoenix.CTFDefense.Value) > Convert.ToInt32(this.MostMI6.CTFDefense.Value) && Convert.ToInt32(this.MostPhoenix.CTFDefense.Value) > Convert.ToInt32(this.MostOther.CTFDefense.Value))
        this.MostOverall.CTFDefense = this.MostPhoenix.CTFDefense;
      if (Convert.ToInt32(this.MostOther.CTFDefense.Value) > Convert.ToInt32(this.MostPhoenix.CTFDefense.Value) && Convert.ToInt32(this.MostOther.CTFDefense.Value) > Convert.ToInt32(this.MostMI6.CTFDefense.Value))
        this.MostOverall.CTFDefense = this.MostOther.CTFDefense;
      if (Convert.ToInt32(this.MostMI6.TimeAccelerator.Value) > Convert.ToInt32(this.MostPhoenix.TimeAccelerator.Value) && Convert.ToInt32(this.MostMI6.TimeAccelerator.Value) > Convert.ToInt32(this.MostOther.TimeAccelerator.Value))
        this.MostOverall.TimeAccelerator = this.MostMI6.TimeAccelerator;
      if (Convert.ToInt32(this.MostPhoenix.TimeAccelerator.Value) > Convert.ToInt32(this.MostMI6.TimeAccelerator.Value) && Convert.ToInt32(this.MostPhoenix.TimeAccelerator.Value) > Convert.ToInt32(this.MostOther.TimeAccelerator.Value))
        this.MostOverall.TimeAccelerator = this.MostPhoenix.TimeAccelerator;
      if (Convert.ToInt32(this.MostOther.TimeAccelerator.Value) > Convert.ToInt32(this.MostPhoenix.TimeAccelerator.Value) && Convert.ToInt32(this.MostOther.TimeAccelerator.Value) > Convert.ToInt32(this.MostMI6.TimeAccelerator.Value))
        this.MostOverall.TimeAccelerator = this.MostOther.TimeAccelerator;
      if (Convert.ToInt32(this.MostMI6.TimeBackPack.Value) > Convert.ToInt32(this.MostPhoenix.TimeBackPack.Value) && Convert.ToInt32(this.MostMI6.TimeBackPack.Value) > Convert.ToInt32(this.MostOther.TimeBackPack.Value))
        this.MostOverall.TimeBackPack = this.MostMI6.TimeBackPack;
      if (Convert.ToInt32(this.MostPhoenix.TimeBackPack.Value) > Convert.ToInt32(this.MostMI6.TimeBackPack.Value) && Convert.ToInt32(this.MostPhoenix.TimeBackPack.Value) > Convert.ToInt32(this.MostOther.TimeBackPack.Value))
        this.MostOverall.TimeBackPack = this.MostPhoenix.TimeBackPack;
      if (Convert.ToInt32(this.MostOther.TimeBackPack.Value) > Convert.ToInt32(this.MostPhoenix.TimeBackPack.Value) && Convert.ToInt32(this.MostOther.TimeBackPack.Value) > Convert.ToInt32(this.MostMI6.TimeBackPack.Value))
        this.MostOverall.TimeBackPack = this.MostOther.TimeBackPack;
      if (Convert.ToInt32(this.MostMI6.TimePortableBattery.Value) > Convert.ToInt32(this.MostPhoenix.TimePortableBattery.Value) && Convert.ToInt32(this.MostMI6.TimePortableBattery.Value) > Convert.ToInt32(this.MostOther.TimePortableBattery.Value))
        this.MostOverall.TimePortableBattery = this.MostMI6.TimePortableBattery;
      if (Convert.ToInt32(this.MostPhoenix.TimePortableBattery.Value) > Convert.ToInt32(this.MostMI6.TimePortableBattery.Value) && Convert.ToInt32(this.MostPhoenix.TimePortableBattery.Value) > Convert.ToInt32(this.MostOther.TimePortableBattery.Value))
        this.MostOverall.TimePortableBattery = this.MostPhoenix.TimePortableBattery;
      if (Convert.ToInt32(this.MostOther.TimePortableBattery.Value) > Convert.ToInt32(this.MostPhoenix.TimePortableBattery.Value) && Convert.ToInt32(this.MostOther.TimePortableBattery.Value) > Convert.ToInt32(this.MostMI6.TimePortableBattery.Value))
        this.MostOverall.TimePortableBattery = this.MostOther.TimePortableBattery;
      if (Convert.ToInt32(this.MostMI6.TimeRegenerator.Value) > Convert.ToInt32(this.MostPhoenix.TimeRegenerator.Value) && Convert.ToInt32(this.MostMI6.TimeRegenerator.Value) > Convert.ToInt32(this.MostOther.TimeRegenerator.Value))
        this.MostOverall.TimeRegenerator = this.MostMI6.TimeRegenerator;
      if (Convert.ToInt32(this.MostPhoenix.TimeRegenerator.Value) > Convert.ToInt32(this.MostMI6.TimeRegenerator.Value) && Convert.ToInt32(this.MostPhoenix.TimeRegenerator.Value) > Convert.ToInt32(this.MostOther.TimeRegenerator.Value))
        this.MostOverall.TimeRegenerator = this.MostPhoenix.TimeRegenerator;
      if (Convert.ToInt32(this.MostOther.TimeRegenerator.Value) > Convert.ToInt32(this.MostPhoenix.TimeRegenerator.Value) && Convert.ToInt32(this.MostOther.TimeRegenerator.Value) > Convert.ToInt32(this.MostMI6.TimeRegenerator.Value))
        this.MostOverall.TimeRegenerator = this.MostOther.TimeRegenerator;
      if (Convert.ToInt32(this.MostMI6.TimeSpeed.Value) > Convert.ToInt32(this.MostPhoenix.TimeSpeed.Value) && Convert.ToInt32(this.MostMI6.TimeSpeed.Value) > Convert.ToInt32(this.MostOther.TimeSpeed.Value))
        this.MostOverall.TimeSpeed = this.MostMI6.TimeSpeed;
      if (Convert.ToInt32(this.MostPhoenix.TimeSpeed.Value) > Convert.ToInt32(this.MostMI6.TimeSpeed.Value) && Convert.ToInt32(this.MostPhoenix.TimeSpeed.Value) > Convert.ToInt32(this.MostOther.TimeSpeed.Value))
        this.MostOverall.TimeSpeed = this.MostPhoenix.TimeSpeed;
      if (Convert.ToInt32(this.MostOther.TimeSpeed.Value) > Convert.ToInt32(this.MostPhoenix.TimeSpeed.Value) && Convert.ToInt32(this.MostOther.TimeSpeed.Value) > Convert.ToInt32(this.MostMI6.TimeSpeed.Value))
        this.MostOverall.TimeSpeed = this.MostOther.TimeSpeed;
      if (Convert.ToInt32(this.LeastMI6.Scored.Value) > Convert.ToInt32(this.LeastPhoenix.Scored.Value) && Convert.ToInt32(this.LeastMI6.Scored.Value) > Convert.ToInt32(this.LeastOther.Scored.Value))
        this.LeastOverall.Scored = this.LeastMI6.Scored;
      if (Convert.ToInt32(this.LeastPhoenix.Scored.Value) > Convert.ToInt32(this.LeastMI6.Scored.Value) && Convert.ToInt32(this.LeastPhoenix.Scored.Value) > Convert.ToInt32(this.LeastOther.Scored.Value))
        this.LeastOverall.Scored = this.LeastPhoenix.Scored;
      if (Convert.ToInt32(this.LeastOther.Scored.Value) > Convert.ToInt32(this.LeastPhoenix.Scored.Value) && Convert.ToInt32(this.LeastOther.Scored.Value) > Convert.ToInt32(this.LeastMI6.Scored.Value))
        this.LeastOverall.Scored = this.LeastOther.Scored;
      if (Convert.ToInt32(this.LeastMI6.TimeConnected.Value) > Convert.ToInt32(this.LeastPhoenix.TimeConnected.Value) && Convert.ToInt32(this.LeastMI6.TimeConnected.Value) > Convert.ToInt32(this.LeastOther.TimeConnected.Value))
        this.LeastOverall.TimeConnected = this.LeastMI6.TimeConnected;
      if (Convert.ToInt32(this.LeastPhoenix.TimeConnected.Value) > Convert.ToInt32(this.LeastMI6.TimeConnected.Value) && Convert.ToInt32(this.LeastPhoenix.TimeConnected.Value) > Convert.ToInt32(this.LeastOther.TimeConnected.Value))
        this.LeastOverall.TimeConnected = this.LeastPhoenix.TimeConnected;
      if (Convert.ToInt32(this.LeastOther.TimeConnected.Value) > Convert.ToInt32(this.LeastPhoenix.TimeConnected.Value) && Convert.ToInt32(this.LeastOther.TimeConnected.Value) > Convert.ToInt32(this.LeastMI6.TimeConnected.Value))
        this.LeastOverall.TimeConnected = this.LeastOther.TimeConnected;
      if (Convert.ToInt32(this.LeastMI6.TimeReconnect.Value) > Convert.ToInt32(this.LeastPhoenix.TimeReconnect.Value) && Convert.ToInt32(this.LeastMI6.TimeReconnect.Value) > Convert.ToInt32(this.LeastOther.TimeReconnect.Value))
        this.LeastOverall.TimeReconnect = this.LeastMI6.TimeReconnect;
      if (Convert.ToInt32(this.LeastPhoenix.TimeReconnect.Value) > Convert.ToInt32(this.LeastMI6.TimeReconnect.Value) && Convert.ToInt32(this.LeastPhoenix.TimeReconnect.Value) > Convert.ToInt32(this.LeastOther.TimeReconnect.Value))
        this.LeastOverall.TimeReconnect = this.LeastPhoenix.TimeReconnect;
      if (Convert.ToInt32(this.LeastOther.TimeReconnect.Value) > Convert.ToInt32(this.LeastPhoenix.TimeReconnect.Value) && Convert.ToInt32(this.LeastOther.TimeReconnect.Value) > Convert.ToInt32(this.LeastMI6.TimeReconnect.Value))
        this.LeastOverall.TimeReconnect = this.LeastOther.TimeReconnect;
      if (Convert.ToInt32(this.LeastMI6.Kills.Value) > Convert.ToInt32(this.LeastPhoenix.Kills.Value) && Convert.ToInt32(this.LeastMI6.Kills.Value) > Convert.ToInt32(this.LeastOther.Kills.Value))
        this.LeastOverall.Kills = this.LeastMI6.Kills;
      if (Convert.ToInt32(this.LeastPhoenix.Kills.Value) > Convert.ToInt32(this.LeastMI6.Kills.Value) && Convert.ToInt32(this.LeastPhoenix.Kills.Value) > Convert.ToInt32(this.LeastOther.Kills.Value))
        this.LeastOverall.Kills = this.LeastPhoenix.Kills;
      if (Convert.ToInt32(this.LeastOther.Kills.Value) > Convert.ToInt32(this.LeastPhoenix.Kills.Value) && Convert.ToInt32(this.LeastOther.Kills.Value) > Convert.ToInt32(this.LeastMI6.Kills.Value))
        this.LeastOverall.Kills = this.LeastOther.Kills;
      if (Convert.ToInt32(this.LeastMI6.Deaths.Value) > Convert.ToInt32(this.LeastPhoenix.Deaths.Value) && Convert.ToInt32(this.LeastMI6.Deaths.Value) > Convert.ToInt32(this.LeastOther.Deaths.Value))
        this.LeastOverall.Deaths = this.LeastMI6.Deaths;
      if (Convert.ToInt32(this.LeastPhoenix.Deaths.Value) > Convert.ToInt32(this.LeastMI6.Deaths.Value) && Convert.ToInt32(this.LeastPhoenix.Deaths.Value) > Convert.ToInt32(this.LeastOther.Deaths.Value))
        this.LeastOverall.Deaths = this.LeastPhoenix.Deaths;
      if (Convert.ToInt32(this.LeastOther.Deaths.Value) > Convert.ToInt32(this.LeastPhoenix.Deaths.Value) && Convert.ToInt32(this.LeastOther.Deaths.Value) > Convert.ToInt32(this.LeastMI6.Deaths.Value))
        this.LeastOverall.Deaths = this.LeastOther.Deaths;
      if (Convert.ToInt32(this.LeastMI6.Suicides.Value) > Convert.ToInt32(this.LeastPhoenix.Suicides.Value) && Convert.ToInt32(this.LeastMI6.Suicides.Value) > Convert.ToInt32(this.LeastOther.Suicides.Value))
        this.LeastOverall.Suicides = this.LeastMI6.Suicides;
      if (Convert.ToInt32(this.LeastPhoenix.Suicides.Value) > Convert.ToInt32(this.LeastMI6.Suicides.Value) && Convert.ToInt32(this.LeastPhoenix.Suicides.Value) > Convert.ToInt32(this.LeastOther.Suicides.Value))
        this.LeastOverall.Suicides = this.LeastPhoenix.Suicides;
      if (Convert.ToInt32(this.LeastOther.Suicides.Value) > Convert.ToInt32(this.LeastPhoenix.Suicides.Value) && Convert.ToInt32(this.LeastOther.Suicides.Value) > Convert.ToInt32(this.LeastMI6.Suicides.Value))
        this.LeastOverall.Suicides = this.LeastOther.Suicides;
      if (Convert.ToDouble(this.LeastMI6.KillDeathRatio.Value) > Convert.ToDouble(this.LeastPhoenix.KillDeathRatio.Value) && Convert.ToDouble(this.LeastMI6.KillDeathRatio.Value) > Convert.ToDouble(this.LeastOther.KillDeathRatio.Value))
        this.LeastOverall.KillDeathRatio = this.LeastMI6.KillDeathRatio;
      if (Convert.ToDouble(this.LeastPhoenix.KillDeathRatio.Value) > Convert.ToDouble(this.LeastMI6.KillDeathRatio.Value) && Convert.ToDouble(this.LeastPhoenix.KillDeathRatio.Value) > Convert.ToDouble(this.LeastOther.KillDeathRatio.Value))
        this.LeastOverall.KillDeathRatio = this.LeastPhoenix.KillDeathRatio;
      if (Convert.ToDouble(this.LeastOther.KillDeathRatio.Value) > Convert.ToDouble(this.LeastPhoenix.KillDeathRatio.Value) && Convert.ToDouble(this.LeastOther.KillDeathRatio.Value) > Convert.ToDouble(this.LeastMI6.KillDeathRatio.Value))
        this.LeastOverall.KillDeathRatio = this.LeastOther.KillDeathRatio;
      if (Convert.ToDouble(this.LeastMI6.KillsPerMinute.Value) > Convert.ToDouble(this.LeastPhoenix.KillsPerMinute.Value) && Convert.ToDouble(this.LeastMI6.KillsPerMinute.Value) > Convert.ToDouble(this.LeastOther.KillsPerMinute.Value))
        this.LeastOverall.KillsPerMinute = this.LeastMI6.KillsPerMinute;
      if (Convert.ToDouble(this.LeastPhoenix.KillsPerMinute.Value) > Convert.ToDouble(this.LeastMI6.KillsPerMinute.Value) && Convert.ToDouble(this.LeastPhoenix.KillsPerMinute.Value) > Convert.ToDouble(this.LeastOther.KillsPerMinute.Value))
        this.LeastOverall.KillsPerMinute = this.LeastPhoenix.KillsPerMinute;
      if (Convert.ToDouble(this.LeastOther.KillsPerMinute.Value) > Convert.ToDouble(this.LeastPhoenix.KillsPerMinute.Value) && Convert.ToDouble(this.LeastOther.KillsPerMinute.Value) > Convert.ToDouble(this.LeastMI6.KillsPerMinute.Value))
        this.LeastOverall.KillsPerMinute = this.LeastOther.KillsPerMinute;
      if (Convert.ToDouble(this.LeastMI6.DeathsPerMinute.Value) > Convert.ToDouble(this.LeastPhoenix.DeathsPerMinute.Value) && Convert.ToDouble(this.LeastMI6.DeathsPerMinute.Value) > Convert.ToDouble(this.LeastOther.DeathsPerMinute.Value))
        this.LeastOverall.DeathsPerMinute = this.LeastMI6.DeathsPerMinute;
      if (Convert.ToDouble(this.LeastPhoenix.DeathsPerMinute.Value) > Convert.ToDouble(this.LeastMI6.DeathsPerMinute.Value) && Convert.ToDouble(this.LeastPhoenix.DeathsPerMinute.Value) > Convert.ToDouble(this.LeastOther.DeathsPerMinute.Value))
        this.LeastOverall.DeathsPerMinute = this.LeastPhoenix.DeathsPerMinute;
      if (Convert.ToDouble(this.LeastOther.DeathsPerMinute.Value) > Convert.ToDouble(this.LeastPhoenix.DeathsPerMinute.Value) && Convert.ToDouble(this.LeastOther.DeathsPerMinute.Value) > Convert.ToDouble(this.LeastMI6.DeathsPerMinute.Value))
        this.LeastOverall.DeathsPerMinute = this.LeastOther.DeathsPerMinute;
      if (Convert.ToDouble(this.LeastMI6.SuicidesPerMinute.Value) > Convert.ToDouble(this.LeastPhoenix.SuicidesPerMinute.Value) && Convert.ToDouble(this.LeastMI6.SuicidesPerMinute.Value) > Convert.ToDouble(this.LeastOther.SuicidesPerMinute.Value))
        this.LeastOverall.SuicidesPerMinute = this.LeastMI6.SuicidesPerMinute;
      if (Convert.ToDouble(this.LeastPhoenix.SuicidesPerMinute.Value) > Convert.ToDouble(this.LeastMI6.SuicidesPerMinute.Value) && Convert.ToDouble(this.LeastPhoenix.SuicidesPerMinute.Value) > Convert.ToDouble(this.LeastOther.SuicidesPerMinute.Value))
        this.LeastOverall.SuicidesPerMinute = this.LeastPhoenix.SuicidesPerMinute;
      if (Convert.ToDouble(this.LeastOther.SuicidesPerMinute.Value) > Convert.ToDouble(this.LeastPhoenix.SuicidesPerMinute.Value) && Convert.ToDouble(this.LeastOther.SuicidesPerMinute.Value) > Convert.ToDouble(this.LeastMI6.SuicidesPerMinute.Value))
        this.LeastOverall.SuicidesPerMinute = this.LeastOther.SuicidesPerMinute;
      if (Convert.ToInt32(this.LeastMI6.CTFPoints.Value) > Convert.ToInt32(this.LeastPhoenix.CTFPoints.Value) && Convert.ToInt32(this.LeastMI6.CTFPoints.Value) > Convert.ToInt32(this.LeastOther.CTFPoints.Value))
        this.LeastOverall.CTFPoints = this.LeastMI6.CTFPoints;
      if (Convert.ToInt32(this.LeastPhoenix.CTFPoints.Value) > Convert.ToInt32(this.LeastMI6.CTFPoints.Value) && Convert.ToInt32(this.LeastPhoenix.CTFPoints.Value) > Convert.ToInt32(this.LeastOther.CTFPoints.Value))
        this.LeastOverall.CTFPoints = this.LeastPhoenix.CTFPoints;
      if (Convert.ToInt32(this.LeastOther.CTFPoints.Value) > Convert.ToInt32(this.LeastPhoenix.CTFPoints.Value) && Convert.ToInt32(this.LeastOther.CTFPoints.Value) > Convert.ToInt32(this.LeastMI6.CTFPoints.Value))
        this.LeastOverall.CTFPoints = this.LeastOther.CTFPoints;
      if (Convert.ToInt32(this.LeastMI6.CTFOffense.Value) > Convert.ToInt32(this.LeastPhoenix.CTFOffense.Value) && Convert.ToInt32(this.LeastMI6.CTFOffense.Value) > Convert.ToInt32(this.LeastOther.CTFOffense.Value))
        this.LeastOverall.CTFOffense = this.LeastMI6.CTFOffense;
      if (Convert.ToInt32(this.LeastPhoenix.CTFOffense.Value) > Convert.ToInt32(this.LeastMI6.CTFOffense.Value) && Convert.ToInt32(this.LeastPhoenix.CTFOffense.Value) > Convert.ToInt32(this.LeastOther.CTFOffense.Value))
        this.LeastOverall.CTFOffense = this.LeastPhoenix.CTFOffense;
      if (Convert.ToInt32(this.LeastOther.CTFOffense.Value) > Convert.ToInt32(this.LeastPhoenix.CTFOffense.Value) && Convert.ToInt32(this.LeastOther.CTFOffense.Value) > Convert.ToInt32(this.LeastMI6.CTFOffense.Value))
        this.LeastOverall.CTFOffense = this.LeastOther.CTFOffense;
      if (Convert.ToInt32(this.LeastMI6.CTFDefense.Value) > Convert.ToInt32(this.LeastPhoenix.CTFDefense.Value) && Convert.ToInt32(this.LeastMI6.CTFDefense.Value) > Convert.ToInt32(this.LeastOther.CTFDefense.Value))
        this.LeastOverall.CTFDefense = this.LeastMI6.CTFDefense;
      if (Convert.ToInt32(this.LeastPhoenix.CTFDefense.Value) > Convert.ToInt32(this.LeastMI6.CTFDefense.Value) && Convert.ToInt32(this.LeastPhoenix.CTFDefense.Value) > Convert.ToInt32(this.LeastOther.CTFDefense.Value))
        this.LeastOverall.CTFDefense = this.LeastPhoenix.CTFDefense;
      if (Convert.ToInt32(this.LeastOther.CTFDefense.Value) > Convert.ToInt32(this.LeastPhoenix.CTFDefense.Value) && Convert.ToInt32(this.LeastOther.CTFDefense.Value) > Convert.ToInt32(this.LeastMI6.CTFDefense.Value))
        this.LeastOverall.CTFDefense = this.LeastOther.CTFDefense;
      if (Convert.ToInt32(this.LeastMI6.TimeAccelerator.Value) > Convert.ToInt32(this.LeastPhoenix.TimeAccelerator.Value) && Convert.ToInt32(this.LeastMI6.TimeAccelerator.Value) > Convert.ToInt32(this.LeastOther.TimeAccelerator.Value))
        this.LeastOverall.TimeAccelerator = this.LeastMI6.TimeAccelerator;
      if (Convert.ToInt32(this.LeastPhoenix.TimeAccelerator.Value) > Convert.ToInt32(this.LeastMI6.TimeAccelerator.Value) && Convert.ToInt32(this.LeastPhoenix.TimeAccelerator.Value) > Convert.ToInt32(this.LeastOther.TimeAccelerator.Value))
        this.LeastOverall.TimeAccelerator = this.LeastPhoenix.TimeAccelerator;
      if (Convert.ToInt32(this.LeastOther.TimeAccelerator.Value) > Convert.ToInt32(this.LeastPhoenix.TimeAccelerator.Value) && Convert.ToInt32(this.LeastOther.TimeAccelerator.Value) > Convert.ToInt32(this.LeastMI6.TimeAccelerator.Value))
        this.LeastOverall.TimeAccelerator = this.LeastOther.TimeAccelerator;
      if (Convert.ToInt32(this.LeastMI6.TimeBackPack.Value) > Convert.ToInt32(this.LeastPhoenix.TimeBackPack.Value) && Convert.ToInt32(this.LeastMI6.TimeBackPack.Value) > Convert.ToInt32(this.LeastOther.TimeBackPack.Value))
        this.LeastOverall.TimeBackPack = this.LeastMI6.TimeBackPack;
      if (Convert.ToInt32(this.LeastPhoenix.TimeBackPack.Value) > Convert.ToInt32(this.LeastMI6.TimeBackPack.Value) && Convert.ToInt32(this.LeastPhoenix.TimeBackPack.Value) > Convert.ToInt32(this.LeastOther.TimeBackPack.Value))
        this.LeastOverall.TimeBackPack = this.LeastPhoenix.TimeBackPack;
      if (Convert.ToInt32(this.LeastOther.TimeBackPack.Value) > Convert.ToInt32(this.LeastPhoenix.TimeBackPack.Value) && Convert.ToInt32(this.LeastOther.TimeBackPack.Value) > Convert.ToInt32(this.LeastMI6.TimeBackPack.Value))
        this.LeastOverall.TimeBackPack = this.LeastOther.TimeBackPack;
      if (Convert.ToInt32(this.LeastMI6.TimePortableBattery.Value) > Convert.ToInt32(this.LeastPhoenix.TimePortableBattery.Value) && Convert.ToInt32(this.LeastMI6.TimePortableBattery.Value) > Convert.ToInt32(this.LeastOther.TimePortableBattery.Value))
        this.LeastOverall.TimePortableBattery = this.LeastMI6.TimePortableBattery;
      if (Convert.ToInt32(this.LeastPhoenix.TimePortableBattery.Value) > Convert.ToInt32(this.LeastMI6.TimePortableBattery.Value) && Convert.ToInt32(this.LeastPhoenix.TimePortableBattery.Value) > Convert.ToInt32(this.LeastOther.TimePortableBattery.Value))
        this.LeastOverall.TimePortableBattery = this.LeastPhoenix.TimePortableBattery;
      if (Convert.ToInt32(this.LeastOther.TimePortableBattery.Value) > Convert.ToInt32(this.LeastPhoenix.TimePortableBattery.Value) && Convert.ToInt32(this.LeastOther.TimePortableBattery.Value) > Convert.ToInt32(this.LeastMI6.TimePortableBattery.Value))
        this.LeastOverall.TimePortableBattery = this.LeastOther.TimePortableBattery;
      if (Convert.ToInt32(this.LeastMI6.TimeRegenerator.Value) > Convert.ToInt32(this.LeastPhoenix.TimeRegenerator.Value) && Convert.ToInt32(this.LeastMI6.TimeRegenerator.Value) > Convert.ToInt32(this.LeastOther.TimeRegenerator.Value))
        this.LeastOverall.TimeRegenerator = this.LeastMI6.TimeRegenerator;
      if (Convert.ToInt32(this.LeastPhoenix.TimeRegenerator.Value) > Convert.ToInt32(this.LeastMI6.TimeRegenerator.Value) && Convert.ToInt32(this.LeastPhoenix.TimeRegenerator.Value) > Convert.ToInt32(this.LeastOther.TimeRegenerator.Value))
        this.LeastOverall.TimeRegenerator = this.LeastPhoenix.TimeRegenerator;
      if (Convert.ToInt32(this.LeastOther.TimeRegenerator.Value) > Convert.ToInt32(this.LeastPhoenix.TimeRegenerator.Value) && Convert.ToInt32(this.LeastOther.TimeRegenerator.Value) > Convert.ToInt32(this.LeastMI6.TimeRegenerator.Value))
        this.LeastOverall.TimeRegenerator = this.LeastOther.TimeRegenerator;
      if (Convert.ToInt32(this.LeastMI6.TimeSpeed.Value) > Convert.ToInt32(this.LeastPhoenix.TimeSpeed.Value) && Convert.ToInt32(this.LeastMI6.TimeSpeed.Value) > Convert.ToInt32(this.LeastOther.TimeSpeed.Value))
        this.LeastOverall.TimeSpeed = this.LeastMI6.TimeSpeed;
      if (Convert.ToInt32(this.LeastPhoenix.TimeSpeed.Value) > Convert.ToInt32(this.LeastMI6.TimeSpeed.Value) && Convert.ToInt32(this.LeastPhoenix.TimeSpeed.Value) > Convert.ToInt32(this.LeastOther.TimeSpeed.Value))
        this.LeastOverall.TimeSpeed = this.LeastPhoenix.TimeSpeed;
      if (Convert.ToInt32(this.LeastOther.TimeSpeed.Value) <= Convert.ToInt32(this.LeastPhoenix.TimeSpeed.Value) || Convert.ToInt32(this.LeastOther.TimeSpeed.Value) <= Convert.ToInt32(this.LeastMI6.TimeSpeed.Value))
        return;
      this.LeastOverall.TimeSpeed = this.LeastOther.TimeSpeed;
    }

    public void RefreshThresholds()
    {
      this.GetThreshold(this.ScoreMI6, ref this.MostMI6, ref this.LeastMI6);
      this.GetThreshold(this.ScorePhoenix, ref this.MostPhoenix, ref this.LeastPhoenix);
      this.GetThreshold(this.ScoreOther, ref this.MostOther, ref this.LeastOther);
      this.GetOverallThreshold();
    }

    private void Loop()
    {
      this._IsStopped = false;
      int line = 1;
      long offset = 0;
      while (!this._Stop)
      {
        bool flag = false;
        try
        {
          FileInfo fileInfo = new FileInfo(this._Filename);
          if (fileInfo.Length > offset)
            flag = true;
          if (fileInfo.Length < offset)
          {
            offset = 0L;
            line = 1;
            flag = true;
          }
          if (flag)
          {
            FileStream fileStream = new FileStream(this._Filename, FileMode.Open, FileAccess.Read, FileShare.Write);
            fileStream.Seek(offset, SeekOrigin.Begin);
            StreamReader sr = new StreamReader((Stream) fileStream, this.ENCODING);
            while (sr.Peek() != -1)
            {
              JbnLib.Log.Types.DateData.DateData dateData = this.ToDateData(ref sr, ref line);
              if (dateData.Data.StartsWith("Log file started"))
              {
                this._TimeOpened = dateData.DateTime;
                List<string> data = this.ParseData(dateData.Data);
                this._OriginalFilename = data[0];
                this._Game = data[1];
                this._Version = data[2];
              }
              else if (!dateData.Data.StartsWith("Loading map") && !dateData.Data.StartsWith("Server variables:"))
              {
                if (dateData.Data.StartsWith("Server shutdown"))
                {
                  ServerShutdownHandler serverShutdown = this.ServerShutdown;
                  if (serverShutdown != null)
                    serverShutdown((object) this, new EventArgs());
                }
                else if (dateData.Data.StartsWith("Server cvar"))
                {
                  dateData.Data = dateData.Data.Replace("Server cvar ", "");
                  if (dateData.Data.StartsWith("\""))
                  {
                    List<string> data = this.ParseData(dateData.Data);
                    string name = data[0];
                    string newvalue = data[1];
                    for (int index = 0; index < this._Variables.Count; ++index)
                    {
                      Variable variable = this._Variables[index];
                      if (variable.Name == name)
                      {
                        this._VariableChanges.Add(new VariableChanged(dateData.DateTime, variable.Name, variable.Value, newvalue));
                        this._Variables.RemoveAt(index);
                      }
                    }
                    this._Variables.Add(new Variable(name, newvalue));
                  }
                  else
                  {
                    string[] strArray = LogFile.CleanDataSplit(dateData.Data);
                    this._Variables.Add(new Variable(strArray[0].Trim(), strArray[1].Trim()));
                  }
                }
                else if (dateData.Data.StartsWith("Server say"))
                {
                  dateData.Data = dateData.Data.Replace("Server say \"say ", "").Replace("Server say \"say_team ", "");
                  dateData.Data = dateData.Data.TrimEnd('"', ' ');
                  this._SayServer.Add(new JbnLib.Log.Types.Say.SayServer(dateData.DateTime, dateData.Data));
                }
                else if (dateData.Data.StartsWith("Started map"))
                {
                  List<string> data = this.ParseData(dateData.Data);
                  this._Map = Convert.ToString(data[0]);
                  MapChangedHandler mapChanged = this.MapChanged;
                  if (mapChanged != null)
                    mapChanged((object) this, new MapChangedEventArgs(Convert.ToString(data[0])));
                  this._CRC = Convert.ToInt32(data[1]);
                }
                else if (dateData.Data.StartsWith("\""))
                {
                  string[] strArray1 = dateData.Data.Split(new char[1]
                  {
                    '"'
                  }, StringSplitOptions.RemoveEmptyEntries);
                  int num1 = this._Players.IndexOf(strArray1[0]);
                  if (num1 != -1)
                  {
                    switch (strArray1[1])
                    {
                      case " changed name to ":
                        JbnLib.Log.Types.DateID.ChangedName changedName = new JbnLib.Log.Types.DateID.ChangedName(dateData.DateTime, num1, this._Players[num1].Name, strArray1[2]);
                        this._Players[num1].Name = strArray1[2];
                        this._ChangedName.Add(changedName);
                        continue;
                      case " changed role to ":
                        this._ChangedRole.Add(new JbnLib.Log.Types.DateID.ChangedRole(dateData.DateTime, num1, strArray1[2]));
                        this._Players[num1].Character = strArray1[2];
                        continue;
                      case " committed suicide with ":
                        this._Suicides.Add(new Suicide(dateData.DateTime, num1, strArray1[2]));
                        --this._Players[num1].Wins;
                        ++this._Players[num1].Losses;
                        continue;
                      case " connected, address ":
                        this._Connected.Add(new JbnLib.Log.Types.DateID.Connected(dateData.DateTime, num1, strArray1[2]));
                        if (strArray1[2].IndexOf(':') != -1)
                        {
                          string[] strArray2 = strArray1[2].Split(':');
                          this._Players[num1].IP = strArray2[0];
                        }
                        else
                          this._Players[num1].IP = strArray1[2];
                        this._Players[num1].IsConnecting = true;
                        continue;
                      case " disconnected":
                        this._Disconnected.Add(new JbnLib.Log.Types.DateID.Disconnected(dateData.DateTime, num1));
                        this._Players[num1].InGame = false;
                        continue;
                      case " entered the game":
                        this._EnteredGame.Add(new JbnLib.Log.Types.DateID.EnteredGame(dateData.DateTime, num1));
                        this._Players[num1].IsConnecting = false;
                        this._Players[num1].InGame = true;
                        continue;
                      case " joined team ":
                        this._Players[num1].Team = LogFile.ToTeam(strArray1[2]);
                        this._JoinedTeam.Add(new JbnLib.Log.Types.DateIDTeam.JoinedTeam(dateData.DateTime, num1, LogFile.ToTeam(strArray1[2])));
                        continue;
                      case " killed ":
                        int num2 = this._Players.IndexOf(strArray1[2]);
                        if (this._Players[num1].Team == this._Players[num2].Team)
                          this._Killed.Add(new Killed(dateData.DateTime, num1, this._Players[num1].Team, num2, strArray1[4], true));
                        else
                          this._Killed.Add(new Killed(dateData.DateTime, num1, this._Players[num1].Team, num2, strArray1[4], false));
                        ++this._Players[num1].Wins;
                        ++this._Players[num2].Losses;
                        continue;
                      case " say ":
                        this._SayGeneral.Add(new JbnLib.Log.Types.Say.SayGeneral(dateData.DateTime, num1, strArray1[2]));
                        continue;
                      case " say_team ":
                        switch (this._Players[num1].Team)
                        {
                          case Team.MI6:
                            this._SayMI6.Add(new JbnLib.Log.Types.Say.SayMI6(dateData.DateTime, num1, strArray1[2]));
                            continue;
                          case Team.Phoenix:
                            this._SayPhoenix.Add(new JbnLib.Log.Types.Say.SayPhoenix(dateData.DateTime, num1, strArray1[2]));
                            continue;
                          default:
                            continue;
                        }
                      case " scored ":
                        continue;
                      case " triggered ":
                        if (strArray1.Length == 3)
                        {
                          this._Triggered.Add(new TriggeredEvent(dateData.DateTime, num1, this._Players[num1].Team, strArray1[2]));
                          switch (strArray1[2])
                          {
                            case "CapturedFlag":
                              ++this._Players[num1].Points;
                              continue;
                            case "LostFlag":
                              this._Players[num1].HasFlag = false;
                              continue;
                            case "TookFlag":
                              this._Players[num1].HasFlag = true;
                              continue;
                            case "drop_Ammo_Powerup":
                            case "drop_Battery_Powerup":
                            case "drop_Damage_Powerup":
                            case "drop_Health_Powerup":
                            case "drop_Speed_Powerup":
                              this.Players[num1].PowerUp = PowerUp.None;
                              continue;
                            case "take_Ammo_Powerup":
                              this.Players[num1].PowerUp = PowerUp.BackPack;
                              continue;
                            case "take_Battery_Powerup":
                              this.Players[num1].PowerUp = PowerUp.PortableBattery;
                              continue;
                            case "take_Damage_Powerup":
                              this.Players[num1].PowerUp = PowerUp.Accelerator;
                              continue;
                            case "take_Health_Powerup":
                              this.Players[num1].PowerUp = PowerUp.Regeneration;
                              continue;
                            case "take_Speed_Powerup":
                              this.Players[num1].PowerUp = PowerUp.Speed;
                              continue;
                            default:
                              continue;
                          }
                        }
                        else
                        {
                          this._TriggeredAgainst.Add(new TriggeredAgainstEvent(dateData.DateTime, num1, this._Players[num1].Team, strArray1[2], this._Players.IndexOf(strArray1[4])));
                          continue;
                        }
                      default:
                        if (dateData.DateTime != new DateTime(0L))
                        {
                          this._Unknown.Add(dateData.Data);
                          continue;
                        }
                        continue;
                    }
                  }
                }
                else if (dateData.Data.StartsWith("Rcon:"))
                {
                  List<string> data = this.ParseData(dateData.Data);
                  this._RCon.Add(new JbnLib.Log.Types.RCon(dateData.DateTime, data[1].Trim(), data[2]));
                }
                else if (dateData.Data.StartsWith("World triggered \"Draw\""))
                  this._WinningTeam = Team.Draw;
                else if (dateData.Data.StartsWith("World triggered"))
                {
                  List<string> data = this.ParseData(dateData.Data);
                  this._WorldTriggered.Add(new WorldTriggered(dateData.DateTime, data[0]));
                }
                else if (dateData.Data.StartsWith("Team"))
                {
                  List<string> data = this.ParseData(dateData.Data);
                  if (data.Count == 2)
                  {
                    switch (data[0])
                    {
                      case "Phoenix":
                        this._WinningTeam = Team.Phoenix;
                        continue;
                      case "MI6":
                        this._WinningTeam = Team.MI6;
                        continue;
                      default:
                        this._WinningTeam = Team.Unknown;
                        continue;
                    }
                  }
                }
                else if (!dateData.Data.StartsWith("//"))
                {
                  if (dateData.Data.StartsWith("Log file closed"))
                    this._TimeClosed = dateData.DateTime;
                  else if (dateData.DateTime != new DateTime(0L))
                    this._Unknown.Add(dateData.Data);
                }
              }
            }
            offset = new FileInfo(this._Filename).Length;
            sr.Close();
            fileStream.Close();
          }
        }
        catch
        {
        }
        if (!this._Loop)
          this._Stop = true;
        if (!this._Stop)
          Thread.Sleep(this._Delay);
      }
      this._IsStopped = true;
    }

    public int Delay
    {
      get => this._Delay;
      set => this._Delay = value;
    }

    public ThreadPriority Priority
    {
      get => this._Looper.Priority;
      set => this._Looper.Priority = value;
    }

    public bool IsStopped => this._IsStopped;

    public void Start()
    {
      this._Stop = false;
      if (this._Loop)
      {
        this._Looper = new Thread(new ThreadStart(this.Loop));
        this._Looper.Start();
      }
      else
        this.Loop();
    }

    public void Stop() => this._Stop = true;

    public void Abort() => this._Looper.Abort();

    public static int TimeToInteger(string time)
    {
      string[] strArray = time.Split(':');
      return Convert.ToInt32(strArray[0]) * 60 + Convert.ToInt32(strArray[1]);
    }

    public static string FilePathToName(string path)
    {
      FileInfo fileInfo = new FileInfo(path);
      return fileInfo.Name.Replace(fileInfo.Extension, "");
    }

    public static Team ToTeam(string team)
    {
      switch (team)
      {
        case "MI6":
          return Team.MI6;
        case "Phoenix":
          return Team.Phoenix;
        default:
          return Team.NotApplicable;
      }
    }

    public static PowerUp ToPowerUp(string powerup)
    {
      switch (powerup)
      {
        case "take_Damage_Powerup":
          return PowerUp.Accelerator;
        case "take_Ammo_Powerup":
          return PowerUp.BackPack;
        case "take_Battery_Powerup":
          return PowerUp.PortableBattery;
        case "take_Health_Powerup":
          return PowerUp.Regeneration;
        case "take_Speed_Powerup":
          return PowerUp.Speed;
        default:
          return PowerUp.None;
      }
    }

    private JbnLib.Log.Types.DateData.DateData ToDateData(ref StreamReader sr, ref int line)
    {
      string str = sr.ReadLine();
      ++line;
      JbnLib.Log.Types.DateData.DateData dateData1 = new JbnLib.Log.Types.DateData.DateData(new DateTime(0L), "");
      if (str.StartsWith("L "))
      {
        string[] strArray = str.Split(' ');
        string s = (strArray[1] + " " + strArray[3]).TrimEnd(':');
        try
        {
          dateData1.DateTime = DateTime.Parse(s, (IFormatProvider) this.CULTURE);
        }
        catch
        {
          this.Errors.Add("[ToDateData] DateTime.Parse: " + s);
          dateData1.DateTime = new DateTime(0L);
          dateData1.Data = str;
          return dateData1;
        }
        dateData1.Data = strArray[4];
        for (int index = 5; index < strArray.Length; ++index)
        {
          JbnLib.Log.Types.DateData.DateData dateData2 = dateData1;
          dateData2.Data = dateData2.Data + " " + strArray[index];
        }
        int num = sr.Peek();
        while (num != 76)
        {
          dateData1.Data += sr.ReadLine();
          num = sr.Peek();
          if (num == -1)
            num = 76;
        }
        return dateData1;
      }
      dateData1.Data = str;
      return dateData1;
    }

    private List<string> ParseData(string data)
    {
      int start = -1;
      List<string> data1 = new List<string>();
      byte[] bytes = Encoding.ASCII.GetBytes(data);
      for (int index = 0; index < bytes.Length; ++index)
      {
        if (bytes[index] == (byte) 34 && start == -1)
          start = index + 1;
        else if (bytes[index] == (byte) 34 && start > -1)
        {
          data1.Add(this.BytesToString(bytes, start, index - 1));
          start = -1;
        }
      }
      return data1;
    }

    private string BytesToString(byte[] bytes, int start, int stop)
    {
      byte[] bytes1 = new byte[stop - start + 1];
      int index1 = 0;
      for (int index2 = start; index2 <= stop; ++index2)
      {
        bytes1[index1] = bytes[index2];
        ++index1;
      }
      return Encoding.ASCII.GetString(bytes1);
    }

    private static string[] CleanDataSplit(string data)
    {
      string[] strArray = data.Split(':');
      if (strArray.Length > 2)
      {
        for (int index = 2; index < strArray.Length; ++index)
        {
          ref string local = ref strArray[1];
          local = local + ":" + strArray[index];
        }
      }
      return strArray;
    }

    private static double KillDeathRatio(int kills, int deaths)
    {
      double num1 = Convert.ToDouble(kills);
      double num2 = Convert.ToDouble(deaths);
      if (num1 == 0.0 && num2 == 0.0)
        return 0.0;
      return num2 == 0.0 ? num1 : Math.Round(num1 / num2, 2);
    }

    private static double KillsPerMinute(int kills, int seconds)
    {
      double num1 = Convert.ToDouble(kills);
      double num2 = Convert.ToDouble(seconds);
      if (num1 == 0.0 && num2 == 0.0)
        return 0.0;
      return num2 == 0.0 ? num1 : Math.Round(num1 / (num2 / 60.0), 2);
    }

    public void Clear()
    {
      this._ChangedName.Clear();
      this._ChangedRole.Clear();
      this._Connected.Clear();
      this._Disconnected.Clear();
      this._EnteredGame.Clear();
      this._JoinedTeam.Clear();
      this._Killed.Clear();
      this._RCon.Clear();
      this._SayGeneral.Clear();
      this._SayMI6.Clear();
      this._SayPhoenix.Clear();
      this._SayServer.Clear();
      this._Suicides.Clear();
      this._Triggered.Clear();
      this._TriggeredAgainst.Clear();
      this._VariableChanges.Clear();
      this._Variables.Clear();
      this._WorldTriggered.Clear();
    }

    public void ClearPlayers() => this._Players.Clear();

    public void ClearErrors() => this._Errors.Clear();

    public void ClearUnknowns() => this._Unknown.Clear();

    public SortableBindingList<string> Errors => this._Errors;

    public string[] GetErrors()
    {
      string[] array = new string[this._Errors.Count];
      this._Errors.CopyTo(array, 0);
      return array;
    }

    public SortableBindingList<string> Unknowns => this._Unknown;

    public string[] GetUnknowns()
    {
      string[] array = new string[this._Unknown.Count];
      this._Unknown.CopyTo(array, 0);
      return array;
    }

    public void DumpHtml(string path, HtmlOutput options, string css)
    {
      this.RefreshScores();
      this.RefreshReconnectScores();
      this.RefreshThresholds();
      FileStream fileStream = new FileStream(path, FileMode.Create, FileAccess.Write);
      StreamWriter streamWriter = new StreamWriter((Stream) fileStream, this.ENCODING);
      FileInfo fileInfo = new FileInfo(this._Filename);
      streamWriter.WriteLine("<HTML>");
      streamWriter.WriteLine("\t<HEAD>");
      streamWriter.WriteLine("\t\t<TITLE>Log Parser - " + fileInfo.Name + "</TITLE>");
      streamWriter.WriteLine("\t\t<META http-equiv=\"Content-Type\" content=\"text/html;charset=UTF-8\">");
      if (css.Length == 0)
      {
        streamWriter.WriteLine("\t\t<STYLE type=\"text/css\">");
        streamWriter.WriteLine("\t\t\ta {cursor: pointer; text-decoration: underline; color: #0000FF;}");
        streamWriter.WriteLine("\t\t\tdiv {display: none;}");
        streamWriter.WriteLine("\t\t\tth {border-style: dotted;}");
        streamWriter.WriteLine("\t\t\ttd, table {border-style: dashed;}");
        streamWriter.WriteLine("\t\t\t.mi6 {color: #0000FF}");
        streamWriter.WriteLine("\t\t\t.mi6head {color: #0000FF; background-color: #F0F0F0}");
        streamWriter.WriteLine("\t\t\t.phx {color: #FF0000}");
        streamWriter.WriteLine("\t\t\t.phxhead {color: #FF0000; background-color: #F0F0F0}");
        streamWriter.WriteLine("\t\t\t.oth {color: #000000}");
        streamWriter.WriteLine("\t\t\t.othhead {color: #000000; background-color: #F0F0F0}");
        streamWriter.WriteLine("\t\t</STYLE>");
      }
      else
        streamWriter.WriteLine("\t\t<LINK rel=\"stylesheet\" type=\"text/css\" href=\"" + css + "\" />");
      streamWriter.WriteLine("\t\t<SCRIPT type=\"text/javascript\">");
      streamWriter.WriteLine("\t\t\tfunction getElementByClass(classname) {");
      streamWriter.WriteLine("\t\t\t\tpartscollect=new Array()");
      streamWriter.WriteLine("\t\t\t\tvar inc=0");
      streamWriter.WriteLine("\t\t\t\tvar alltags=document.getElementsByTagName(\"TR\")");
      streamWriter.WriteLine("\t\t\t\tfor (i=0; i<alltags.length; i++){");
      streamWriter.WriteLine("\t\t\t\t\tif (alltags[i].className==classname) {");
      streamWriter.WriteLine("\t\t\t\t\t\tpartscollect[inc++]=alltags[i]");
      streamWriter.WriteLine("\t\t\t\t\t}");
      streamWriter.WriteLine("\t\t\t\t}");
      streamWriter.WriteLine("\t\t\t\treturn partscollect");
      streamWriter.WriteLine("\t\t\t}");
      streamWriter.WriteLine("\t\t</SCRIPT>");
      streamWriter.WriteLine("\t\t<SCRIPT type=\"text/javascript\">");
      streamWriter.WriteLine("\t\t\tfunction toggle_visibility(id) {");
      streamWriter.WriteLine("\t\t\t\tvar e = document.getElementById(id);");
      streamWriter.WriteLine("\t\t\t\tif(e.style.display == 'none' || e.style.display == '')");
      streamWriter.WriteLine("\t\t\t\t\te.style.display = 'block';");
      streamWriter.WriteLine("\t\t\t\telse");
      streamWriter.WriteLine("\t\t\t\t\te.style.display = 'none';");
      streamWriter.WriteLine("\t\t\t}");
      streamWriter.WriteLine("\t\t</SCRIPT>");
      streamWriter.WriteLine("\t\t<SCRIPT type=\"text/javascript\">");
      streamWriter.WriteLine("\t\t\tfunction toggle_visibility_class(classname) {");
      streamWriter.WriteLine("\t\t\t\tvar items = getElementByClass(classname);");
      streamWriter.WriteLine("\t\t\t\tfor (e in items){");
      streamWriter.WriteLine("\t\t\t\t\tif(items[e].style.display == 'none' || items[e].style.display == '')");
      streamWriter.WriteLine("\t\t\t\t\t\titems[e].style.display = 'block';");
      streamWriter.WriteLine("\t\t\t\t\telse");
      streamWriter.WriteLine("\t\t\t\t\t\titems[e].style.display = 'none';");
      streamWriter.WriteLine("\t\t\t\t}");
      streamWriter.WriteLine("\t\t\t}");
      streamWriter.WriteLine("\t\t</SCRIPT>");
      streamWriter.WriteLine("\t</HEAD>");
      streamWriter.WriteLine("\t<BODY>");
      streamWriter.Flush();
      if ((options & HtmlOutput.LogDetails) == HtmlOutput.LogDetails)
      {
        streamWriter.WriteLine("\t\t<B>Original Filename:</B> {0}<BR />", (object) this._OriginalFilename);
        streamWriter.WriteLine("\t\t<B>Processed Filename:</B> {0}<BR />", (object) this._Filename);
        streamWriter.WriteLine("\t\t<HR />");
        streamWriter.WriteLine("\t\t<B>Map:</B> {0} (CRC: {1})<BR />", (object) this._Map, (object) this._CRC);
        streamWriter.WriteLine("\t\t<B>Game:</B> {0}<BR />", (object) this._Game);
        streamWriter.WriteLine("\t\t<B>Version:</B> {0}<BR />", (object) this._Version);
        streamWriter.WriteLine("\t\t<HR />");
        streamWriter.WriteLine("\t\t<B>Start:</B> {0}<BR />", (object) this._TimeOpened);
        streamWriter.WriteLine("\t\t<B>Stop:</B> {0}<BR />", (object) this._TimeClosed);
        TimeSpan timeSpan = this._TimeClosed - this._TimeOpened;
        streamWriter.WriteLine("\t\t<B>Duration:</B> {0}<BR />", (object) timeSpan.Duration());
        streamWriter.WriteLine("\t\t<HR />");
        streamWriter.WriteLine("\t\t<B>Winning Team:</B> {0}<BR />", (object) this.WinningTeamToHTML());
        streamWriter.Flush();
      }
      if ((options & HtmlOutput.OverallDetails) == HtmlOutput.OverallDetails)
      {
        streamWriter.WriteLine("\t\t<H2><A onclick=\"toggle_visibility('overalldetails')\">Overall Details</A></H2>");
        streamWriter.WriteLine("\t\t<DIV id=\"overalldetails\">");
        streamWriter.WriteLine("\t\t\t<TABLE cellpadding=0 cellspacing=0 border=1>");
        streamWriter.WriteLine("\t\t\t\t<TR>");
        streamWriter.WriteLine("\t\t\t\t\t<TH>Name</TH><TH>Score</TH><TH>Points</TH><TH>Offense</TH><TH>Defense</TH><TH><SUP>K</SUP>/<SUB>D</SUB></TH><TH>Kills</TH><TH><SUP>K</SUP>/<SUB>min</SUB></TH><TH>Deaths</TH><TH><SUP>D</SUP>/<SUB>min</SUB></TH><TH>Suicides</TH><TH><SUP>S</SUP>/<SUB>min</SUB></TH><TH>Accelerator</TH><TH>BackPack</TH><TH>PortableBattery</TH><TH>Regenerator</TH><TH>Speed</TH><TH>Connected</TH><TH>Reconnecting</TH>");
        streamWriter.WriteLine("\t\t\t\t</TR>");
        Score teamScore1 = LogFile.GetTeamScore(this.ScoreMI6, "[TEAM MI6]");
        streamWriter.WriteLine("\t\t\t\t<TR class=\"mi6head\">");
        streamWriter.WriteLine("\t\t\t\t\t<TD nowrap>{0}</TD><TD>{1}</TD><TD>{2}</TD><TD>{3}</TD><TD>{4}</TD><TD>{5}</TD><TD>{6}</TD><TD>{7}</TD><TD>{8}</TD><TD>{9}</TD><TD>{10}</TD><TD>{11}</TD><TD>{12}</TD><TD>{13}</TD><TD>{14}</TD><TD>{15}</TD><TD>{16}</TD><TD>{17}</TD><TD>{18}</TD>", (object) teamScore1.Name, (object) teamScore1.Scored, (object) teamScore1.CTFPoints, (object) teamScore1.CTFOffense, (object) teamScore1.CTFDefense, (object) teamScore1.KillDeathRatio, (object) teamScore1.Kills, (object) teamScore1.KillsPerMinute, (object) teamScore1.Deaths, (object) teamScore1.DeathsPerMinute, (object) teamScore1.Suicides, (object) teamScore1.SuicidesPerMinute, (object) teamScore1.TimeAccelerator, (object) teamScore1.TimeBackPack, (object) teamScore1.TimePortableBattery, (object) teamScore1.TimeRegenerator, (object) teamScore1.TimeSpeed, (object) teamScore1.TimeConnected, (object) teamScore1.TimeReconnect);
        streamWriter.WriteLine("\t\t\t\t</TR>");
        foreach (Score score in this.ScoreMI6)
        {
          streamWriter.WriteLine("\t\t\t\t<TR class=\"mi6\">");
          streamWriter.WriteLine("\t\t\t\t\t<TD nowrap>{0}</TD><TD>{1}</TD><TD>{2}</TD><TD>{3}</TD><TD>{4}</TD><TD>{5}</TD><TD>{6}</TD><TD>{7}</TD><TD>{8}</TD><TD>{9}</TD><TD>{10}</TD><TD>{11}</TD><TD>{12}</TD><TD>{13}</TD><TD>{14}</TD><TD>{15}</TD><TD>{16}</TD><TD>{17}</TD><TD>{18}</TD>", (object) score.Name, (object) score.Scored, (object) score.CTFPoints, (object) score.CTFOffense, (object) score.CTFDefense, (object) score.KillDeathRatio, (object) score.Kills, (object) score.KillsPerMinute, (object) score.Deaths, (object) score.DeathsPerMinute, (object) score.Suicides, (object) score.SuicidesPerMinute, (object) score.TimeAccelerator, (object) score.TimeBackPack, (object) score.TimePortableBattery, (object) score.TimeRegenerator, (object) score.TimeSpeed, (object) score.TimeConnected, (object) score.TimeReconnect);
          streamWriter.WriteLine("\t\t\t\t</TR>");
        }
        streamWriter.Flush();
        Score teamScore2 = LogFile.GetTeamScore(this.ScorePhoenix, "[TEAM PHOENIX]");
        streamWriter.WriteLine("\t\t\t\t<TR class=\"phxhead\">");
        streamWriter.WriteLine("\t\t\t\t\t<TD nowrap>{0}</TD><TD>{1}</TD><TD>{2}</TD><TD>{3}</TD><TD>{4}</TD><TD>{5}</TD><TD>{6}</TD><TD>{7}</TD><TD>{8}</TD><TD>{9}</TD><TD>{10}</TD><TD>{11}</TD><TD>{12}</TD><TD>{13}</TD><TD>{14}</TD><TD>{15}</TD><TD>{16}</TD><TD>{17}</TD><TD>{18}</TD>", (object) teamScore2.Name, (object) teamScore2.Scored, (object) teamScore2.CTFPoints, (object) teamScore2.CTFOffense, (object) teamScore2.CTFDefense, (object) teamScore2.KillDeathRatio, (object) teamScore2.Kills, (object) teamScore2.KillsPerMinute, (object) teamScore2.Deaths, (object) teamScore2.DeathsPerMinute, (object) teamScore2.Suicides, (object) teamScore2.SuicidesPerMinute, (object) teamScore2.TimeAccelerator, (object) teamScore2.TimeBackPack, (object) teamScore2.TimePortableBattery, (object) teamScore2.TimeRegenerator, (object) teamScore2.TimeSpeed, (object) teamScore2.TimeConnected, (object) teamScore2.TimeReconnect);
        streamWriter.WriteLine("\t\t\t\t</TR>");
        foreach (Score score in this.ScorePhoenix)
        {
          streamWriter.WriteLine("\t\t\t\t<TR class=\"phx\">");
          streamWriter.WriteLine("\t\t\t\t\t<TD nowrap>{0}</TD><TD>{1}</TD><TD>{2}</TD><TD>{3}</TD><TD>{4}</TD><TD>{5}</TD><TD>{6}</TD><TD>{7}</TD><TD>{8}</TD><TD>{9}</TD><TD>{10}</TD><TD>{11}</TD><TD>{12}</TD><TD>{13}</TD><TD>{14}</TD><TD>{15}</TD><TD>{16}</TD><TD>{17}</TD><TD>{18}</TD>", (object) score.Name, (object) score.Scored, (object) score.CTFPoints, (object) score.CTFOffense, (object) score.CTFDefense, (object) score.KillDeathRatio, (object) score.Kills, (object) score.KillsPerMinute, (object) score.Deaths, (object) score.DeathsPerMinute, (object) score.Suicides, (object) score.SuicidesPerMinute, (object) score.TimeAccelerator, (object) score.TimeBackPack, (object) score.TimePortableBattery, (object) score.TimeRegenerator, (object) score.TimeSpeed, (object) score.TimeConnected, (object) score.TimeReconnect);
          streamWriter.WriteLine("\t\t\t\t</TR>");
        }
        streamWriter.Flush();
        Score teamScore3 = LogFile.GetTeamScore(this.ScoreOther, "[TEAM OTHER]");
        streamWriter.WriteLine("\t\t\t\t<TR class=\"othhead\">");
        streamWriter.WriteLine("\t\t\t\t\t<TD nowrap>{0}</TD><TD>{1}</TD><TD>{2}</TD><TD>{3}</TD><TD>{4}</TD><TD>{5}</TD><TD>{6}</TD><TD>{7}</TD><TD>{8}</TD><TD>{9}</TD><TD>{10}</TD><TD>{11}</TD><TD>{12}</TD><TD>{13}</TD><TD>{14}</TD><TD>{15}</TD><TD>{16}</TD><TD>{17}</TD><TD>{18}</TD>", (object) teamScore3.Name, (object) teamScore3.Scored, (object) teamScore3.CTFPoints, (object) teamScore3.CTFOffense, (object) teamScore3.CTFDefense, (object) teamScore3.KillDeathRatio, (object) teamScore3.Kills, (object) teamScore3.KillsPerMinute, (object) teamScore3.Deaths, (object) teamScore3.DeathsPerMinute, (object) teamScore3.Suicides, (object) teamScore3.SuicidesPerMinute, (object) teamScore3.TimeAccelerator, (object) teamScore3.TimeBackPack, (object) teamScore3.TimePortableBattery, (object) teamScore3.TimeRegenerator, (object) teamScore3.TimeSpeed, (object) teamScore3.TimeConnected, (object) teamScore3.TimeReconnect);
        streamWriter.WriteLine("\t\t\t\t</TR>");
        foreach (Score score in this.ScoreOther)
        {
          streamWriter.WriteLine("\t\t\t\t<TR class=\"oth\">");
          streamWriter.WriteLine("\t\t\t\t\t<TD nowrap>{0}</TD><TD>{1}</TD><TD>{2}</TD><TD>{3}</TD><TD>{4}</TD><TD>{5}</TD><TD>{6}</TD><TD>{7}</TD><TD>{8}</TD><TD>{9}</TD><TD>{10}</TD><TD>{11}</TD><TD>{12}</TD><TD>{13}</TD><TD>{14}</TD><TD>{15}</TD><TD>{16}</TD><TD>{17}</TD><TD>{18}</TD>", (object) score.Name, (object) score.Scored, (object) score.CTFPoints, (object) score.CTFOffense, (object) score.CTFDefense, (object) score.KillDeathRatio, (object) score.Kills, (object) score.KillsPerMinute, (object) score.Deaths, (object) score.DeathsPerMinute, (object) score.Suicides, (object) score.SuicidesPerMinute, (object) score.TimeAccelerator, (object) score.TimeBackPack, (object) score.TimePortableBattery, (object) score.TimeRegenerator, (object) score.TimeSpeed, (object) score.TimeConnected, (object) score.TimeReconnect);
          streamWriter.WriteLine("\t\t\t\t</TR>");
        }
        streamWriter.WriteLine("\t\t\t</TABLE>");
        streamWriter.WriteLine("\t\t</DIV>");
        streamWriter.Flush();
      }
      if ((options & HtmlOutput.ReconnectDetails) == HtmlOutput.ReconnectDetails)
      {
        streamWriter.WriteLine("\t\t<H2><A onclick=\"toggle_visibility('reconnectdetails')\">Reconnect Details</A></H2>");
        streamWriter.WriteLine("\t\t<DIV id=\"reconnectdetails\">");
        streamWriter.WriteLine("\t\t\t<TABLE cellpadding=0 cellspacing=0 border=1>");
        streamWriter.WriteLine("\t\t\t\t<TR>");
        streamWriter.WriteLine("\t\t\t\t\t<TH>Name</TH><TH>Score</TH><TH>Points</TH><TH>Offense</TH><TH>Defense</TH><TH><SUP>K</SUP>/<SUB>D</SUB></TH><TH>Kills</TH><TH><SUP>K</SUP>/<SUB>min</SUB></TH><TH>Deaths</TH><TH><SUP>D</SUP>/<SUB>min</SUB></TH><TH>Suicides</TH><TH><SUP>S</SUP>/<SUB>min</SUB></TH><TH>Accelerator</TH><TH>BackPack</TH><TH>PortableBattery</TH><TH>Regenerator</TH><TH>Speed</TH><TH>Connected</TH><TH>Reconnecting</TH>");
        streamWriter.WriteLine("\t\t\t\t</TR>");
        Score teamScore4 = LogFile.GetTeamScore(this.ReconnectMI6, "[TEAM MI6]");
        streamWriter.WriteLine("\t\t\t\t<TR class=\"mi6head\">");
        streamWriter.WriteLine("\t\t\t\t\t<TD nowrap>{0}</TD><TD>{1}</TD><TD>{2}</TD><TD>{3}</TD><TD>{4}</TD><TD>{5}</TD><TD>{6}</TD><TD>{7}</TD><TD>{8}</TD><TD>{9}</TD><TD>{10}</TD><TD>{11}</TD><TD>{12}</TD><TD>{13}</TD><TD>{14}</TD><TD>{15}</TD><TD>{16}</TD><TD>{17}</TD><TD>{18}</TD>", (object) teamScore4.Name, (object) teamScore4.Scored, (object) teamScore4.CTFPoints, (object) teamScore4.CTFOffense, (object) teamScore4.CTFDefense, (object) teamScore4.KillDeathRatio, (object) teamScore4.Kills, (object) teamScore4.KillsPerMinute, (object) teamScore4.Deaths, (object) teamScore4.DeathsPerMinute, (object) teamScore4.Suicides, (object) teamScore4.SuicidesPerMinute, (object) teamScore4.TimeAccelerator, (object) teamScore4.TimeBackPack, (object) teamScore4.TimePortableBattery, (object) teamScore4.TimeRegenerator, (object) teamScore4.TimeSpeed, (object) teamScore4.TimeConnected, (object) teamScore4.TimeReconnect);
        streamWriter.WriteLine("\t\t\t\t</TR>");
        foreach (Score score in this.ReconnectMI6)
        {
          streamWriter.WriteLine("\t\t\t\t<TR class=\"mi6\">");
          streamWriter.WriteLine("\t\t\t\t\t<TD nowrap>{0}</TD><TD>{1}</TD><TD>{2}</TD><TD>{3}</TD><TD>{4}</TD><TD>{5}</TD><TD>{6}</TD><TD>{7}</TD><TD>{8}</TD><TD>{9}</TD><TD>{10}</TD><TD>{11}</TD><TD>{12}</TD><TD>{13}</TD><TD>{14}</TD><TD>{15}</TD><TD>{16}</TD><TD>{17}</TD><TD>{18}</TD>", (object) score.Name, (object) score.Scored, (object) score.CTFPoints, (object) score.CTFOffense, (object) score.CTFDefense, (object) score.KillDeathRatio, (object) score.Kills, (object) score.KillsPerMinute, (object) score.Deaths, (object) score.DeathsPerMinute, (object) score.Suicides, (object) score.SuicidesPerMinute, (object) score.TimeAccelerator, (object) score.TimeBackPack, (object) score.TimePortableBattery, (object) score.TimeRegenerator, (object) score.TimeSpeed, (object) score.TimeConnected, (object) score.TimeReconnect);
          streamWriter.WriteLine("\t\t\t\t</TR>");
        }
        streamWriter.Flush();
        Score teamScore5 = LogFile.GetTeamScore(this.ReconnectPhoenix, "[TEAM PHOENIX]");
        streamWriter.WriteLine("\t\t\t\t<TR class=\"phxhead\">");
        streamWriter.WriteLine("\t\t\t\t\t<TD nowrap>{0}</TD><TD>{1}</TD><TD>{2}</TD><TD>{3}</TD><TD>{4}</TD><TD>{5}</TD><TD>{6}</TD><TD>{7}</TD><TD>{8}</TD><TD>{9}</TD><TD>{10}</TD><TD>{11}</TD><TD>{12}</TD><TD>{13}</TD><TD>{14}</TD><TD>{15}</TD><TD>{16}</TD><TD>{17}</TD><TD>{18}</TD>", (object) teamScore5.Name, (object) teamScore5.Scored, (object) teamScore5.CTFPoints, (object) teamScore5.CTFOffense, (object) teamScore5.CTFDefense, (object) teamScore5.KillDeathRatio, (object) teamScore5.Kills, (object) teamScore5.KillsPerMinute, (object) teamScore5.Deaths, (object) teamScore5.DeathsPerMinute, (object) teamScore5.Suicides, (object) teamScore5.SuicidesPerMinute, (object) teamScore5.TimeAccelerator, (object) teamScore5.TimeBackPack, (object) teamScore5.TimePortableBattery, (object) teamScore5.TimeRegenerator, (object) teamScore5.TimeSpeed, (object) teamScore5.TimeConnected, (object) teamScore5.TimeReconnect);
        streamWriter.WriteLine("\t\t\t\t</TR>");
        foreach (Score score in this.ReconnectPhoenix)
        {
          streamWriter.WriteLine("\t\t\t\t<TR class=\"phx\">");
          streamWriter.WriteLine("\t\t\t\t\t<TD nowrap>{0}</TD><TD>{1}</TD><TD>{2}</TD><TD>{3}</TD><TD>{4}</TD><TD>{5}</TD><TD>{6}</TD><TD>{7}</TD><TD>{8}</TD><TD>{9}</TD><TD>{10}</TD><TD>{11}</TD><TD>{12}</TD><TD>{13}</TD><TD>{14}</TD><TD>{15}</TD><TD>{16}</TD><TD>{17}</TD><TD>{18}</TD>", (object) score.Name, (object) score.Scored, (object) score.CTFPoints, (object) score.CTFOffense, (object) score.CTFDefense, (object) score.KillDeathRatio, (object) score.Kills, (object) score.KillsPerMinute, (object) score.Deaths, (object) score.DeathsPerMinute, (object) score.Suicides, (object) score.SuicidesPerMinute, (object) score.TimeAccelerator, (object) score.TimeBackPack, (object) score.TimePortableBattery, (object) score.TimeRegenerator, (object) score.TimeSpeed, (object) score.TimeConnected, (object) score.TimeReconnect);
          streamWriter.WriteLine("\t\t\t\t</TR>");
        }
        streamWriter.Flush();
        Score teamScore6 = LogFile.GetTeamScore(this.ReconnectOther, "[TEAM OTHER]");
        streamWriter.WriteLine("\t\t\t\t<TR class=\"othhead\">");
        streamWriter.WriteLine("\t\t\t\t\t<TD nowrap>{0}</TD><TD>{1}</TD><TD>{2}</TD><TD>{3}</TD><TD>{4}</TD><TD>{5}</TD><TD>{6}</TD><TD>{7}</TD><TD>{8}</TD><TD>{9}</TD><TD>{10}</TD><TD>{11}</TD><TD>{12}</TD><TD>{13}</TD><TD>{14}</TD><TD>{15}</TD><TD>{16}</TD><TD>{17}</TD><TD>{18}</TD>", (object) teamScore6.Name, (object) teamScore6.Scored, (object) teamScore6.CTFPoints, (object) teamScore6.CTFOffense, (object) teamScore6.CTFDefense, (object) teamScore6.KillDeathRatio, (object) teamScore6.Kills, (object) teamScore6.KillsPerMinute, (object) teamScore6.Deaths, (object) teamScore6.DeathsPerMinute, (object) teamScore6.Suicides, (object) teamScore6.SuicidesPerMinute, (object) teamScore6.TimeAccelerator, (object) teamScore6.TimeBackPack, (object) teamScore6.TimePortableBattery, (object) teamScore6.TimeRegenerator, (object) teamScore6.TimeSpeed, (object) teamScore6.TimeConnected, (object) teamScore6.TimeReconnect);
        streamWriter.WriteLine("\t\t\t\t</TR>");
        foreach (Score score in this.ReconnectOther)
        {
          streamWriter.WriteLine("\t\t\t\t<TR class=\"oth\">");
          streamWriter.WriteLine("\t\t\t\t\t<TD nowrap>{0}</TD><TD>{1}</TD><TD>{2}</TD><TD>{3}</TD><TD>{4}</TD><TD>{5}</TD><TD>{6}</TD><TD>{7}</TD><TD>{8}</TD><TD>{9}</TD><TD>{10}</TD><TD>{11}</TD><TD>{12}</TD><TD>{13}</TD><TD>{14}</TD><TD>{15}</TD><TD>{16}</TD><TD>{17}</TD><TD>{18}</TD>", (object) score.Name, (object) score.Scored, (object) score.CTFPoints, (object) score.CTFOffense, (object) score.CTFDefense, (object) score.KillDeathRatio, (object) score.Kills, (object) score.KillsPerMinute, (object) score.Deaths, (object) score.DeathsPerMinute, (object) score.Suicides, (object) score.SuicidesPerMinute, (object) score.TimeAccelerator, (object) score.TimeBackPack, (object) score.TimePortableBattery, (object) score.TimeRegenerator, (object) score.TimeSpeed, (object) score.TimeConnected, (object) score.TimeReconnect);
          streamWriter.WriteLine("\t\t\t\t</TR>");
        }
        streamWriter.WriteLine("\t\t\t</TABLE>");
        streamWriter.WriteLine("\t\t</DIV>");
        streamWriter.Flush();
      }
      if ((options & HtmlOutput.FragList) == HtmlOutput.FragList)
      {
        streamWriter.WriteLine("\t\t<H2><A onclick=\"toggle_visibility('fraglist')\">Frag List</A></H2>");
        streamWriter.WriteLine("\t\t<DIV id=\"fraglist\">");
        streamWriter.WriteLine("\t\t\t<TABLE cellpadding=0 cellspacing=0 border=1>");
        streamWriter.WriteLine("\t\t\t\t<TR><TH>Time</TH><TH>Fragged</TH><TH>Weapon</TH></TR>");
        foreach (string str in this.FragListToHTML(Team.MI6))
          streamWriter.WriteLine(str);
        streamWriter.Flush();
        foreach (string str in this.FragListToHTML(Team.Phoenix))
          streamWriter.WriteLine(str);
        streamWriter.Flush();
        foreach (string str in this.FragListToHTML(Team.NotApplicable))
          streamWriter.WriteLine(str);
        streamWriter.WriteLine("\t\t\t</TABLE>");
        streamWriter.WriteLine("\t\t</DIV>");
        streamWriter.Flush();
      }
      if ((options & HtmlOutput.MostList) == HtmlOutput.MostList)
      {
        streamWriter.WriteLine("\t\t<H2><A onclick=\"toggle_visibility('mostlist')\">\"Most\" List</A></H2>");
        streamWriter.WriteLine("\t\t<DIV id=\"mostlist\">");
        streamWriter.WriteLine("\t\t\t<TABLE cellpadding=0 cellspacing=0 border=1>");
        streamWriter.WriteLine("\t\t\t\t<TR><TH>Type</TH><TH>Name</TH><TH>Score</TH></TR>");
        foreach (string str in this.ListsToHTML(this.MostOverall, Team.Overall))
          streamWriter.WriteLine(str);
        streamWriter.Flush();
        foreach (string str in this.ListsToHTML(this.MostMI6, Team.MI6))
          streamWriter.WriteLine(str);
        streamWriter.Flush();
        foreach (string str in this.ListsToHTML(this.MostPhoenix, Team.Phoenix))
          streamWriter.WriteLine(str);
        streamWriter.Flush();
        foreach (string str in this.ListsToHTML(this.MostOther, Team.NotApplicable))
          streamWriter.WriteLine(str);
        streamWriter.WriteLine("\t\t\t</TABLE>");
        streamWriter.WriteLine("\t\t</DIV>");
        streamWriter.Flush();
      }
      if ((options & HtmlOutput.LeastList) == HtmlOutput.LeastList)
      {
        streamWriter.WriteLine("\t\t<H2><A onclick=\"toggle_visibility('leastlist')\">\"Least\" List</A></H2>");
        streamWriter.WriteLine("\t\t<DIV id=\"leastlist\">");
        streamWriter.WriteLine("\t\t\t<TABLE cellpadding=0 cellspacing=0 border=1>");
        streamWriter.WriteLine("\t\t\t\t<TR><TH>Type</TH><TH>Name</TH><TH>Score</TH></TR>");
        foreach (string str in this.ListsToHTML(this.LeastOverall, Team.Overall))
          streamWriter.WriteLine(str);
        streamWriter.Flush();
        foreach (string str in this.ListsToHTML(this.LeastMI6, Team.MI6))
          streamWriter.WriteLine(str);
        streamWriter.Flush();
        foreach (string str in this.ListsToHTML(this.LeastPhoenix, Team.Phoenix))
          streamWriter.WriteLine(str);
        streamWriter.Flush();
        foreach (string str in this.ListsToHTML(this.LeastOther, Team.NotApplicable))
          streamWriter.WriteLine(str);
        streamWriter.WriteLine("\t\t\t</TABLE>");
        streamWriter.WriteLine("\t\t</DIV>");
        streamWriter.Flush();
      }
      if ((options & HtmlOutput.ChatHistory) == HtmlOutput.ChatHistory)
      {
        streamWriter.WriteLine("\t\t<H2><A onclick=\"toggle_visibility('chathistory')\">Chat History</A></H2>");
        streamWriter.WriteLine("\t\t<DIV id=\"chathistory\">");
        streamWriter.WriteLine("\t\t\t<TABLE cellpadding=0 cellspacing=0 border=1>");
        int num = 0;
        foreach (string str in this.ChatHistoryToHTML())
        {
          streamWriter.WriteLine("\t\t\t\t" + str);
          if (num % 25 == 0)
            streamWriter.Flush();
          ++num;
        }
        streamWriter.WriteLine("\t\t\t</TABLE>");
        streamWriter.WriteLine("\t\t</DIV>");
        streamWriter.Flush();
      }
      if ((options & HtmlOutput.ServerVars) == HtmlOutput.ServerVars)
      {
        streamWriter.WriteLine("\t\t<H2><A onclick=\"toggle_visibility('servervars')\">Server Variables</A></H2>");
        streamWriter.WriteLine("\t\t<DIV id=\"servervars\">");
        streamWriter.WriteLine("\t\t\t<TABLE cellpadding=0 cellspacing=0 border=1>");
        streamWriter.WriteLine("\t\t\t\t<TR><TH>Name</TH><TH>Value</TH></TR>");
        foreach (Variable variable in (Collection<Variable>) this._Variables)
        {
          VariableChanged[] variableChangeList = this.GetVariableChangeList(variable.Name);
          if (variableChangeList.Length != 0)
          {
            streamWriter.WriteLine("\t\t\t\t<TR><TD><A onclick=\"toggle_visibility_class('" + variable.Name + "')\"><B>" + variable.Name + "</B></A></TD><TH><B>" + variable.Value + "</B></TD></TR>");
            for (int index = 0; index < variableChangeList.Length; ++index)
            {
              VariableChanged variableChanged = variableChangeList[index];
              if (index == 0)
                streamWriter.WriteLine("\t\t\t\t<TR style=\"display: none;\" class=\"" + variable.Name + "\"><TD>" + this.DateTimeToTimeString(this._TimeOpened) + "</TD><TD><B>" + variableChanged.OldValue + "</B></TD></TR>");
              streamWriter.WriteLine("\t\t\t\t<TR style=\"display: none;\" class=\"" + variable.Name + "\"><TD>" + this.DateTimeToTimeString(variableChanged.Date) + "</TD><TD><B>" + variableChanged.NewValue + "</B></TD></TR>");
            }
          }
          else
            streamWriter.WriteLine("\t\t\t\t<TR><TD>" + variable.Name + "</TD><TD>" + variable.Value + "</TD></TR>");
        }
        streamWriter.WriteLine("\t\t\t</TABLE>");
        streamWriter.WriteLine("\t\t</DIV>");
        streamWriter.Flush();
      }
      if ((options & HtmlOutput.IPAddresses) == HtmlOutput.IPAddresses)
      {
        streamWriter.WriteLine("\t\t<H2><A onclick=\"toggle_visibility('ip')\">IP Addresses</A></H2>");
        streamWriter.WriteLine("\t\t<DIV id=\"ip\">");
        streamWriter.WriteLine("\t\t\t<TABLE cellpadding=0 cellspacing=0 border=1>");
        streamWriter.WriteLine("\t\t\t\t<TR><TH>Name</TH><TH>IP</TH></TR>");
        foreach (JbnLib.Log.Types.DateID.Connected connect in this.GetConnects())
          streamWriter.WriteLine("\t\t\t\t<TR><TD>" + this._Players[connect.ID].Name + "</TD><TD>" + this.TrimPort(connect.IP) + "</TD></TR>");
        streamWriter.WriteLine("\t\t\t</TABLE>");
        streamWriter.WriteLine("\t\t</DIV>");
        streamWriter.Flush();
      }
      streamWriter.WriteLine("\t</BODY>");
      streamWriter.WriteLine("</HTML>");
      streamWriter.Flush();
      streamWriter.Close();
      fileStream.Close();
    }

    private static string TeamToClassName(Team team)
    {
      if (team == Team.MI6)
        return "mi6";
      return team == Team.Phoenix ? "phx" : "oth";
    }

    private static string TeamToName(Team team)
    {
      switch (team)
      {
        case Team.MI6:
          return "[TEAM MI6]";
        case Team.Phoenix:
          return "[TEAM PHOENIX]";
        case Team.NotApplicable:
          return "[TEAM OTHER]";
        default:
          return "[OVERALL]";
      }
    }

    private string[] ListsToHTML(ScoreThreshold data, Team team) => new string[19]
    {
      "\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "head\"><TD nowrap colspan=3>" + LogFile.TeamToName(team) + "</TD></TR>",
      "\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "\"><TD nowrap>Scored</TD><TD>" + data.Scored.Name + "</TD><TD>" + data.Scored.Value + "</TD></TR>",
      "\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "\"><TD nowrap>Points</TD><TD>" + data.CTFPoints.Name + "</TD><TD>" + data.CTFPoints.Value + "</TD></TR>",
      "\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "\"><TD nowrap>Offense</TD><TD>" + data.CTFOffense.Name + "</TD><TD>" + data.CTFOffense.Value + "</TD></TR>",
      "\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "\"><TD nowrap>Defense</TD><TD>" + data.CTFDefense.Name + "</TD><TD>" + data.CTFDefense.Value + "</TD></TR>",
      "\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "\"><TD nowrap><SUP>Kill</SUP>/<SUB>Death</SUB></TD><TD>" + data.KillDeathRatio.Name + "</TD><TD>" + data.KillDeathRatio.Value + "</TD></TR>",
      "\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "\"><TD nowrap>Kills</TD><TD>" + data.Kills.Name + "</TD><TD>" + data.Kills.Value + "</TD></TR>",
      "\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "\"><TD nowrap><SUP>Kills</SUP>/<SUB>Minute</SUB></TD><TD>" + data.KillsPerMinute.Name + "</TD><TD>" + data.KillsPerMinute.Value + "</TD></TR>",
      "\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "\"><TD nowrap>Deaths</TD><TD>" + data.Deaths.Name + "</TD><TD>" + data.Deaths.Value + "</TD></TR>",
      "\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "\"><TD nowrap><SUP>Deaths</SUP>/<SUB>Minute</SUB></TD><TD>" + data.DeathsPerMinute.Name + "</TD><TD>" + data.DeathsPerMinute.Value + "</TD></TR>",
      "\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "\"><TD nowrap>Suicides</TD><TD>" + data.Suicides.Name + "</TD><TD>" + data.Suicides.Value + "</TD></TR>",
      "\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "\"><TD nowrap><SUP>Suicides</SUP>/<SUB>Minute</SUB></TD><TD>" + data.SuicidesPerMinute.Name + "</TD><TD>" + data.SuicidesPerMinute.Value + "</TD></TR>",
      "\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "\"><TD nowrap>Accelerator</TD><TD>" + data.TimeAccelerator.Name + "</TD><TD>" + data.TimeAccelerator.Value + "</TD></TR>",
      "\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "\"><TD nowrap>BackPack</TD><TD>" + data.TimeBackPack.Name + "</TD><TD>" + data.TimeBackPack.Value + "</TD></TR>",
      "\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "\"><TD nowrap>PortableBattery</TD><TD>" + data.TimePortableBattery.Name + "</TD><TD>" + data.TimePortableBattery.Value + "</TD></TR>",
      "\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "\"><TD nowrap>Regenerator</TD><TD>" + data.TimeRegenerator.Name + "</TD><TD>" + data.TimeRegenerator.Value + "</TD></TR>",
      "\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "\"><TD nowrap>Speed</TD><TD>" + data.TimeSpeed.Name + "</TD><TD>" + data.TimeSpeed.Value + "</TD></TR>",
      "\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "\"><TD nowrap>Connected</TD><TD>" + data.TimeConnected.Name + "</TD><TD>" + data.TimeConnected.Value + "</TD></TR>",
      "\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "\"><TD nowrap>Reconnecting</TD><TD>" + data.TimeReconnect.Name + "</TD><TD>" + data.TimeReconnect.Value + "</TD></TR>"
    };

    private string[] FragListToHTML(Team team)
    {
      BindingList<string> bindingList1 = new BindingList<string>();
      foreach (Player player in (CollectionBase) this._Players)
      {
        if (player.Team == team)
          bindingList1.Add(player.Name);
      }
      string[] array = new string[bindingList1.Count];
      for (int index = 0; index < bindingList1.Count; ++index)
        array[index] = bindingList1[index];
      Array.Sort<string>(array);
      BindingList<string> bindingList2 = new BindingList<string>();
      bindingList2.Add("\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "head\"><TD nowrap colspan=3>" + LogFile.TeamToName(team) + "</TD></TR>");
      foreach (string name in array)
      {
        Killed[] killList = this.GetKillList(this._Players.IndexOf(name));
        bindingList2.Add("\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "head\"><TD nowrap colspan=3>" + name + "</TD></TR>");
        for (int index = 0; index < killList.Length; ++index)
          bindingList2.Add("\t\t\t\t<TR class=\"" + LogFile.TeamToClassName(team) + "\"><TD nowrap>" + this.DateTimeToTimeString(killList[index].Date) + "</TD><TD>" + this._Players[killList[index].VictimID].Name + "</TD><TD>" + killList[index].Weapon + "</TD></TR>");
      }
      string[] html = new string[bindingList2.Count];
      for (int index = 0; index < html.Length; ++index)
        html[index] = bindingList2[index];
      return html;
    }

    private string[] ChatHistoryToHTML()
    {
      BindingList<string> bindingList = new BindingList<string>();
      foreach (JbnLib.Log.Types.Say.SayGeneral sayGeneral in (Collection<JbnLib.Log.Types.Say.SayGeneral>) this._SayGeneral)
        bindingList.Add("<TR><TD>[" + this.DateTimeToTimeString(sayGeneral.Date) + "]</TD><TD>" + this._Players[sayGeneral.ID].Name + ":</TD><TD>" + sayGeneral.Message + "</TD></TR>");
      foreach (JbnLib.Log.Types.Say.SayMI6 sayMi6 in (Collection<JbnLib.Log.Types.Say.SayMI6>) this._SayMI6)
        bindingList.Add("<TR><TD>[" + this.DateTimeToTimeString(sayMi6.Date) + "]</TD><TD>" + this.TeamToHTML(Team.MI6, this._Players[sayMi6.ID].Name) + ":</TD><TD>" + sayMi6.Message + "</TD></TR>");
      foreach (JbnLib.Log.Types.Say.SayPhoenix sayPhoenix in (Collection<JbnLib.Log.Types.Say.SayPhoenix>) this._SayPhoenix)
        bindingList.Add("<TR><TD>[" + this.DateTimeToTimeString(sayPhoenix.Date) + "]</TD><TD>" + this.TeamToHTML(Team.Phoenix, this._Players[sayPhoenix.ID].Name) + ":</TD><TD>" + sayPhoenix.Message + "</TD></TR>");
      foreach (JbnLib.Log.Types.Say.SayServer sayServer in (Collection<JbnLib.Log.Types.Say.SayServer>) this._SayServer)
        bindingList.Add("<TR><TD>[" + this.DateTimeToTimeString(sayServer.Date) + "]</TD><TD>Server:</TD><TD>" + sayServer.Message + "</TD></TR>");
      string[] array = new string[bindingList.Count];
      for (int index = 0; index < bindingList.Count; ++index)
        array[index] = bindingList[index];
      Array.Sort<string>(array);
      return array;
    }

    private string TeamToHTML(Team team, string name)
    {
      if (team == Team.MI6)
        return "<SPAN class=\"mi6\">" + name + "</SPAN>";
      return team == Team.Phoenix ? "<SPAN class=\"phx\">" + name + "</SPAN>" : "<SPAN class=\"oth\">" + name + "</SPAN>";
    }

    public string WinningTeamToHTML()
    {
      switch (this._WinningTeam)
      {
        case Team.MI6:
          return "<SPAN class=\"mi6\">MI6</SPAN>";
        case Team.Phoenix:
          return "<SPAN class=\"phx\">Phoenix</SPAN>";
        case Team.Draw:
          return "<SPAN class=\"oth\">Draw</SPAN>";
        default:
          return "<SPAN class=\"oth\">Not Set</SPAN>";
      }
    }

    private string DateTimeToTimeString(DateTime datetime) => Convert.ToString(datetime.Hour) + ":" + Convert.ToString(datetime.Minute).PadLeft(2, '0') + ":" + Convert.ToString(datetime.Second).PadLeft(2, '0');

    private string TrimPort(string ipport) => ipport.Split(':')[0];
  }
}
