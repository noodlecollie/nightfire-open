// Decompiled with JetBrains decompiler
// Type: JbnLib.Network.RemoteConsole
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace JbnLib.Network
{
  public class RemoteConsole
  {
    private UdpClient _UDP;
    private byte[] _ChallengeStart = new byte[9]
    {
      byte.MaxValue,
      byte.MaxValue,
      byte.MaxValue,
      byte.MaxValue,
      (byte) 114,
      (byte) 99,
      (byte) 111,
      (byte) 110,
      (byte) 32
    };
    private byte[] _Password;
    private IPEndPoint _IPEndPoint;
    private byte[] _ID;

    public string Password
    {
      get => Encoding.ASCII.GetString(this._Password);
      set => this._Password = Encoding.ASCII.GetBytes(value);
    }

    public IPEndPoint IPEndPoint => this._IPEndPoint;

    public string ID => Encoding.ASCII.GetString(this._ID);

    public RemoteConsole()
    {
    }

    public RemoteConsole(IPEndPoint ep, string password)
    {
      try
      {
        this.Connect(ep, password, (byte) 1);
      }
      catch
      {
        Messages.ThrowException("Network.RemoteConsole.New(IPEndPoint, string)", Messages.CANNOT_CONNECT);
      }
    }

    public RemoteConsole(string ipport, string password)
    {
      string[] strArray = ipport.Split(Convert.ToChar(":"));
      if (strArray[0] == "localhost")
      {
        IPHostEntry hostEntry = Dns.GetHostEntry(Dns.GetHostName());
        strArray[0] = hostEntry.AddressList.GetValue(0).ToString();
      }
      try
      {
        this.Connect(new IPEndPoint(IPAddress.Parse(strArray[0]), Convert.ToInt32(strArray[1])), password, (byte) 1);
      }
      catch
      {
        Messages.ThrowException("Network.RemoteConsole.New(string, string)", Messages.CANNOT_CONNECT);
      }
    }

    public RemoteConsole(string ip, int port, string password)
    {
      if (ip == "localhost")
        ip = Dns.GetHostEntry(Dns.GetHostName()).AddressList.GetValue(0).ToString();
      try
      {
        this.Connect(new IPEndPoint(IPAddress.Parse(ip), Convert.ToInt32(port)), password, (byte) 1);
      }
      catch
      {
        Messages.ThrowException("Network.RemoteConsole.New(string, int, string)", Messages.CANNOT_CONNECT);
      }
    }

    public bool Connected() => this._UDP != null && this._ID != null && this._ID.Length != 0;

    public bool Connect(IPEndPoint ep, string password, byte timeout)
    {
      try
      {
        this._IPEndPoint = ep;
        this._Password = Encoding.ASCII.GetBytes(password);
        this._UDP = new UdpClient();
        this._UDP.Connect(this._IPEndPoint);
        byte[] dgram = new byte[20]
        {
          byte.MaxValue,
          byte.MaxValue,
          byte.MaxValue,
          byte.MaxValue,
          (byte) 99,
          (byte) 104,
          (byte) 97,
          (byte) 108,
          (byte) 108,
          (byte) 101,
          (byte) 110,
          (byte) 103,
          (byte) 101,
          (byte) 32,
          (byte) 114,
          (byte) 99,
          (byte) 111,
          (byte) 110,
          (byte) 10,
          (byte) 0
        };
        this._UDP.Send(dgram, dgram.Length);
        for (byte index = 0; (int) index <= (int) timeout; ++index)
        {
          if (this._UDP.Available > 0)
          {
            byte[] sourceArray = this._UDP.Receive(ref this._IPEndPoint);
            this._ID = new byte[sourceArray.Length - 21];
            Array.Copy((Array) sourceArray, 19, (Array) this._ID, 0, sourceArray.Length - 21);
            return true;
          }
          Thread.Sleep(1000);
        }
        return false;
      }
      catch
      {
        return false;
      }
    }

    public string SendCommand(string command)
    {
      byte[] challengeByteArray = this.CreateChallengeByteArray(Encoding.ASCII.GetBytes(command));
      try
      {
        this._UDP.Send(challengeByteArray, challengeByteArray.Length);
        string str = "";
        do
        {
          str += Encoding.ASCII.GetString(this._UDP.Receive(ref this._IPEndPoint)).Substring(4);
          Thread.Sleep(100);
        }
        while (this._UDP.Available > 0);
        return str.Replace("\0l", "").Replace("\0", "").Trim('\n');
      }
      catch
      {
        Messages.ThrowException("Network.RemoteConsole.SendCommand(string)", Messages.CONNECTION_LOST);
      }
      return "";
    }

    private byte[] CreateChallengeByteArray(byte[] command)
    {
      byte[] challengeByteArray = new byte[this._ChallengeStart.Length + this._ID.Length + this._Password.Length + command.Length + 6];
      this._ChallengeStart.CopyTo((Array) challengeByteArray, 0);
      int length = this._ChallengeStart.Length;
      this._ID.CopyTo((Array) challengeByteArray, length);
      int num1 = length + this._ID.Length;
      byte[] numArray1 = challengeByteArray;
      int index1 = num1;
      int num2 = index1 + 1;
      numArray1[index1] = (byte) 32;
      byte[] numArray2 = challengeByteArray;
      int index2 = num2;
      int index3 = index2 + 1;
      numArray2[index2] = (byte) 34;
      this._Password.CopyTo((Array) challengeByteArray, index3);
      int num3 = index3 + this._Password.Length;
      byte[] numArray3 = challengeByteArray;
      int index4 = num3;
      int num4 = index4 + 1;
      numArray3[index4] = (byte) 34;
      byte[] numArray4 = challengeByteArray;
      int index5 = num4;
      int index6 = index5 + 1;
      numArray4[index5] = (byte) 32;
      command.CopyTo((Array) challengeByteArray, index6);
      int num5 = index6 + command.Length;
      byte[] numArray5 = challengeByteArray;
      int index7 = num5;
      int num6 = index7 + 1;
      numArray5[index7] = (byte) 10;
      byte[] numArray6 = challengeByteArray;
      int index8 = num6;
      int num7 = index8 + 1;
      numArray6[index8] = (byte) 0;
      return challengeByteArray;
    }

    public void Close() => this._UDP.Close();

    public IPEndPoint ToIPEndPoint(string ipport)
    {
      string[] strArray = ipport.Split(Convert.ToChar(58));
      return new IPEndPoint(IPAddress.Parse(strArray[0]), Convert.ToInt32(strArray[1]));
    }

    public IPEndPoint ToIPEndPoint(string ip, int port) => new IPEndPoint(IPAddress.Parse(ip), Convert.ToInt32(port));

    public new string ToString()
    {
      byte[] numArray = new byte[6];
      this._IPEndPoint.Address.GetAddressBytes().CopyTo((Array) numArray, 0);
      return numArray[0].ToString("X") + numArray[1].ToString("X") + numArray[2].ToString("X") + numArray[3].ToString("X") + this._IPEndPoint.Port.ToString("X4");
    }
  }
}
