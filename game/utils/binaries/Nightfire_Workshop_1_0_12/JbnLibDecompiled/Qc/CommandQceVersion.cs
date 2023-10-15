// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.CommandQceVersion
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Qc
{
  public class CommandQceVersion
  {
    public const string Command = "$qceversion";
    public QceGame Game;
    public int QcVersion;
    public int QceVersionMajor;
    public int QceVersionMinor;

    public CommandQceVersion(string data)
    {
      string[] strArray = data.Split('.');
      this.Game = (QceGame) Convert.ToInt32(strArray[0]);
      this.QcVersion = Convert.ToInt32(strArray[1]);
      this.QceVersionMajor = Convert.ToInt32(strArray[2]);
      this.QceVersionMinor = Convert.ToInt32(strArray[3]);
    }

    public CommandQceVersion(QceGame game, int version, int major, int minor)
    {
      this.Game = game;
      this.QcVersion = version;
      this.QceVersionMajor = major;
      this.QceVersionMinor = minor;
    }

    public new string ToString() => "$qceversion " + (object) (int) this.Game + "." + (object) this.QcVersion + "." + (object) this.QceVersionMajor + "." + (object) this.QceVersionMinor;
  }
}
