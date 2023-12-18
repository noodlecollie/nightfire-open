// Decompiled with JetBrains decompiler
// Type: JbnLib.Spr.SprFileV1
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Shared;
using JbnLib.Spz;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;

namespace JbnLib.Spr
{
  public class SprFileV1
  {
    public const int VERSION = 1;
    private static Color[] _Palette = new Color[256]
    {
      Color.FromArgb((int) byte.MaxValue, 0, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 15, 15, 15),
      Color.FromArgb((int) byte.MaxValue, 31, 31, 31),
      Color.FromArgb((int) byte.MaxValue, 47, 47, 47),
      Color.FromArgb((int) byte.MaxValue, 63, 63, 63),
      Color.FromArgb((int) byte.MaxValue, 75, 75, 75),
      Color.FromArgb((int) byte.MaxValue, 91, 91, 91),
      Color.FromArgb((int) byte.MaxValue, 107, 107, 107),
      Color.FromArgb((int) byte.MaxValue, 123, 123, 123),
      Color.FromArgb((int) byte.MaxValue, 139, 139, 139),
      Color.FromArgb((int) byte.MaxValue, 155, 155, 155),
      Color.FromArgb((int) byte.MaxValue, 171, 171, 171),
      Color.FromArgb((int) byte.MaxValue, 187, 187, 187),
      Color.FromArgb((int) byte.MaxValue, 203, 203, 203),
      Color.FromArgb((int) byte.MaxValue, 219, 219, 219),
      Color.FromArgb((int) byte.MaxValue, 235, 235, 235),
      Color.FromArgb((int) byte.MaxValue, 15, 11, 7),
      Color.FromArgb((int) byte.MaxValue, 23, 15, 11),
      Color.FromArgb((int) byte.MaxValue, 31, 23, 11),
      Color.FromArgb((int) byte.MaxValue, 39, 27, 15),
      Color.FromArgb((int) byte.MaxValue, 47, 35, 19),
      Color.FromArgb((int) byte.MaxValue, 55, 43, 23),
      Color.FromArgb((int) byte.MaxValue, 63, 47, 23),
      Color.FromArgb((int) byte.MaxValue, 75, 55, 27),
      Color.FromArgb((int) byte.MaxValue, 83, 59, 27),
      Color.FromArgb((int) byte.MaxValue, 91, 67, 31),
      Color.FromArgb((int) byte.MaxValue, 99, 75, 31),
      Color.FromArgb((int) byte.MaxValue, 107, 83, 31),
      Color.FromArgb((int) byte.MaxValue, 115, 87, 31),
      Color.FromArgb((int) byte.MaxValue, 123, 95, 35),
      Color.FromArgb((int) byte.MaxValue, 131, 103, 35),
      Color.FromArgb((int) byte.MaxValue, 143, 111, 35),
      Color.FromArgb((int) byte.MaxValue, 11, 11, 15),
      Color.FromArgb((int) byte.MaxValue, 19, 19, 27),
      Color.FromArgb((int) byte.MaxValue, 27, 27, 39),
      Color.FromArgb((int) byte.MaxValue, 39, 39, 51),
      Color.FromArgb((int) byte.MaxValue, 47, 47, 63),
      Color.FromArgb((int) byte.MaxValue, 55, 55, 75),
      Color.FromArgb((int) byte.MaxValue, 63, 63, 87),
      Color.FromArgb((int) byte.MaxValue, 71, 71, 103),
      Color.FromArgb((int) byte.MaxValue, 79, 79, 115),
      Color.FromArgb((int) byte.MaxValue, 91, 91, (int) sbyte.MaxValue),
      Color.FromArgb((int) byte.MaxValue, 99, 99, 139),
      Color.FromArgb((int) byte.MaxValue, 107, 107, 151),
      Color.FromArgb((int) byte.MaxValue, 115, 115, 163),
      Color.FromArgb((int) byte.MaxValue, 123, 123, 175),
      Color.FromArgb((int) byte.MaxValue, 131, 131, 187),
      Color.FromArgb((int) byte.MaxValue, 139, 139, 203),
      Color.FromArgb((int) byte.MaxValue, 0, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 7, 7, 0),
      Color.FromArgb((int) byte.MaxValue, 11, 11, 0),
      Color.FromArgb((int) byte.MaxValue, 19, 19, 0),
      Color.FromArgb((int) byte.MaxValue, 27, 27, 0),
      Color.FromArgb((int) byte.MaxValue, 35, 35, 0),
      Color.FromArgb((int) byte.MaxValue, 43, 43, 7),
      Color.FromArgb((int) byte.MaxValue, 47, 47, 7),
      Color.FromArgb((int) byte.MaxValue, 55, 55, 7),
      Color.FromArgb((int) byte.MaxValue, 63, 63, 7),
      Color.FromArgb((int) byte.MaxValue, 71, 71, 7),
      Color.FromArgb((int) byte.MaxValue, 75, 75, 11),
      Color.FromArgb((int) byte.MaxValue, 83, 83, 11),
      Color.FromArgb((int) byte.MaxValue, 91, 91, 11),
      Color.FromArgb((int) byte.MaxValue, 99, 99, 11),
      Color.FromArgb((int) byte.MaxValue, 107, 107, 15),
      Color.FromArgb((int) byte.MaxValue, 7, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 15, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 23, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 31, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 39, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 47, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 55, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 63, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 71, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 79, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 87, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 95, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 103, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 111, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 119, 0, 0),
      Color.FromArgb((int) byte.MaxValue, (int) sbyte.MaxValue, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 19, 19, 0),
      Color.FromArgb((int) byte.MaxValue, 27, 27, 0),
      Color.FromArgb((int) byte.MaxValue, 35, 35, 0),
      Color.FromArgb((int) byte.MaxValue, 47, 43, 0),
      Color.FromArgb((int) byte.MaxValue, 55, 47, 0),
      Color.FromArgb((int) byte.MaxValue, 67, 55, 0),
      Color.FromArgb((int) byte.MaxValue, 75, 59, 7),
      Color.FromArgb((int) byte.MaxValue, 87, 67, 7),
      Color.FromArgb((int) byte.MaxValue, 95, 71, 7),
      Color.FromArgb((int) byte.MaxValue, 107, 75, 11),
      Color.FromArgb((int) byte.MaxValue, 119, 83, 15),
      Color.FromArgb((int) byte.MaxValue, 131, 87, 19),
      Color.FromArgb((int) byte.MaxValue, 139, 91, 19),
      Color.FromArgb((int) byte.MaxValue, 151, 95, 27),
      Color.FromArgb((int) byte.MaxValue, 163, 99, 31),
      Color.FromArgb((int) byte.MaxValue, 175, 103, 35),
      Color.FromArgb((int) byte.MaxValue, 35, 19, 7),
      Color.FromArgb((int) byte.MaxValue, 47, 23, 11),
      Color.FromArgb((int) byte.MaxValue, 59, 31, 15),
      Color.FromArgb((int) byte.MaxValue, 75, 35, 19),
      Color.FromArgb((int) byte.MaxValue, 87, 43, 23),
      Color.FromArgb((int) byte.MaxValue, 99, 47, 31),
      Color.FromArgb((int) byte.MaxValue, 115, 55, 35),
      Color.FromArgb((int) byte.MaxValue, (int) sbyte.MaxValue, 59, 43),
      Color.FromArgb((int) byte.MaxValue, 143, 67, 51),
      Color.FromArgb((int) byte.MaxValue, 159, 79, 51),
      Color.FromArgb((int) byte.MaxValue, 175, 99, 47),
      Color.FromArgb((int) byte.MaxValue, 191, 119, 47),
      Color.FromArgb((int) byte.MaxValue, 207, 143, 43),
      Color.FromArgb((int) byte.MaxValue, 223, 171, 39),
      Color.FromArgb((int) byte.MaxValue, 239, 203, 31),
      Color.FromArgb((int) byte.MaxValue, (int) byte.MaxValue, 243, 27),
      Color.FromArgb((int) byte.MaxValue, 11, 7, 0),
      Color.FromArgb((int) byte.MaxValue, 27, 19, 0),
      Color.FromArgb((int) byte.MaxValue, 43, 35, 15),
      Color.FromArgb((int) byte.MaxValue, 55, 43, 19),
      Color.FromArgb((int) byte.MaxValue, 71, 51, 27),
      Color.FromArgb((int) byte.MaxValue, 83, 55, 35),
      Color.FromArgb((int) byte.MaxValue, 99, 63, 43),
      Color.FromArgb((int) byte.MaxValue, 111, 71, 51),
      Color.FromArgb((int) byte.MaxValue, (int) sbyte.MaxValue, 83, 63),
      Color.FromArgb((int) byte.MaxValue, 139, 95, 71),
      Color.FromArgb((int) byte.MaxValue, 155, 107, 83),
      Color.FromArgb((int) byte.MaxValue, 167, 123, 95),
      Color.FromArgb((int) byte.MaxValue, 183, 135, 107),
      Color.FromArgb((int) byte.MaxValue, 195, 147, 123),
      Color.FromArgb((int) byte.MaxValue, 211, 163, 139),
      Color.FromArgb((int) byte.MaxValue, 227, 179, 151),
      Color.FromArgb((int) byte.MaxValue, 171, 139, 163),
      Color.FromArgb((int) byte.MaxValue, 159, (int) sbyte.MaxValue, 151),
      Color.FromArgb((int) byte.MaxValue, 147, 115, 135),
      Color.FromArgb((int) byte.MaxValue, 139, 103, 123),
      Color.FromArgb((int) byte.MaxValue, (int) sbyte.MaxValue, 91, 111),
      Color.FromArgb((int) byte.MaxValue, 119, 83, 99),
      Color.FromArgb((int) byte.MaxValue, 107, 75, 87),
      Color.FromArgb((int) byte.MaxValue, 95, 63, 75),
      Color.FromArgb((int) byte.MaxValue, 87, 55, 67),
      Color.FromArgb((int) byte.MaxValue, 75, 47, 55),
      Color.FromArgb((int) byte.MaxValue, 67, 39, 47),
      Color.FromArgb((int) byte.MaxValue, 55, 31, 35),
      Color.FromArgb((int) byte.MaxValue, 43, 23, 27),
      Color.FromArgb((int) byte.MaxValue, 35, 19, 19),
      Color.FromArgb((int) byte.MaxValue, 23, 11, 11),
      Color.FromArgb((int) byte.MaxValue, 15, 7, 7),
      Color.FromArgb((int) byte.MaxValue, 187, 115, 159),
      Color.FromArgb((int) byte.MaxValue, 175, 107, 143),
      Color.FromArgb((int) byte.MaxValue, 163, 95, 131),
      Color.FromArgb((int) byte.MaxValue, 151, 87, 119),
      Color.FromArgb((int) byte.MaxValue, 139, 79, 107),
      Color.FromArgb((int) byte.MaxValue, (int) sbyte.MaxValue, 75, 95),
      Color.FromArgb((int) byte.MaxValue, 115, 67, 83),
      Color.FromArgb((int) byte.MaxValue, 107, 59, 75),
      Color.FromArgb((int) byte.MaxValue, 95, 51, 63),
      Color.FromArgb((int) byte.MaxValue, 83, 43, 55),
      Color.FromArgb((int) byte.MaxValue, 71, 35, 43),
      Color.FromArgb((int) byte.MaxValue, 59, 31, 35),
      Color.FromArgb((int) byte.MaxValue, 47, 23, 27),
      Color.FromArgb((int) byte.MaxValue, 35, 19, 19),
      Color.FromArgb((int) byte.MaxValue, 23, 11, 11),
      Color.FromArgb((int) byte.MaxValue, 15, 7, 7),
      Color.FromArgb((int) byte.MaxValue, 219, 195, 187),
      Color.FromArgb((int) byte.MaxValue, 203, 179, 167),
      Color.FromArgb((int) byte.MaxValue, 191, 163, 155),
      Color.FromArgb((int) byte.MaxValue, 175, 151, 139),
      Color.FromArgb((int) byte.MaxValue, 163, 135, 123),
      Color.FromArgb((int) byte.MaxValue, 151, 123, 111),
      Color.FromArgb((int) byte.MaxValue, 135, 111, 95),
      Color.FromArgb((int) byte.MaxValue, 123, 99, 83),
      Color.FromArgb((int) byte.MaxValue, 107, 87, 71),
      Color.FromArgb((int) byte.MaxValue, 95, 75, 59),
      Color.FromArgb((int) byte.MaxValue, 83, 63, 51),
      Color.FromArgb((int) byte.MaxValue, 67, 51, 39),
      Color.FromArgb((int) byte.MaxValue, 55, 43, 31),
      Color.FromArgb((int) byte.MaxValue, 39, 31, 23),
      Color.FromArgb((int) byte.MaxValue, 27, 19, 15),
      Color.FromArgb((int) byte.MaxValue, 15, 11, 7),
      Color.FromArgb((int) byte.MaxValue, 111, 131, 123),
      Color.FromArgb((int) byte.MaxValue, 103, 123, 111),
      Color.FromArgb((int) byte.MaxValue, 95, 115, 103),
      Color.FromArgb((int) byte.MaxValue, 87, 107, 95),
      Color.FromArgb((int) byte.MaxValue, 79, 99, 87),
      Color.FromArgb((int) byte.MaxValue, 71, 91, 79),
      Color.FromArgb((int) byte.MaxValue, 63, 83, 71),
      Color.FromArgb((int) byte.MaxValue, 55, 75, 63),
      Color.FromArgb((int) byte.MaxValue, 47, 67, 55),
      Color.FromArgb((int) byte.MaxValue, 43, 59, 47),
      Color.FromArgb((int) byte.MaxValue, 35, 51, 39),
      Color.FromArgb((int) byte.MaxValue, 31, 43, 31),
      Color.FromArgb((int) byte.MaxValue, 23, 35, 23),
      Color.FromArgb((int) byte.MaxValue, 15, 27, 19),
      Color.FromArgb((int) byte.MaxValue, 11, 19, 11),
      Color.FromArgb((int) byte.MaxValue, 7, 11, 7),
      Color.FromArgb((int) byte.MaxValue, (int) byte.MaxValue, 243, 27),
      Color.FromArgb((int) byte.MaxValue, 239, 223, 23),
      Color.FromArgb((int) byte.MaxValue, 219, 203, 19),
      Color.FromArgb((int) byte.MaxValue, 203, 183, 15),
      Color.FromArgb((int) byte.MaxValue, 187, 167, 15),
      Color.FromArgb((int) byte.MaxValue, 171, 151, 11),
      Color.FromArgb((int) byte.MaxValue, 155, 131, 7),
      Color.FromArgb((int) byte.MaxValue, 139, 115, 7),
      Color.FromArgb((int) byte.MaxValue, 123, 99, 7),
      Color.FromArgb((int) byte.MaxValue, 107, 83, 0),
      Color.FromArgb((int) byte.MaxValue, 91, 71, 0),
      Color.FromArgb((int) byte.MaxValue, 75, 55, 0),
      Color.FromArgb((int) byte.MaxValue, 59, 43, 0),
      Color.FromArgb((int) byte.MaxValue, 43, 31, 0),
      Color.FromArgb((int) byte.MaxValue, 27, 15, 0),
      Color.FromArgb((int) byte.MaxValue, 11, 7, 0),
      Color.FromArgb((int) byte.MaxValue, 0, 0, (int) byte.MaxValue),
      Color.FromArgb((int) byte.MaxValue, 11, 11, 239),
      Color.FromArgb((int) byte.MaxValue, 19, 19, 223),
      Color.FromArgb((int) byte.MaxValue, 27, 27, 207),
      Color.FromArgb((int) byte.MaxValue, 35, 35, 191),
      Color.FromArgb((int) byte.MaxValue, 43, 43, 175),
      Color.FromArgb((int) byte.MaxValue, 47, 47, 159),
      Color.FromArgb((int) byte.MaxValue, 47, 47, 143),
      Color.FromArgb((int) byte.MaxValue, 47, 47, (int) sbyte.MaxValue),
      Color.FromArgb((int) byte.MaxValue, 47, 47, 111),
      Color.FromArgb((int) byte.MaxValue, 47, 47, 95),
      Color.FromArgb((int) byte.MaxValue, 43, 43, 79),
      Color.FromArgb((int) byte.MaxValue, 35, 35, 63),
      Color.FromArgb((int) byte.MaxValue, 27, 27, 47),
      Color.FromArgb((int) byte.MaxValue, 19, 19, 31),
      Color.FromArgb((int) byte.MaxValue, 11, 11, 15),
      Color.FromArgb((int) byte.MaxValue, 43, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 59, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 75, 7, 0),
      Color.FromArgb((int) byte.MaxValue, 95, 7, 0),
      Color.FromArgb((int) byte.MaxValue, 111, 15, 0),
      Color.FromArgb((int) byte.MaxValue, (int) sbyte.MaxValue, 23, 7),
      Color.FromArgb((int) byte.MaxValue, 147, 31, 7),
      Color.FromArgb((int) byte.MaxValue, 163, 39, 11),
      Color.FromArgb((int) byte.MaxValue, 183, 51, 15),
      Color.FromArgb((int) byte.MaxValue, 195, 75, 27),
      Color.FromArgb((int) byte.MaxValue, 207, 99, 43),
      Color.FromArgb((int) byte.MaxValue, 219, (int) sbyte.MaxValue, 59),
      Color.FromArgb((int) byte.MaxValue, 227, 151, 79),
      Color.FromArgb((int) byte.MaxValue, 231, 171, 95),
      Color.FromArgb((int) byte.MaxValue, 239, 191, 119),
      Color.FromArgb((int) byte.MaxValue, 247, 211, 139),
      Color.FromArgb((int) byte.MaxValue, 167, 123, 59),
      Color.FromArgb((int) byte.MaxValue, 183, 155, 55),
      Color.FromArgb((int) byte.MaxValue, 199, 195, 55),
      Color.FromArgb((int) byte.MaxValue, 231, 227, 87),
      Color.FromArgb((int) byte.MaxValue, (int) sbyte.MaxValue, 191, (int) byte.MaxValue),
      Color.FromArgb((int) byte.MaxValue, 171, 231, (int) byte.MaxValue),
      Color.FromArgb((int) byte.MaxValue, 215, (int) byte.MaxValue, (int) byte.MaxValue),
      Color.FromArgb((int) byte.MaxValue, 103, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 139, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 179, 0, 0),
      Color.FromArgb((int) byte.MaxValue, 215, 0, 0),
      Color.FromArgb((int) byte.MaxValue, (int) byte.MaxValue, 0, 0),
      Color.FromArgb((int) byte.MaxValue, (int) byte.MaxValue, 243, 147),
      Color.FromArgb((int) byte.MaxValue, (int) byte.MaxValue, 247, 199),
      Color.FromArgb((int) byte.MaxValue, (int) byte.MaxValue, (int) byte.MaxValue, (int) byte.MaxValue),
      Color.FromArgb(0, 159, 91, 83)
    };
    private HeaderV1 _Header;
    private List<object> _FrameCollection;

