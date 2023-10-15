// Decompiled with JetBrains decompiler
// Type: JbnLib.Log.Types.PlayerCollection
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;
using System.Collections;
using System.Text;

namespace JbnLib.Log.Types
{
  public class PlayerCollection : CollectionBase
  {
    public event PlayerCollectionAlteredHandler CollectionAltered;

    public Player this[int index]
    {
      get
      {
        PlayerCollectionAlteredHandler collectionAltered = this.CollectionAltered;
        if (collectionAltered != null)
          collectionAltered((object) this, new EventArgs());
        return (Player) this.List[index];
      }
      set
      {
        this.List[index] = (object) value;
        PlayerCollectionAlteredHandler collectionAltered = this.CollectionAltered;
        if (collectionAltered == null)
          return;
        collectionAltered((object) this, new EventArgs());
      }
    }

    public void Add(Player value)
    {
      this.List.Add((object) value);
      PlayerCollectionAlteredHandler collectionAltered = this.CollectionAltered;
      if (collectionAltered == null)
        return;
      collectionAltered((object) this, new EventArgs());
    }

    public void Insert(int index, Player value)
    {
      this.List.Insert(index, (object) value);
      PlayerCollectionAlteredHandler collectionAltered = this.CollectionAltered;
      if (collectionAltered == null)
        return;
      collectionAltered((object) this, new EventArgs());
    }

    public bool Contains(Player value) => this.List.Contains((object) value);

    public bool Contains(string name)
    {
      foreach (Player player in (CollectionBase) this)
      {
        if (name.CompareTo(player.Name) == 0)
          return true;
      }
      return false;
    }

    public int IndexOf(Player value) => this.List.IndexOf((object) value);

    public int IndexOf(string name)
    {
      int playerid = 0;
      long wonid = 0;
      string character = "";
      name = PlayerCollection.CleanUpName(name, ref playerid, ref wonid, ref character);
      int index1 = -1;
      if (name == "" && playerid != 0)
      {
        for (int index2 = 0; index2 < this.Count; ++index2)
        {
          if (playerid == this[index2].PlayerID)
            index1 = index2;
        }
      }
      for (int index3 = 0; index3 < this.Count; ++index3)
      {
        if (name.CompareTo(this[index3].Name) == 0)
          index1 = index3;
      }
      if (index1 == -1 && playerid != 0)
      {
        for (int index4 = 0; index4 < this.Count; ++index4)
        {
          if (playerid == this[index4].PlayerID)
            index1 = index4;
        }
      }
      if (index1 == -1)
      {
        index1 = this.Count;
        this.Insert(index1, new Player(name, playerid, wonid, Team.Unknown));
      }
      if (character.Length > 0)
        this[index1].Character = character;
      if (this[index1].PlayerID != playerid && playerid != 0)
        this[index1].PlayerID = playerid;
      return index1;
    }

    public int IndexOf(int playerid)
    {
      for (int index = 0; index < this.Count; ++index)
      {
        if (this[index].PlayerID == playerid)
          return index;
      }
      return -1;
    }

    public void CopyTo(Player[] array, int value) => this.List.CopyTo((Array) array, value);

    private static string CleanUpName(
      string name,
      ref int playerid,
      ref long wonid,
      ref string character)
    {
      string str1 = name;
      if (PlayerCollection.CountOccurances(name, (byte) 60) < 3)
        return str1;
      int num1 = str1.LastIndexOf('<');
      string str2 = str1.Substring(num1);
      string str3 = str1.Substring(0, num1);
      string s1 = str2.TrimStart('<').TrimEnd('>');
      bool flag1;
      if (!(s1 == "MI6") && !(s1 == "Phoenix"))
      {
        if (s1.Length != 0)
        {
          try
          {
            int.Parse(s1);
            flag1 = true;
            goto label_6;
          }
          catch
          {
            character = s1;
            flag1 = true;
            goto label_6;
          }
        }
      }
      flag1 = true;
label_6:
      int num2 = str3.LastIndexOf('<');
      string str4 = str3.Substring(num2);
      string str5 = str3.Substring(0, num2);
      string s2 = str4.TrimStart('<').TrimEnd('>');
      bool flag2;
      try
      {
        wonid = long.Parse(s2);
        flag2 = true;
      }
      catch
      {
        flag2 = false;
      }
      int num3 = str5.LastIndexOf('<');
      string str6 = str5.Substring(num3);
      string str7 = str5.Substring(0, num3);
      string s3 = str6.TrimStart('<').TrimEnd('>');
      bool flag3;
      try
      {
        playerid = int.Parse(s3);
        flag3 = true;
      }
      catch
      {
        flag3 = false;
      }
      return flag1 & flag3 & flag2 ? str7 : name;
    }

    private static int CountOccurances(string name, byte num)
    {
      byte[] bytes = Encoding.ASCII.GetBytes(name);
      int num1 = 0;
      for (int index = 0; index < bytes.Length; ++index)
      {
        if ((int) bytes[index] == (int) num)
          ++num1;
      }
      return num1;
    }

    public Team GetTeam(string name)
    {
      Team team = Team.Unknown;
      for (int index = 0; index < this.List.Count; ++index)
      {
        if (name.CompareTo(this[index].Name) == 0)
          team = this[index].Team;
      }
      return team;
    }

    public void UpdateTeam(string name, Team team)
    {
      for (int index = 0; index < this.Count; ++index)
      {
        if (name.CompareTo(this[index].Name) == 0)
          this[index].Team = team;
      }
    }
  }
}
