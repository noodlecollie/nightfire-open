// Decompiled with JetBrains decompiler
// Type: JbnLib.Log.Types.DateIDTeam.Killed
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Log.Types.DateIDTeam
{
  public class Killed : JbnLib.Log.Types.DateIDTeam.DateIDTeam
  {
    private int _VictimID;
    private string _Weapon;
    private bool _TeamKill;

    public int VictimID => this._VictimID;

    public string Weapon => this._Weapon;

    public bool TeamKill => this._TeamKill;

    public Killed(DateTime date, int id, Team team, int victimid, string weapon, bool teamkill)
    {
      this._Date = date;
      this._ID = id;
      this._Team = team;
      this._VictimID = victimid;
      this._Weapon = weapon;
      this._TeamKill = teamkill;
    }
  }
}
