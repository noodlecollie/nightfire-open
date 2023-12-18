// Decompiled with JetBrains decompiler
// Type: JbnLib.Network.GameSpy
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace JbnLib.Network
{
  public static class GameSpy
  {
    private static int p_ind;

    public static IPEndPoint[] GetMasterServerList(
      string gamename,
      string handoff,
      EncType type,
      string filter)
    {
      TcpClient tcpClient = new TcpClient("master.nightfirepc.com", 28900);
      StringBuilder stringBuilder = new StringBuilder();
      NetworkStream stream = tcpClient.GetStream();
      while (tcpClient.Available == 0)
        Thread.Sleep(10);
      while (tcpClient.Available > 0)
        stringBuilder.Append(Convert.ToChar(stream.ReadByte()));
      byte[] bytes1 = Encoding.ASCII.GetBytes(GameSpy.GetHandoff(handoff));
      byte[] bytes2 = Encoding.ASCII.GetBytes("\\basic\\gamename\\" + gamename + "\\enctype\\" + (object) (int) type + "\\location\\0\\where\\" + filter + "\\validate\\" + GameSpy.MakeValidate(Encoding.ASCII.GetBytes(GameSpy.GetSecureKey(stringBuilder.ToString().TrimEnd(new char[1]))), bytes1, type) + "\\final\\list\\\\gamename\\" + gamename + "\\final\\");
      stream.Write(bytes2, 0, bytes2.Length);
      List<byte> byteList = new List<byte>();
      while (tcpClient.Available == 0)
        Thread.Sleep(10);
      while (tcpClient.Available > 0)
        byteList.Add(Convert.ToByte(stream.ReadByte()));
      stream.Close();
      tcpClient.Close();
      return type != EncType.Advanced2 ? (IPEndPoint[]) null : GameSpy.ServerListToIPEndPoints(GameSpy.DecodeAdvanced2(byteList.ToArray(), bytes1));
    }

    private static string MakeValidate(byte[] securekey, byte[] handoff, EncType type)
    {
      byte[] numArray1 = new byte[256];
      int[] numArray2 = new int[4];
      for (short index = 0; index < (short) 256; ++index)
        numArray1[(int) index] = Convert.ToByte(index);
      for (short index = 0; index < (short) 256; ++index)
      {
        numArray2[0] = numArray2[0] + (int) numArray1[(int) index] + (int) handoff[(int) index % handoff.Length] & (int) byte.MaxValue;
        numArray2[2] = (int) numArray1[numArray2[0]];
        numArray1[numArray2[0]] = numArray1[(int) index];
        numArray1[(int) index] = Convert.ToByte(numArray2[2]);
      }
      numArray2[0] = 0;
      byte[] numArray3 = new byte[6];
      for (byte index = 0; (int) index < securekey.Length; ++index)
      {
        numArray3[(int) index] = securekey[(int) index];
        numArray2[0] = numArray2[0] + (int) numArray3[(int) index] + 1 & (int) byte.MaxValue;
        numArray2[2] = (int) numArray1[numArray2[0]];
        numArray2[1] = numArray2[1] + numArray2[2] & (int) byte.MaxValue;
        numArray2[3] = (int) numArray1[numArray2[1]];
        numArray1[numArray2[1]] = Convert.ToByte(numArray2[2]);
        numArray1[numArray2[0]] = Convert.ToByte(numArray2[3]);
        numArray3[(int) index] = Convert.ToByte((int) numArray3[(int) index] ^ (int) numArray1[numArray2[2] + numArray2[3] & (int) byte.MaxValue]);
      }
      if (type == EncType.Advanced2)
      {
        for (byte index = 0; (int) index < securekey.Length; ++index)
          numArray3[(int) index] ^= handoff[(int) index % handoff.Length];
      }
      int int32 = Convert.ToInt32(securekey.Length / 3);
      StringBuilder stringBuilder = new StringBuilder();
      byte index1 = 0;
      while (int32 >= 1)
      {
        --int32;
        numArray2[2] = (int) numArray3[(int) index1];
        numArray2[3] = (int) numArray3[(int) index1 + 1];
        stringBuilder.Append(GameSpy.AddChar(numArray2[2] >> 2));
        stringBuilder.Append(GameSpy.AddChar((numArray2[2] & 3) << 4 | numArray2[3] >> 4));
        numArray2[2] = (int) numArray3[(int) index1 + 2];
        stringBuilder.Append(GameSpy.AddChar((numArray2[3] & 15) << 2 | numArray2[2] >> 6));
        stringBuilder.Append(GameSpy.AddChar(numArray2[2] & 63));
        index1 = Convert.ToByte((int) index1 + 3);
      }
      return stringBuilder.ToString();
    }

    private static char AddChar(int value)
    {
      if (value < 26)
        return Convert.ToChar(value + 65);
      if (value < 52)
        return Convert.ToChar(value + 71);
      if (value < 62)
        return Convert.ToChar(value - 4);
      if (value == 62)
        return '+';
      return value == 63 ? '/' : Convert.ToChar(0);
    }

    private static string GetHandoff(string handoff)
    {
      if (handoff.Length == 6)
        return handoff;
      StringBuilder stringBuilder = new StringBuilder();
      for (int index = 2; index <= 13; index += 2)
        stringBuilder.Append(handoff[index]);
      return stringBuilder.ToString();
    }

    private static string GetSecureKey(string message) => message.Substring(message.Length - 6);

    private static uint SumOverflow(uint a, uint b) => Convert.ToUInt32((double) (a + b) % 4294967296.0);

    private static byte[] Shared1(uint[] tbuff, byte[] datap, int len)
    {
      GameSpy.p_ind = 309;
      int num = 309;
      int index1 = 0;
      int index2 = 309;
      int index3 = 4;
      byte[] numArray = new byte[4];
      for (; len > 0; --len)
      {
        if (index1 % 63 == 0)
        {
          GameSpy.p_ind = num;
          index2 = 309;
          index3 = 4;
          tbuff = GameSpy.Shared2(tbuff, 16);
        }
        if (index3 > 3)
        {
          numArray = BitConverter.GetBytes(tbuff[index2]);
          index3 = 0;
          ++index2;
        }
        datap[index1] ^= Convert.ToByte((int) numArray[index3] % 256);
        ++index1;
        ++GameSpy.p_ind;
        ++index3;
      }
      return datap;
    }

    private static uint[] Shared2(uint[] tbuff, int len)
    {
      int pInd = GameSpy.p_ind;
      uint num1 = tbuff[304];
      uint a1 = tbuff[305];
      uint a2 = tbuff[306];
      uint b1 = tbuff[307];
      int num2 = 0;
      for (int index = 0; index < len; ++index)
      {
        GameSpy.p_ind = Convert.ToInt32(num1 + 272U);
        while (b1 < 65536U)
        {
          uint num3 = GameSpy.SumOverflow(a1, b1);
          ++GameSpy.p_ind;
          uint b2 = GameSpy.SumOverflow(a2, num3);
          uint num4 = GameSpy.SumOverflow(num3, b2);
          tbuff[GameSpy.p_ind - 17] = num4;
          tbuff[GameSpy.p_ind - 1] = b2;
          uint num5 = b2 << 24 | b2 >> 8;
          tbuff[GameSpy.p_ind + 15] = b1;
          b1 <<= 1;
          ++num1;
          uint num6 = num4 ^ tbuff[(int) num4 & (int) byte.MaxValue];
          uint num7 = num5 ^ tbuff[(int) num5 & (int) byte.MaxValue];
          uint num8 = num7 << 24 | num7 >> 8;
          uint num9 = num6 >> 24 | num6 << 8;
          uint num10 = num9 ^ tbuff[(int) num9 & (int) byte.MaxValue];
          a2 = num8 ^ tbuff[(int) num8 & (int) byte.MaxValue];
          a1 = num10 >> 24 | num10 << 8;
        }
        uint num11 = a2 ^ a1;
        tbuff[pInd + index] = num11;
        --num1;
        uint num12 = tbuff[(int) num1 + 256];
        uint num13 = tbuff[(int) num1 + 272];
        uint num14 = ~num12;
        uint num15 = num14 << 24 | num14 >> 8;
        uint num16 = num15 ^ tbuff[(int) num15 & (int) byte.MaxValue];
        uint num17 = num13 ^ tbuff[(int) num13 & (int) byte.MaxValue];
        uint num18 = num16 << 24 | num16 >> 8;
        uint num19 = num17 >> 24 | num17 << 8;
        a1 = num18 ^ tbuff[(int) num18 & (int) byte.MaxValue];
        uint num20 = num19 ^ tbuff[(int) num19 & (int) byte.MaxValue];
        a2 = num20 >> 24 | num20 << 8;
        b1 = (uint) (((int) tbuff[(int) num1 + 288] << 1) + 1);
        ++num2;
      }
      tbuff[304] = num1;
      tbuff[305] = a1;
      tbuff[306] = a2;
      tbuff[307] = b1;
      return tbuff;
    }

    private static uint[] Shared3(uint[] data, int n1, int n2)
    {
      uint a1 = Convert.ToUInt32(n1);
      uint a2 = 0;
      uint b1 = 1;
      data[304] = 0U;
      for (int index = 32768; index != 0; index >>= 1)
      {
        uint num1 = GameSpy.SumOverflow(a1, b1);
        uint b2 = GameSpy.SumOverflow(a2, num1);
        uint num2 = GameSpy.SumOverflow(num1, b2);
        if ((n2 & index) != 0)
        {
          uint num3 = ~num2;
          b1 = (uint) (((int) b1 << 1) + 1);
          uint num4 = num3 << 24 | num3 >> 8;
          uint num5 = num4 ^ data[(int) num4 & (int) byte.MaxValue];
          uint num6 = b2 ^ data[(int) b2 & (int) byte.MaxValue];
          uint num7 = num5 << 24 | num5 >> 8;
          uint num8 = num6 >> 24 | num6 << 8;
          a1 = num7 ^ data[(int) num7 & (int) byte.MaxValue];
          uint num9 = num8 ^ data[(int) num8 & (int) byte.MaxValue];
          a2 = num9 >> 24 | num9 << 8;
        }
        else
        {
          data[(int) data[304] + 256] = num2;
          data[(int) data[304] + 272] = b2;
          data[(int) data[304] + 288] = b1;
          ++data[304];
          uint num10 = b2 << 24 | b2 >> 8;
          uint num11 = num2 ^ data[(int) num2 & (int) byte.MaxValue];
          uint num12 = num10 ^ data[(int) num10 & (int) byte.MaxValue];
          uint num13 = num12 << 24 | num12 >> 8;
          uint num14 = num11 >> 24 | num11 << 8;
          uint num15 = num14 ^ data[(int) num14 & (int) byte.MaxValue];
          a2 = num13 ^ data[(int) num13 & (int) byte.MaxValue];
          a1 = num15 >> 24 | num15 << 8;
          b1 <<= 1;
        }
      }
      data[305] = a1;
      data[306] = a2;
      data[307] = b1;
      data[308] = Convert.ToUInt32(n1);
      return data;
    }

    private static uint[] Shared4(byte[] data, uint[] dest)
    {
      byte[] numArray = new byte[data.Length - 1];
      for (int index = 0; index < data.Length - 1; ++index)
        numArray[index] = data[index + 1];
      int num1 = (int) data[0];
      for (short index = 0; index <= (short) byte.MaxValue; ++index)
        dest[(int) index] = 0U;
      for (byte index1 = 0; index1 <= (byte) 3; ++index1)
      {
        for (short index2 = 0; index2 <= (short) byte.MaxValue; ++index2)
          dest[(int) index2] = Convert.ToUInt32((long) (dest[(int) index2] << 8) + (long) index2 & (long) uint.MaxValue);
        byte index3 = index1;
        for (byte index4 = 0; index4 <= (byte) 1; ++index4)
        {
          for (short index5 = 0; index5 <= (short) byte.MaxValue; ++index5)
          {
            uint num2 = dest[(int) index5];
            index3 += Convert.ToByte((uint) ((int) num2 + (int) numArray[(int) index5 % num1] & (int) byte.MaxValue));
            dest[(int) index5] = dest[(int) index3];
            dest[(int) index3] = num2;
          }
        }
      }
      for (short index = 0; index <= (short) byte.MaxValue; ++index)
        dest[(int) index] ^= Convert.ToUInt32(index);
      dest = GameSpy.Shared3(dest, 0, 0);
      return dest;
    }

    private static string DecodeAdvanced2(byte[] data, byte[] handoff)
    {
      uint[] dest = new uint[326];
      for (int index = 256; index < dest.Length; ++index)
        dest[index] = 0U;
      data[0] ^= (byte) 236;
      for (int index = 0; index < handoff.Length; ++index)
        data[index + 1] ^= handoff[index];
      uint[] tbuff = GameSpy.Shared4(data, dest);
      byte[] datap = new byte[data.Length - 1 - (int) data[0]];
      for (int index = 0; index < data.Length - (int) data[0] - 1; ++index)
        datap[index] = data[(int) data[0] + index + 1];
      return datap.Length < 6 ? Encoding.ASCII.GetString(data) : Encoding.ASCII.GetString(GameSpy.Shared1(tbuff, datap, datap.Length));
    }

    private static IPEndPoint[] ServerListToIPEndPoints(string list)
    {
      string[] strArray1 = list.Split(new char[1]{ '\\' }, StringSplitOptions.RemoveEmptyEntries);
      List<IPEndPoint> ipEndPointList = new List<IPEndPoint>();
      for (int index = 1; index < strArray1.Length; index += 2)
      {
        string[] strArray2 = strArray1[index].Split(':');
        ipEndPointList.Add(new IPEndPoint(IPAddress.Parse(strArray2[0]), (int) Convert.ToUInt16(strArray2[1])));
      }
      return ipEndPointList.ToArray();
    }
  }
}