    public HeaderV1 Header
    {
      get => this._Header;
      set => this._Header = value;
    }

    public List<object> FramCollection
    {
      get => this._FrameCollection;
      set => this._FrameCollection = value;
    }

    public void Clear()
    {
      this._Header = new HeaderV1();
      this._FrameCollection = new List<object>();
    }

    public int Read(string file)
    {
      this.Clear();
      FileStream input = new FileInfo(file).OpenRead();
      BinaryReader binaryReader = new BinaryReader((Stream) input);
      input.Seek(0L, SeekOrigin.Begin);
      this._Header = new HeaderV1(binaryReader.ReadBytes(HeaderV1.Length));
      for (int index1 = 0; index1 < this._Header.FrameCount; ++index1)
      {
        if (binaryReader.ReadInt32() == 0)
        {
          Frame frame = new Frame();
          frame.SetV1(binaryReader.ReadBytes(Frame.Length));
          byte[] numArray = binaryReader.ReadBytes(frame.Width * frame.Height);
          Bitmap bitmap = new Bitmap(frame.Width, frame.Height);
          int num = 0;
          for (int x = 0; x < frame.Width; ++x)
          {
            for (int y = 0; y < frame.Height; ++y)
              bitmap.SetPixel(x, y, SprFileV1._Palette[(int) numArray[num++]]);
          }
          frame.Image = (Image) bitmap;
          this._FrameCollection.Add((object) frame);
        }
        else
        {
          Group group = new Group();
          int num1 = binaryReader.ReadInt32();
          for (int index2 = 0; index2 < num1; ++index2)
            group.Intervals.Add(binaryReader.ReadSingle());
          for (int index3 = 0; index3 < num1; ++index3)
          {
            Frame frame = new Frame();
            frame.SetV1(binaryReader.ReadBytes(Frame.Length));
            byte[] numArray = binaryReader.ReadBytes(frame.Width * frame.Height);
            Bitmap bitmap = new Bitmap(frame.Width, frame.Height);
            int num2 = 0;
            for (int x = 0; x < frame.Width; ++x)
            {
              for (int y = 0; y < frame.Height; ++y)
                bitmap.SetPixel(x, y, SprFileV1._Palette[(int) numArray[num2++]]);
            }
            frame.Image = (Image) bitmap;
            group.FrameCollection.Add(frame);
          }
          this._FrameCollection.Add((object) group);
        }
      }
      binaryReader.Close();
      input.Close();
      return this._FrameCollection.Count;
    }

