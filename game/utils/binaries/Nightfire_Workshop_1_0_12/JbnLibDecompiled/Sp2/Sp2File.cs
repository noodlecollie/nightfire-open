// Decompiled with JetBrains decompiler
// Type: JbnLib.Sp2.Sp2File
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using JbnLib.Shared;
using JbnLib.Spz;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace JbnLib.Sp2
{
  public class Sp2File
  {
    public const string IDENTIFIER = "IDS2";
    public const int VERSION = 2;
    private List<Frame> _FrameCollection;

    public List<Frame> FrameCollection
    {
      get => this._FrameCollection;
      set => this._FrameCollection = value;
    }

    public Sp2File() => this.Clear();

    public void Clear() => this._FrameCollection = new List<Frame>();

    public int Read(string file)
    {
      this.Clear();
      FileStream input = new FileStream(file, FileMode.Open, FileAccess.Read);
      BinaryReader binaryReader = new BinaryReader((Stream) input);
      string str = Encoding.ASCII.GetString(binaryReader.ReadBytes(4));
      if (str != "IDS2")
        Messages.ThrowException("Sp2.Sp2File.Read(string)", Messages.IDENTIFIER_MISMATCH + Messages.EXPECTING + "IDS2 " + Messages.GOT + str);
      int num1 = binaryReader.ReadInt32();
      if (num1 != 2)
        Messages.ThrowException("Sp2.Sp2File.Read(string)", Messages.VERSION_MISMATCH + Messages.EXPECTING + (object) 2 + " " + Messages.GOT + (object) num1);
      int num2 = binaryReader.ReadInt32();
      for (int index = 0; index < num2; ++index)
        this._FrameCollection.Add(new Frame(binaryReader.ReadBytes(Frame.Length)));
      binaryReader.Close();
      input.Close();
      return this._FrameCollection.Count;
    }

    public string Write(string file)
    {
      FileStream output = new FileStream(file, FileMode.Create, FileAccess.Write);
      BinaryWriter binaryWriter = new BinaryWriter((Stream) output);
      binaryWriter.Write(Encoding.ASCII.GetBytes("IDS2"));
      binaryWriter.Write(2);
      binaryWriter.Write(this._FrameCollection.Count);
      for (int index = 0; index < this._FrameCollection.Count; ++index)
        binaryWriter.Write(this._FrameCollection[index].GetBytes());
      binaryWriter.Flush();
      binaryWriter.Close();
      output.Close();
      return file;
    }

    public SpzFile ToSpzFile()
    {
      SpzFile spzFile = new SpzFile();
      spzFile.Type = SpriteZType.vp_parallel;
      spzFile.Texture = SpriteZTexture.normal;
      foreach (Frame frame in this._FrameCollection)
        spzFile.Images.Add(StaticMethods.StripExtension(frame.Image) + ".png");
      return spzFile;
    }
  }
}
