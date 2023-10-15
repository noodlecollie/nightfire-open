// Decompiled with JetBrains decompiler
// Type: JbnLib.Network.ServerQuery
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;
using System;
using System.Collections.Generic;
using System.Net;
using System.Threading;

namespace JbnLib.Network
{
  public class ServerQuery : Dictionary<IPEndPoint, ServerStatus>
  {
    private const uint MIN_REFRESH = 10;
    private DateTime _LastRefresh;
    private uint _AutoRefresh;
    private string _GameName = "jbnightfire";
    private string _HandOff = "JcSM9ujj3GLl2by";
    private EncType _EncType = EncType.Advanced2;
    private string _Filter = "";
    private bool _ClearUnresponsive = true;
    private Thread _Thread;
    private bool _Stop;
    private bool _IsStopped = true;

    public event ServerQueryRefreshedHandler ServerQueryRefreshed;

    public DateTime LastRefresh => this._LastRefresh;

    public uint AutoRefresh
    {
      get => this._AutoRefresh;
      set
      {
        if (value == 0U || value >= 10U)
          this._AutoRefresh = value;
        else
          this._AutoRefresh = 10U;
      }
    }

    public string GameName
    {
      get => this._GameName;
      set => this._GameName = value;
    }

    public string HandOff
    {
      get => this._HandOff;
      set => this._HandOff = value;
    }

    public EncType EncType
    {
      get => this._EncType;
      set => this._EncType = value;
    }

    public string Filter
    {
      get => this._Filter;
      set => this._Filter = value;
    }

    public bool ClearUnresponsive
    {
      get => this._ClearUnresponsive;
      set => this._ClearUnresponsive = value;
    }

    public ServerQuery()
    {
    }

    public ServerQuery(
      string gamename,
      string handoff,
      EncType enctype,
      string filter,
      bool clearunresponsive,
      uint autorefresh)
    {
      this._GameName = gamename;
      this._HandOff = handoff;
      this._EncType = enctype;
      this._Filter = filter;
      this._ClearUnresponsive = clearunresponsive;
      this.AutoRefresh = autorefresh;
    }

    public bool Start()
    {
      if (this._AutoRefresh == 0U)
        return false;
      this._Thread = new Thread(new ThreadStart(this.Looper));
      this._Thread.Start();
      return true;
    }

    public void Stop() => this._Stop = true;

    public void Abort() => this._Thread.Abort();

    public bool IsStopped => this._IsStopped;

    private void Looper()
    {
      this._IsStopped = false;
      while (!this._Stop)
      {
        if ((long) (DateTime.Now - this._LastRefresh).Seconds >= (long) this._AutoRefresh)
          this.ForceRefresh();
        if (!this._Stop)
          Thread.Sleep(750);
      }
      this._IsStopped = true;
    }

    private void RefreshServer(object param)
    {
      IPEndPoint ipEndPoint = (IPEndPoint) param;
      ServerStatus serverStatus = StaticMethods.GetServerStatus(ipEndPoint);
      if (serverStatus == null)
        this[ipEndPoint].Ping = -1;
      else
        this[ipEndPoint] = serverStatus;
    }

    public void ForceRefresh()
    {
      if ((DateTime.Now - this._LastRefresh).Seconds < 2)
        return;
      List<IPEndPoint> ipEndPointList = new List<IPEndPoint>();
      List<Thread> threadList = new List<Thread>();
      IPEndPoint[] masterServerList = GameSpy.GetMasterServerList(this._GameName, this._HandOff, this._EncType, this._Filter);
      if (masterServerList.Length != 0)
      {
        foreach (IPEndPoint ipEndPoint in masterServerList)
        {
          ServerStatus serverStatus = new ServerStatus();
          if (!this.ContainsKey(ipEndPoint))
            this.Add(ipEndPoint, serverStatus);
          ipEndPointList.Add(ipEndPoint);
          threadList.Add(new Thread(new ParameterizedThreadStart(this.RefreshServer)));
          threadList[threadList.Count - 1].Start((object) ipEndPoint);
        }
        while (threadList.Count > 0)
        {
          for (int index = threadList.Count - 1; index >= 0; --index)
          {
            if (!threadList[index].IsAlive)
              threadList.RemoveAt(index);
          }
          Thread.Sleep(100);
        }
        if (this._ClearUnresponsive)
        {
          IPEndPoint[] array = new IPEndPoint[this.Count];
          this.Keys.CopyTo(array, 0);
          for (int index = this.Keys.Count - 1; index >= 0; --index)
          {
            if (!ipEndPointList.Contains(array[index]) || this[array[index]].Ping == -1)
              this.Remove(array[index]);
          }
        }
        ServerQueryRefreshedHandler serverQueryRefreshed = this.ServerQueryRefreshed;
        if (serverQueryRefreshed != null)
          serverQueryRefreshed((object) this, new EventArgs());
      }
      this._LastRefresh = DateTime.Now;
    }
  }
}