    public void DumpSpz(string file, string texturedir)
    {
      FileInfo fileInfo = new FileInfo(file);
      string str1 = StaticMethods.StripExtension(fileInfo.Name);
      texturedir = texturedir.TrimEnd('\\');
      Directory.CreateDirectory(fileInfo.DirectoryName);
      Directory.CreateDirectory(texturedir);
      SpzFile spzFile = new SpzFile();
      spzFile.Type = (SpriteZType) Enum.Parse(typeof (SpriteZType), this._Header.Type.ToString(), true);
      spzFile.Texture = SpriteZTexture.normal;
      for (int index1 = 0; index1 < this._FrameCollection.Count; ++index1)
      {
        string str2 = str1 + "_" + (object) index1;
        if (this._FrameCollection[index1].GetType() == typeof (Frame))
        {
          ((Frame) this._FrameCollection[index1]).Image.Save(texturedir + "\\" + str2 + ".png", ImageFormat.Png);
          spzFile.Images.Add(str2 + ".png");
        }
        else
        {
          Group frame = (Group) this._FrameCollection[index1];
          for (int index2 = 0; index2 < frame.FrameCollection.Count; ++index2)
          {
            frame.FrameCollection[index2].Image.Save(texturedir + "\\" + str2 + "_" + (object) index2 + ".png", ImageFormat.Png);
            spzFile.Images.Add(str2 + "_" + (object) index2 + ".png");
          }
        }
      }
      spzFile.Write(fileInfo.FullName);
    }
  }
}
