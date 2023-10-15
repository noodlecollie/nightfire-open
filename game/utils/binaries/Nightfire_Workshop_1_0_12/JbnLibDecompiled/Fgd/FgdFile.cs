// Decompiled with JetBrains decompiler
// Type: JbnLib.Fgd.FgdFile
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Fgd.Types;
using System.Collections.Generic;
using System.IO;

namespace JbnLib.Fgd
{
  public class FgdFile
  {
    private List<BaseClass> _BaseClassCollection = new List<BaseClass>();
    private List<PointClass> _PointClassCollection = new List<PointClass>();
    private List<SolidClass> _SolidClassCollection = new List<SolidClass>();

    public List<BaseClass> BaseClassCollection
    {
      get => this._BaseClassCollection;
      set => this._BaseClassCollection = value;
    }

    public List<PointClass> PointClassCollection
    {
      get => this._PointClassCollection;
      set => this._PointClassCollection = value;
    }

    public List<SolidClass> SolidClassCollection
    {
      get => this._SolidClassCollection;
      set => this._SolidClassCollection = value;
    }

    public void Parse(string file)
    {
      List<string> stringList1 = new List<string>();
      int num = -1;
      StreamReader streamReader = new StreamReader(file);
      while (streamReader.Peek() != -1)
      {
        string str = streamReader.ReadLine();
        if (!str.StartsWith("//"))
        {
          if (str.StartsWith("@"))
          {
            ++num;
            stringList1.Add(str);
          }
          else if (num != -1)
          {
            List<string> stringList2 = stringList1;
            int index = num;
            stringList2[index] = stringList2[index] + "\n" + str;
          }
        }
      }
      streamReader.Close();
      for (int index = 0; index < stringList1.Count; ++index)
      {
        stringList1[index] = stringList1[index].Trim();
        if (stringList1[index].StartsWith("@BaseClass "))
        {
          BaseClass baseClass = new BaseClass();
          baseClass.Parse(stringList1[index].Substring(11));
          this._BaseClassCollection.Add(baseClass);
        }
        else if (stringList1[index].StartsWith("@PointClass "))
        {
          PointClass pointClass = new PointClass();
          pointClass.Parse(stringList1[index].Substring(12));
          this._PointClassCollection.Add(pointClass);
        }
        else if (stringList1[index].StartsWith("@SolidClass "))
        {
          SolidClass solidClass = new SolidClass();
          solidClass.Parse(stringList1[index].Substring(12));
          this._SolidClassCollection.Add(solidClass);
        }
      }
    }

    public void Save(string file)
    {
      StreamWriter streamWriter = new StreamWriter(file);
      streamWriter.Write("//\n// Base Classes\n//\n\n");
      foreach (BaseClass baseClass in this._BaseClassCollection)
      {
        streamWriter.Write(baseClass.ToString());
        streamWriter.Flush();
      }
      streamWriter.Write("\n//\n// Point Classes\n//\n\n");
      foreach (PointClass pointClass in this._PointClassCollection)
      {
        streamWriter.Write(pointClass.ToString());
        streamWriter.Flush();
      }
      streamWriter.Write("\n//\n// Solid Classes\n//\n\n");
      foreach (SolidClass solidClass in this._SolidClassCollection)
      {
        streamWriter.Write(solidClass.ToString());
        streamWriter.Flush();
      }
      streamWriter.Close();
    }
  }
}
