// Decompiled with JetBrains decompiler
// Type: JbnLib.Spr.SprFileV32
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
  public class SprFileV32
  {
    public const int VERSION = 32;
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
          byte[] numArray1 = binaryReader.ReadBytes(frame.Width * frame.Height * 4);
          Bitmap bitmap1 = new Bitmap(frame.Width, frame.Height);
          int num1 = 0;
          for (int index2 = 0; index2 < frame.Width; ++index2)
          {
            for (int index3 = 0; index3 < frame.Height; ++index3)
            {
              byte[] numArray2 = numArray1;
              int index4 = num1;
              int num2 = index4 + 1;
              byte red = numArray2[index4];
              byte[] numArray3 = numArray1;
              int index5 = num2;
              int num3 = index5 + 1;
              byte green = numArray3[index5];
              byte[] numArray4 = numArray1;
              int index6 = num3;
              int num4 = index6 + 1;
              byte blue = numArray4[index6];
              Bitmap bitmap2 = bitmap1;
              int x = index2;
              int y = index3;
              byte[] numArray5 = numArray1;
              int index7 = num4;
              num1 = index7 + 1;
              Color color = Color.FromArgb((int) numArray5[index7], (int) red, (int) green, (int) blue);
              bitmap2.SetPixel(x, y, color);
            }
          }
          frame.Image = (Image) bitmap1;
          this._FrameCollection.Add((object) frame);
        }
        else
        {
          Group group = new Group();
          int num5 = binaryReader.ReadInt32();
          for (int index8 = 0; index8 < num5; ++index8)
            group.Intervals.Add(binaryReader.ReadSingle());
          for (int index9 = 0; index9 < num5; ++index9)
          {
            Frame frame = new Frame();
            frame.SetV1(binaryReader.ReadBytes(Frame.Length));
            byte[] numArray6 = binaryReader.ReadBytes(frame.Width * frame.Height * 4);
            Bitmap bitmap3 = new Bitmap(frame.Width, frame.Height);
            int num6 = 0;
            for (int index10 = 0; index10 < frame.Width; ++index10)
            {
              for (int index11 = 0; index11 < frame.Height; ++index11)
              {
                byte[] numArray7 = numArray6;
                int index12 = num6;
                int num7 = index12 + 1;
                byte red = numArray7[index12];
                byte[] numArray8 = numArray6;
                int index13 = num7;
                int num8 = index13 + 1;
                byte green = numArray8[index13];
                byte[] numArray9 = numArray6;
                int index14 = num8;
                int num9 = index14 + 1;
                byte blue = numArray9[index14];
                Bitmap bitmap4 = bitmap3;
                int x = index10;
                int y = index11;
                byte[] numArray10 = numArray6;
                int index15 = num9;
                num6 = index15 + 1;
                Color color = Color.FromArgb((int) numArray10[index15], (int) red, (int) green, (int) blue);
                bitmap4.SetPixel(x, y, color);
              }
            }
            frame.Image = (Image) bitmap3;
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
