// Decompiled with JetBrains decompiler
// Type: JbnLib.Wrappers.BondDedArgs
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.ComponentModel;
using System.Drawing.Design;
using System.Windows.Forms.Design;

namespace JbnLib.Wrappers
{
  public class BondDedArgs
  {
    private string _Map = "";
    private bool _Lan = true;
    private string _BaseDir = "";
    private string _Comm = "";
    private bool _ConDebug;
    private ushort _ConHeight;
    private bool _ConTrace;
    private bool _Debug;
    private bool _Dev;
    private string _Game = "";
    private int _hChild = -1;
    private ushort _Heap = 128;
    private int _hFile = -1;
    private int _hParent = -1;
    private ushort _Hunk = 20;
    private string _IPName = "";
    private bool _Loopback;
    private byte _MaxPlayers;
    private bool _NetSleep;
    private bool _NetThread;
    private bool _NoCdAudio;
    private bool _NoIP;
    private bool _NoIPX;
    private ushort _Port = 26015;
    private ushort _QueryPort = 6550;
    private bool _Reuse;
    private bool _StartTime;

    [Category("BondDed")]
    [DefaultValue("")]
    [Description("The map to load.")]
    public string Map
    {
      get => this._Map;
      set => this._Map = value;
    }

    [Category("BondDed")]
    [DefaultValue(true)]
    [Description("False means host on the Internet, true means host only on your Local Area Network (LAN).")]
    public bool Lan
    {
      get => this._Lan;
      set => this._Lan = value;
    }

    [Editor(typeof (FolderNameEditor), typeof (UITypeEditor))]
    [Category("BondDed")]
    [DefaultValue("")]
    [Description("Alters the working directory for Nightfire.")]
    public string BaseDir
    {
      get => this._BaseDir;
      set => this._BaseDir = value;
    }

    [Category("BondDed")]
    [DefaultValue("")]
    [Description("Really not sure what this is for...")]
    public string Comm
    {
      get => this._Comm;
      set => this._Comm = value;
    }

    [Category("BondDed")]
    [DefaultValue(false)]
    [Description("Starts the game with console debugging on meaning it will start writing to qconsole.log right away.")]
    public bool ConDebug
    {
      get => this._ConDebug;
      set => this._ConDebug = value;
    }

    [Category("BondDed")]
    [DefaultValue(typeof (ushort), "0")]
    [Description("It is supposed to scale fonts for larger resolutions but does not appear to work.")]
    public ushort ConHeight
    {
      get => this._ConHeight;
      set => this._ConHeight = value;
    }

    [Category("BondDed")]
    [DefaultValue(false)]
    [Description("Really not sure what this is for...")]
    public bool ConTrace
    {
      get => this._ConTrace;
      set => this._ConTrace = value;
    }

    [Category("BondDed")]
    [DefaultValue(false)]
    [Description("Starts the game debug mode.")]
    public bool Debug
    {
      get => this._Debug;
      set => this._Debug = value;
    }

    [Category("BondDed")]
    [DefaultValue(false)]
    [Description("Enables developer messages ('developer 1') at start up.")]
    public bool Dev
    {
      get => this._Dev;
      set => this._Dev = value;
    }

    [Category("BondDed")]
    [DefaultValue("")]
    [Description("The game to load.  This is 'bond' by default.")]
    public string Game
    {
      get => this._Game;
      set => this._Game = value;
    }

    [Category("BondDed")]
    [DefaultValue(typeof (int), "-1")]
    [Description("QHost child handle.")]
    public int hChild
    {
      get => this._hChild;
      set => this._hChild = value;
    }

    [Category("BondDed")]
    [DefaultValue(typeof (ushort), "128")]
    [Description("This is the maximum amount of memory Nightfire can use in mebibytes (MiB).")]
    public ushort Heap
    {
      get => this._Heap;
      set => this._Heap = value;
    }

    [Category("BondDed")]
    [DefaultValue(typeof (int), "-1")]
    [Description("QHost file handle.")]
    public int hFile
    {
      get => this._hFile;
      set => this._hFile = value;
    }

    [Category("BondDed")]
    [DefaultValue(typeof (int), "-1")]
    [Description("QHost parent handle.")]
    public int hParent
    {
      get => this._hParent;
      set => this._hParent = value;
    }

    [Category("BondDed")]
    [DefaultValue(typeof (ushort), "20")]
    [Description("This is the maximum amount of memory map and all its entities can use in mebibytes (MiB).")]
    public ushort Hunk
    {
      get => this._Hunk;
      set => this._Hunk = value;
    }

    [Category("BondDed")]
    [DefaultValue("")]
    [Description("The IP that should be used for multiplayer servers.  It is only necessary to use this if your computer has more than one IP address.")]
    public string IPName
    {
      get => this._IPName;
      set => this._IPName = value;
    }

