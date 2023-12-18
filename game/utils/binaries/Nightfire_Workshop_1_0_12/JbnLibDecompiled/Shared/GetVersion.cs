// Decompiled with JetBrains decompiler
// Type: JbnLib.Shared.GetVersion
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using System;
using System.IO;

namespace JbnLib.Shared
{
  public static class GetVersion
  {
    public static int BspFile(string path) => GetVersion.BspFile(path, 0);

    public static int BspFile(string path, int offset)
    {
      if (!File.Exists(path))
        Messages.ThrowException("Shared.GetVersion.BspFile(string, int)", Messages.FILE_NOT_FOUND + path);
      FileStream input = new FileStream(path, FileMode.Open, FileAccess.Read);
      BinaryReader binaryReader = new BinaryReader((Stream) input);
      int num = binaryReader.ReadInt32();
      binaryReader.Close();
      input.Close();
      return num;
    }

    public static int MapFile(string path)
    {
      int num = -1;
      if (!File.Exists(path))
        Messages.ThrowException("Shared.GetVersion.MapFile(string)", Messages.FILE_NOT_FOUND + path);
      StreamReader streamReader = new StreamReader(path);
      while (streamReader.Peek() != -1)
      {
        string str1 = streamReader.ReadLine().ToLower().TrimStart(char.MinValue, ' ', '"');
        if (str1.StartsWith("mapversion"))
        {
          string str2 = str1.Substring(10).TrimStart(char.MinValue, ' ', '"').TrimEnd('"', '\n', '\r', ' ');
          try
          {
            num = Convert.ToInt32(str2);
            break;
          }
          catch
          {
          }
        }
      }
      return num;
    }
  }
}
