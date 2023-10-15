// Decompiled with JetBrains decompiler
// Type: JbnLib.Wrappers.BondArgs
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.ComponentModel;
using System.Drawing;
using System.Drawing.Design;
using System.Windows.Forms.Design;

namespace JbnLib.Wrappers
{
  public class BondArgs
  {
    private string _Connect = "";
    private BondArgs.ConsoleEnum _Console = BondArgs.ConsoleEnum.Default;
    private string _Map = "";
    private bool _Lan = true;
    private BondArgs.AAEnum _AA = BondArgs.AAEnum.Default;
    private string _BaseDir = "";
    private string _Comm = "";
    private bool _ConDebug;
    private ushort _ConHeight;
    private bool _ConTrace;
    private bool _Debug;
    private BondArgs.DepthEnum _Depth;
    private bool _Dev;
    private BondArgs.DisplayEnum _Display;
    private string _Game = "";
    private int _hChild = -1;
    private ushort _Heap = 128;
    private int _hFile = -1;
    private int _hParent = -1;
    private ushort _Hunk = 20;
    private bool _HwInfo;
    private string _IPName = "";
    private bool _Layout;
    private bool _Loopback;
    private byte _MaxPlayers;
    private Size _Resolution = Size.Empty;
    private bool _NetSleep;
    private bool _NetThread;
    private bool _NoCdAudio;
    private bool _NoForceMAccel;
    private bool _NoForceMParms;
    private bool _NoForceMSpd;
    private bool _NoIP;
    private bool _NoIPX;
    private bool _NoJoy;
    private bool _NoMouse;
    private bool _NoSound;
    private ushort _Port = 26015;
    private ushort _QueryPort = 6550;
    private bool _Reuse;
    private bool _Safe;
    private bool _StartTime;
    private bool _ToConsole;
    private BondArgs.VSyncEnum _VSync = BondArgs.VSyncEnum.Default;

    [Category("Bond")]
    [DefaultValue("")]
    [Description("The ip address or domain name to connect to.")]
    public string Connect
    {
      get => this._Connect;
      set => this._Connect = value;
    }

    [Category("Bond")]
    [DefaultValue(BondArgs.ConsoleEnum.Default)]
    [Description("Enables, disables, or makes no change to the in-game console.")]
    public BondArgs.ConsoleEnum Console
    {
      get => this._Console;
      set => this._Console = value;
    }

    [Category("Bond")]
    [DefaultValue("")]
    [Description("The name of the map to load without the file extension (.bsp).")]
    public string Map
    {
      get => this._Map;
      set => this._Map = value;
    }

    [Category("Bond")]
    [DefaultValue(true)]
    [Description("False means host on the Internet, true means host only on your Local Area Network (LAN).")]
    public bool Lan
    {
      get => this._Lan;
      set => this._Lan = value;
    }

    [Category("Bond")]
    [DefaultValue(BondArgs.AAEnum.Default)]
    [Description("Sets anti-aliasing or makes no changes at all.  Anti-aliasing smooths corners so they do not appear so pixelated.")]
    public BondArgs.AAEnum AA
    {
      get => this._AA;
      set => this._AA = value;
    }

    [Editor(typeof (FolderNameEditor), typeof (UITypeEditor))]
    [Category("Bond")]
    [DefaultValue("")]
    [Description("Alters the working directory for Nightfire.")]
    public string BaseDir
    {
      get => this._BaseDir;
      set => this._BaseDir = value;
    }

    [Category("Bond")]
    [DefaultValue("")]
    [Description("Really not sure what this is for...")]
    public string Comm
    {
      get => this._Comm;
      set => this._Comm = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("Starts the game with console debugging on meaning it will start writing to qconsole.log right away.")]
    public bool ConDebug
    {
      get => this._ConDebug;
      set => this._ConDebug = value;
    }

    [Category("Bond")]
    [DefaultValue(typeof (ushort), "0")]
    [Description("It is supposed to scale fonts for larger resolutions but does not appear to work.")]
    public ushort ConHeight
    {
      get => this._ConHeight;
      set => this._ConHeight = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("Really not sure what this is for...")]
    public bool ConTrace
    {
      get => this._ConTrace;
      set => this._ConTrace = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("Starts the game debug mode.")]
    public bool Debug
    {
      get => this._Debug;
      set => this._Debug = value;
    }

    [Category("Bond")]
    [DefaultValue(BondArgs.DepthEnum.Default)]
    [Description("Color depth in bits.")]
    public BondArgs.DepthEnum Depth
    {
      get => this._Depth;
      set => this._Depth = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("Enables developer messages ('developer 1') at start up.")]
    public bool Dev
    {
      get => this._Dev;
      set => this._Dev = value;
    }

    [Category("Bond")]
    [DefaultValue(BondArgs.DisplayEnum.Default)]
    [Description("Which mode to start the game in.  If you use Windowed, you should also set 'Resolution' below.")]
    public BondArgs.DisplayEnum Display
    {
      get => this._Display;
      set => this._Display = value;
    }

