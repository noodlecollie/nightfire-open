// Decompiled with JetBrains decompiler
// Type: JbnLib.Network.IPPort
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using System;
using System.Net;

namespace JbnLib.Network
{
  public class IPPort
  {
    private string _IP;
    private int _Port;

    public string IP => this._IP;

    public int Port => this._Port;

    public IPPort(string ip, string port)
    {
      this._IP = !(ip == "localhost") ? ip : Dns.GetHostEntry(Dns.GetHostName()).AddressList.GetValue(0).ToString();
      this._Port = Convert.ToInt32(port);
    }

    public IPPort(string ipport)
    {
      string[] strArray = ipport.Trim().Split(':');
      if (strArray[0] == "localhost")
      {
        IPHostEntry hostEntry = Dns.GetHostEntry(Dns.GetHostName());
        strArray[0] = hostEntry.AddressList.GetValue(0).ToString();
      }
      if (strArray.Length == 2)
      {
        this._IP = strArray[0];
        this._Port = Convert.ToInt32(strArray[1]);
      }
      else
        Messages.ThrowException("Network.IPPort.New(string)", Messages.COLON_REQUIRED);
    }

    public new string ToString() => this._IP + ":" + (object) this._Port;
  }
}