    [Category("BondDed")]
    [DefaultValue(false)]
    [Description("Really not sure what this is for...")]
    public bool Loopback
    {
      get => this._Loopback;
      set => this._Loopback = value;
    }

    [Category("BondDed")]
    [DefaultValue(typeof (byte), "0")]
    [Description("Sets the maximum number of players allowed to join the server.  This value should be 2-32.")]
    public byte MaxPlayers
    {
      get => this._MaxPlayers;
      set => this._MaxPlayers = value;
    }

    [Category("BondDed")]
    [DefaultValue(false)]
    [Description("It makes the network 'sleep forever'.  It is really not known what that means specifically.")]
    public bool NetSleep
    {
      get => this._NetSleep;
      set => this._NetSleep = value;
    }

    [Category("BondDed")]
    [DefaultValue(false)]
    [Description("This supposedly forces networking activity to another thread.  This could improve performance on computers with more than one core but it may also create other issues.  Use with caution.")]
    public bool NetThread
    {
      get => this._NetThread;
      set => this._NetThread = value;
    }

    [Category("BondDed")]
    [DefaultValue(false)]
    [Description("Disables playing audio from the game CD.")]
    public bool NoCdAudio
    {
      get => this._NoCdAudio;
      set => this._NoCdAudio = value;
    }

    [Category("BondDed")]
    [DefaultValue(false)]
    [Description("Disables TCP/IP network support.")]
    public bool NoIP
    {
      get => this._NoIP;
      set => this._NoIP = value;
    }

    [Category("BondDed")]
    [DefaultValue(false)]
    [Description("Disables IPX network support.")]
    public bool NoIPX
    {
      get => this._NoIPX;
      set => this._NoIPX = value;
    }

    [Category("BondDed")]
    [DefaultValue(typeof (ushort), "26015")]
    [Description("This is the port (UDP) players connect to when joining the server.")]
    public ushort Port
    {
      get => this._Port;
      set => this._Port = value;
    }

    [Category("BondDed")]
    [DefaultValue(typeof (ushort), "6550")]
    [Description("This is the port (UDP) that is queried to get information like the name, number of players, and rules of the server.")]
    public ushort QueryPort
    {
      get => this._QueryPort;
      set => this._QueryPort = value;
    }

    [Category("BondDed")]
    [DefaultValue(false)]
    [Description("Really not sure what this is for...")]
    public bool Reuse
    {
      get => this._Reuse;
      set => this._Reuse = value;
    }

    [Category("BondDed")]
    [DefaultValue(false)]
    [Description("Really not sure what this is for...")]
    public bool StartTime
    {
      get => this._StartTime;
      set => this._StartTime = value;
    }

    public new string ToString()
    {
      string str = "";
      if (this._BaseDir.Length > 0)
        str = str + "-basedir \"" + this._BaseDir + "\" ";
      if (this._Comm.Length > 0)
        str = str + "-comm \"" + this._Comm + "\" ";
      if (this._ConDebug)
        str += "-condebug ";
      if (this._ConHeight != (ushort) 0)
        str = str + "-conheight " + (object) this._ConHeight + " ";
      if (this._ConTrace)
        str += "-contrace ";
      if (this._Debug)
        str += "-debug ";
      if (this._Dev)
        str += "-dev ";
      if (this._Game.Length > 0)
        str = str + "-game \"" + this._Game + "\" ";
      if (this._hChild != -1)
        str = str + "-HCHILD " + (object) this._hChild + " ";
      if (this._Heap != (ushort) 128)
        str = str + "-heap " + (object) this._Heap + " ";
      if (this._hFile != -1)
        str = str + "-HFILE " + (object) this._hFile + " ";
      if (this._hParent != -1)
        str = str + "-HPARENT " + (object) this._hParent + " ";
      if (this._Hunk != (ushort) 20)
        str = str + "-heap " + (object) this._Hunk + " ";
      if (this._IPName.Length > 0)
        str = str + "-ipname \"" + this._IPName + "\" ";
      if (!this._Lan)
        str += "+sv_lan 0 ";
      if (this._Loopback)
        str += "-loopback ";
      if (this._Map.Length > 0)
        str = str + "+map \"" + this._Map + "\" ";
      if (this._MaxPlayers > (byte) 0)
        str = str + "-maxplayers " + (object) this._MaxPlayers + " ";
      if (this._NetSleep)
        str += "-netsleep ";
      if (this._NetThread)
        str += "-netthread ";
      if (this._NoCdAudio)
        str += "-nocdaudio ";
      if (this._NoIP)
        str += "-noip ";
      if (this._NoIPX)
        str += "-noipx ";
      if (this._Port != (ushort) 26015)
        str = str + "-port " + (object) this._Port + " ";
      if (this._QueryPort != (ushort) 6550)
        str = str + "-port " + (object) this._QueryPort + " ";
      if (this._Reuse)
        str += "-reuse ";
      if (this._StartTime)
        str += "-starttime ";
      return str;
    }
  }
}