    [Category("Bond")]
    [DefaultValue("")]
    [Description("The game to load.  This is 'bond' by default.")]
    public string Game
    {
      get => this._Game;
      set => this._Game = value;
    }

    [Category("Bond")]
    [DefaultValue(typeof (int), "-1")]
    [Description("QHost child handle.")]
    public int hChild
    {
      get => this._hChild;
      set => this._hChild = value;
    }

    [Category("Bond")]
    [DefaultValue(typeof (ushort), "128")]
    [Description("This is the maximum amount of memory Nightfire can use in mebibytes (MiB).")]
    public ushort Heap
    {
      get => this._Heap;
      set => this._Heap = value;
    }

    [Category("Bond")]
    [DefaultValue(typeof (int), "-1")]
    [Description("QHost file handle.")]
    public int hFile
    {
      get => this._hFile;
      set => this._hFile = value;
    }

    [Category("Bond")]
    [DefaultValue(typeof (int), "-1")]
    [Description("QHost parent handle.")]
    public int hParent
    {
      get => this._hParent;
      set => this._hParent = value;
    }

    [Category("Bond")]
    [DefaultValue(typeof (ushort), "20")]
    [Description("This is the maximum amount of memory map and all its entities can use in mebibytes (MiB).")]
    public ushort Hunk
    {
      get => this._Hunk;
      set => this._Hunk = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("When the game is launched with this set to true, Nightfire dumps a 'hwinfo.txt' file to the same directory as 'Bond.exe' which contains a dump of Direct3D information.")]
    public bool HwInfo
    {
      get => this._HwInfo;
      set => this._HwInfo = value;
    }

    [Category("Bond")]
    [DefaultValue("")]
    [Description("The IP that should be used for multiplayer servers.  It is only necessary to use this if your computer has more than one IP address.")]
    public string IPName
    {
      get => this._IPName;
      set => this._IPName = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("Really not sure what this is for...")]
    public bool Layout
    {
      get => this._Layout;
      set => this._Layout = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("Really not sure what this is for...")]
    public bool Loopback
    {
      get => this._Loopback;
      set => this._Loopback = value;
    }

    [Category("Bond")]
    [DefaultValue(typeof (byte), "0")]
    [Description("Sets the maximum number of players allowed to join the server.  This value should be 2-32.")]
    public byte MaxPlayers
    {
      get => this._MaxPlayers;
      set => this._MaxPlayers = value;
    }

    [Category("Bond")]
    [DefaultValue(typeof (Size))]
    [Description("Sets the height and width of the display when the game is in windowed mode.  It can also override the fullscreen resolution.")]
    public Size Resolution
    {
      get => this._Resolution;
      set => this._Resolution = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("It makes the network 'sleep forever'.  It is really not known what that means specifically.")]
    public bool NetSleep
    {
      get => this._NetSleep;
      set => this._NetSleep = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("This supposedly forces networking activity to another thread.  This could improve performance on computers with more than one core but it may also create other issues.  Use with caution.")]
    public bool NetThread
    {
      get => this._NetThread;
      set => this._NetThread = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("Disables playing audio from the game CD.")]
    public bool NoCdAudio
    {
      get => this._NoCdAudio;
      set => this._NoCdAudio = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("Disables mouse acceleration.")]
    public bool NoForceMAccel
    {
      get => this._NoForceMAccel;
      set => this._NoForceMAccel = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("Disables mouse parameters.")]
    public bool NoForceMParms
    {
      get => this._NoForceMParms;
      set => this._NoForceMParms = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("Disables mouse speed.")]
    public bool NoForceMSpd
    {
      get => this._NoForceMSpd;
      set => this._NoForceMSpd = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("Disables TCP/IP network support.")]
    public bool NoIP
    {
      get => this._NoIP;
      set => this._NoIP = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("Disables IPX network support.")]
    public bool NoIPX
    {
      get => this._NoIPX;
      set => this._NoIPX = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("Disables joystick support.")]
    public bool NoJoy
    {
      get => this._NoJoy;
      set => this._NoJoy = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("Disables mouse support.")]
    public bool NoMouse
    {
      get => this._NoMouse;
      set => this._NoMouse = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("Disables audio support.")]
    public bool NoSound
    {
      get => this._NoSound;
      set => this._NoSound = value;
    }

    [Category("Bond")]
    [DefaultValue(typeof (ushort), "26015")]
    [Description("This is the port (UDP) players connect to when joining the server.")]
    public ushort Port
    {
      get => this._Port;
      set => this._Port = value;
    }

    [Category("Bond")]
    [DefaultValue(typeof (ushort), "6550")]
    [Description("This is the port (UDP) that is queried to get information like the name, number of players, and rules of the server.")]
    public ushort QueryPort
    {
      get => this._QueryPort;
      set => this._QueryPort = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("Really not sure what this is for...")]
    public bool Reuse
    {
      get => this._Reuse;
      set => this._Reuse = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("Starts the game in safe mode.")]
    public bool Safe
    {
      get => this._Safe;
      set => this._Safe = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("Really not sure what this is for...")]
    public bool StartTime
    {
      get => this._StartTime;
      set => this._StartTime = value;
    }

