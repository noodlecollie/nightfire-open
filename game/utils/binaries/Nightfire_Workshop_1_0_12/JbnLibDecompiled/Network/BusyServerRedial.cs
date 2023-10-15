// Decompiled with JetBrains decompiler
// Type: JbnLib.Network.BusyServerRedial
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;
using System.Diagnostics;
using System.Net;
using System.Threading;

namespace JbnLib.Network
{
  public class BusyServerRedial
  {
    private bool _Stop;
    private bool _IsStopped = true;
    private int _Delay = 1000;
    private Thread _Looper;
    private ProcessStartInfo _ProcessStartInfo;
    private ServerStatus _ServerStatus;
    private bool _IgnoreUnreachable;

    public bool IsStopped => this._IsStopped;

    public int Delay
    {
      get => this._Delay;
      set => this._Delay = value;
    }

    public BusyServerRedial(ProcessStartInfo psi, IPPort query, bool ignoreunreachable)
    {
      this._IgnoreUnreachable = ignoreunreachable;
      this._ProcessStartInfo = psi;
      this._ServerStatus = new ServerStatus();
      this._ServerStatus.EndPointQuery = StaticMethods.ToEndPoint(query);
      this._Looper = new Thread(new ThreadStart(this.Loop));
      this._Looper.Start();
    }

    public BusyServerRedial(ProcessStartInfo psi, string query, bool ignoreunreachable)
    {
      this._IgnoreUnreachable = ignoreunreachable;
      this._ProcessStartInfo = psi;
      this._ServerStatus = new ServerStatus();
      this._ServerStatus.EndPointQuery = StaticMethods.ToEndPoint(query);
      this._Looper = new Thread(new ThreadStart(this.Loop));
      this._Looper.Start();
    }

    public BusyServerRedial(
      ProcessStartInfo psi,
      string ip,
      ushort queryport,
      bool ignoreunreachable)
    {
      this._IgnoreUnreachable = ignoreunreachable;
      this._ProcessStartInfo = psi;
      this._ServerStatus = new ServerStatus();
      this._ServerStatus.EndPointQuery = StaticMethods.ToEndPoint(ip, queryport);
      this._Looper = new Thread(new ThreadStart(this.Loop));
      this._Looper.Start();
    }

    public BusyServerRedial(ProcessStartInfo psi, IPEndPoint query, bool ignoreunreachable)
    {
      this._IgnoreUnreachable = ignoreunreachable;
      this._ProcessStartInfo = psi;
      this._ServerStatus = new ServerStatus();
      this._ServerStatus.EndPointQuery = query;
      this._Looper = new Thread(new ThreadStart(this.Loop));
      this._Looper.Start();
    }

    private void Loop()
    {
      this._IsStopped = false;
      while (!this._Stop)
      {
        this._ServerStatus = StaticMethods.GetServerStatus(this._ServerStatus.EndPointQuery);
        if (this._ServerStatus.HostName.Length == 0 && !this._IgnoreUnreachable)
          this._Stop = true;
        if ((int) this._ServerStatus.NumPlayers < (int) this._ServerStatus.MaxPlayers && !this._Stop)
        {
          Process.Start(this._ProcessStartInfo);
          this._Stop = true;
        }
        if (!this._Stop)
          Thread.Sleep(this._Delay);
      }
      this._IsStopped = true;
    }

    public void Stop() => this._Stop = true;

    public void Abort() => this._Looper.Abort();
  }
}
