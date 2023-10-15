// Decompiled with JetBrains decompiler
// Type: JbnLib.Dat.DatFile
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using System;
using System.IO;

namespace JbnLib.Dat
{
  public class DatFile
  {
    private DatCellSize _CellSize;
    private DatCharacter[] _CharacterCollection;

    public DatCellSize CellSize
    {
      get => this._CellSize;
      set => this._CellSize = value;
    }

    public DatCharacter[] CharacterCollection
    {
      get => this._CharacterCollection;
      set => this._CharacterCollection = value;
    }

    public DatFile() => this.Clear();

    public DatFile(string file) => this.Read(file);

    public void Clear()
    {
      this._CellSize = new DatCellSize();
      this._CharacterCollection = new DatCharacter[256];
    }

    public void Read(string file)
    {
      this.Clear();
      int num = 1;
      StreamReader streamReader = new StreamReader(file);
      while (streamReader.Peek() > 0)
      {
        string[] strArray = streamReader.ReadLine().Split(' ');
        if (strArray.Length == 3)
        {
          if (strArray[0].ToLower() != "cellsize")
            Messages.ThrowException("Dat.DatFile.Read(string)", Messages.COMPARE_STRING + Messages.EXPECTING + "cellsize " + Messages.GOT + strArray[0] + " " + Messages.LINE + (object) num);
          this._CellSize = new DatCellSize(Convert.ToUInt16(strArray[1]), Convert.ToUInt16(strArray[2]));
        }
        else if (strArray.Length == 5)
          this._CharacterCollection[(int) Convert.ToByte(strArray[0])] = new DatCharacter(Convert.ToUInt16(strArray[1]), Convert.ToUInt16(strArray[2]), Convert.ToUInt16(strArray[3]), Convert.ToUInt16(strArray[4]));
        else
          Messages.ThrowException("Dat.DatFile.Read(string)", Messages.COMPARE_ARRAY + Messages.EXPECTING + "5 " + Messages.GOT + (object) strArray.Length + " " + Messages.LINE + (object) num);
        ++num;
      }
      streamReader.Close();
    }

    public void Write(string file)
    {
      StreamWriter streamWriter = new StreamWriter(file);
      streamWriter.WriteLine(this._CellSize.ToString());
      for (int index = 0; index < this._CharacterCollection.Length; ++index)
        streamWriter.WriteLine(index.ToString() + " " + this._CharacterCollection[index].ToString());
      streamWriter.Write("\0");
      streamWriter.Flush();
      streamWriter.Close();
    }
  }
}