    [Category("Bond")]
    [DefaultValue(false)]
    [Description("Starts the game in full screen console mode.  Press escape to return to main menu.")]
    public bool ToConsole
    {
      get => this._ToConsole;
      set => this._ToConsole = value;
    }

    [Category("Bond")]
    [DefaultValue(BondArgs.VSyncEnum.Default)]
    [Description("Enables, disables, or makes no change to the vertical sync setting.  If it is enabled, it will not allow your framerate exceed your vertical refresh rate.  This prevents tearing.  If it is disabled, the game will run at the maximum framerate the hardware can handle.")]
    public BondArgs.VSyncEnum VSync
    {
      get => this._VSync;
      set => this._VSync = value;
    }

    public new string ToString()
    {
      string str = "";
      switch (this._AA)
      {
        case BondArgs.AAEnum.Off:
        case BondArgs.AAEnum.x2:
        case BondArgs.AAEnum.x4:
        case BondArgs.AAEnum.x8:
          str = str + "-aa " + (object) (sbyte) this._AA + " ";
          break;
      }
      if (this._BaseDir.Length > 0)
        str = str + "-basedir \"" + this._BaseDir + "\" ";
      if (this._Comm.Length > 0)
        str = str + "-comm \"" + this._Comm + "\" ";
      if (this._ConDebug)
        str += "-condebug ";
      if (this._ConHeight != (ushort) 0)
        str = str + "-conheight " + (object) this._ConHeight + " ";
      if (this._Connect.Length > 0)
        str = str + "+connect \"" + this._Connect + "\" ";
      switch (this._Console)
      {
        case BondArgs.ConsoleEnum.Disable:
        case BondArgs.ConsoleEnum.Enable:
          str = str + "+console " + (object) (sbyte) this._Console + " ";
          break;
      }
      if (this._ConTrace)
        str += "-contrace ";
      if (this._Debug)
        str += "-debug ";
      switch (this._Depth)
      {
        case BondArgs.DepthEnum.Bit16:
        case BondArgs.DepthEnum.Bit24:
          str = str + "-depth " + (object) (byte) this._Depth + " ";
          break;
      }
      if (this._Dev)
        str += "-dev ";
      switch (this._Display)
      {
        case BondArgs.DisplayEnum.Dedicated:
          str += "-dedicated ";
          break;
        case BondArgs.DisplayEnum.FullScreen:
          str += "-fullscreen ";
          break;
        case BondArgs.DisplayEnum.Windowed:
          str += "-windowed ";
          break;
      }
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
      if (this._HwInfo)
        str += "-hwinfo ";
      if (this._IPName.Length > 0)
        str = str + "-ipname \"" + this._IPName + "\" ";
      if (!this._Lan)
        str += "+sv_lan 0 ";
      if (this._Layout)
        str += "-layout ";
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
      if (this._NoForceMAccel)
        str += "-noforcemaccel ";
      if (this._NoForceMParms)
        str += "-noforcemparms ";
      if (this._NoForceMSpd)
        str += "-noforcemspd ";
      if (this._NoIP)
        str += "-noip ";
      if (this._NoIPX)
        str += "-noipx ";
      if (this._NoJoy)
        str += "-nojoy ";
      if (this._NoMouse)
        str += "-nomouse ";
      if (this._NoSound)
        str += "-nosound ";
      if (this._Port != (ushort) 26015)
        str = str + "-port " + (object) this._Port + " ";
      if (this._QueryPort != (ushort) 6550)
        str = str + "-port " + (object) this._QueryPort + " ";
      if (this._Resolution.Width > 0)
        str = str + "-width " + (object) this._Resolution.Width + " ";
      if (this._Resolution.Height > 0)
        str = str + "-height " + (object) this._Resolution.Height + " ";
      if (this._Reuse)
        str += "-reuse ";
      if (this._Safe)
        str += "-safe ";
      if (this._StartTime)
        str += "-starttime ";
      if (this._ToConsole)
        str += "-toconsole ";
      switch (this._VSync)
      {
        case BondArgs.VSyncEnum.Disable:
        case BondArgs.VSyncEnum.Enable:
          str = str + "-vsync " + (object) (sbyte) this._VSync + " ";
          break;
      }
      return str;
    }

    public enum ConsoleEnum : sbyte
    {
      Default = -1, // 0xFF
      Disable = 0,
      Enable = 1,
    }

    public enum AAEnum : sbyte
    {
      Default = -1, // 0xFF
      Off = 0,
      x2 = 2,
      x4 = 4,
      x8 = 8,
    }

    public enum DepthEnum : byte
    {
      Default = 0,
      Bit16 = 16, // 0x10
      Bit24 = 24, // 0x18
    }

    public enum DisplayEnum : byte
    {
      Default,
      Dedicated,
      FullScreen,
      Windowed,
    }

    public enum VSyncEnum : sbyte
    {
      Default = -1, // 0xFF
      Disable = 0,
      Enable = 1,
    }
  }
}
