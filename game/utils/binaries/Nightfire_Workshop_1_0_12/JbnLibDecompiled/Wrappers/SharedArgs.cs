// Decompiled with JetBrains decompiler
// Type: JbnLib.Wrappers.SharedArgs
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System.ComponentModel;

namespace JbnLib.Wrappers
{
  public class SharedArgs
  {
    private bool _Chart;
    private bool _NoLog;
    private bool _NoInfo;
    private bool _Estimate;
    private bool _Verbose;
    private uint _Threads;
    private SharedArgs.PriorityEnum _Priority = SharedArgs.PriorityEnum.Normal;
    private SharedArgs.DeveloperEnum _Dev;

    [Category("BCommon")]
    [DefaultValue(false)]
    [Description("Display bsp statitics.")]
    public bool Chart
    {
      get => this._Chart;
      set => this._Chart = value;
    }

    [Category("BCommon")]
    [DefaultValue(false)]
    [Description("Do not generate the compile logfiles.")]
    public bool NoLog
    {
      get => this._NoLog;
      set => this._NoLog = value;
    }

    [Category("BCommon")]
    [DefaultValue(false)]
    [Description("Do not show tool configuration information.")]
    public bool NoInfo
    {
      get => this._NoInfo;
      set => this._NoInfo = value;
    }

    [Category("BCommon")]
    [DefaultValue(false)]
    [Description("Display estimated time during compile.")]
    public bool Estimate
    {
      get => this._Estimate;
      set => this._Estimate = value;
    }

    [Category("BCommon")]
    [DefaultValue(false)]
    [Description("Compile with verbose messages.")]
    public bool Verbose
    {
      get => this._Verbose;
      set => this._Verbose = value;
    }

    [Category("BCommon")]
    [DefaultValue(typeof (uint), "0")]
    [Description("Manually specify the number of threads to run.")]
    public uint Threads
    {
      get => this._Threads;
      set => this._Threads = value;
    }

    [Category("BCommon")]
    [DefaultValue(SharedArgs.PriorityEnum.Normal)]
    [Description("What priority the process should run at.")]
    public SharedArgs.PriorityEnum Priority
    {
      get => this._Priority;
      set => this._Priority = value;
    }

    [Category("BCommon")]
    [DefaultValue(SharedArgs.DeveloperEnum.Off)]
    [Description("The level of developer messages should be displayed.  MegaSpam will display all available messages while off will only show the important stuff.")]
    public SharedArgs.DeveloperEnum Dev
    {
      get => this._Dev;
      set => this._Dev = value;
    }

    public new string ToString()
    {
      string str = "";
      if (this._Chart)
        str += "-chart ";
      if (this._NoLog)
        str += "-nolog ";
      if (this._NoInfo)
        str += "-noinfo ";
      if (this._Estimate)
        str += "-estimate ";
      if (this._Verbose)
        str += "-verbose ";
      if (this._Threads != 0U)
        str = str + "-threads " + (object) this._Threads + " ";
      switch (this._Priority)
      {
        case SharedArgs.PriorityEnum.Low:
          str += "-low ";
          break;
        case SharedArgs.PriorityEnum.High:
          str += "-high ";
          break;
      }
      if (this._Dev != SharedArgs.DeveloperEnum.Off)
        str = str + "-dev " + (object) (byte) this._Dev + " ";
      return str;
    }

    public enum PriorityEnum : byte
    {
      Low,
      Normal,
      High,
    }

    public enum DeveloperEnum : byte
    {
      Off,
      Error,
      Warning,
      Message,
      Fluff,
      Spam,
      MegaSpam,
    }
  }
}
