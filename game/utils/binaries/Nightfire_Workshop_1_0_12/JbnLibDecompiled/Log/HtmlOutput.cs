// Decompiled with JetBrains decompiler
// Type: JbnLib.Log.HtmlOutput
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Log
{
  [Flags]
  public enum HtmlOutput
  {
    LogDetails = 0,
    OverallDetails = 1,
    ReconnectDetails = 2,
    FragList = ReconnectDetails | OverallDetails, // 0x00000003
    MostList = 4,
    LeastList = MostList | OverallDetails, // 0x00000005
    ChatHistory = MostList | ReconnectDetails, // 0x00000006
    ServerVars = ChatHistory | OverallDetails, // 0x00000007
    IPAddresses = 8,
    Protected = ServerVars, // 0x00000007
    All = Protected | IPAddresses, // 0x0000000F
  }
}
