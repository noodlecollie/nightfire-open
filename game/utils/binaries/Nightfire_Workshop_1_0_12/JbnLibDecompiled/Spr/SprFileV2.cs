// Decompiled with JetBrains decompiler
// Type: JbnLib.Spr.SprFileV2
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
using System.Runtime.InteropServices;

namespace JbnLib.Spr
{
  public class SprFileV2
  {
    public const int VERSION = 2;
    private bool _Do16bit = true;
    private HeaderV2 _Header;
    private Color24bit[] _Palette;
    private List<object> _FrameCollection;

    public bool Do16bit
    {
      get => this._Do16bit;
      set => this._Do16bit = value;
    }

    public HeaderV2 Header
    {
      get => this._Header;
      set => this._Header = value;
    }

    public Color24bit[] Palette
    {
      get => this._Palette;
      set => this._Palette = value;
    }

    public Color[] GetColorPalette()
    {
      Color[] colorPalette = new Color[this._Palette.Length];
      for (int index = 0; index < colorPalette.Length; ++index)
        colorPalette[index] = this._Palette[index].GetColor();
      return colorPalette;
    }

    public List<object> FramCollection
    {
      get => this._FrameCollection;
      set => this._FrameCollection = value;
    }

    public void Clear()
    {
      this._Header = new HeaderV2();
      this._Palette = (Color24bit[]) null;
      this._FrameCollection = new List<object>();
    }

    public int Read(string file)
    {
      this.Clear();
      FileStream input = new FileInfo(file).OpenRead();
      BinaryReader binaryReader = new BinaryReader((Stream) input);
      input.Seek(0L, SeekOrigin.Begin);
      this._Header = new HeaderV2(binaryReader.ReadBytes(HeaderV2.Length));
      if (this._Do16bit)
      {
        short length = binaryReader.ReadInt16();
        this._Palette = new Color24bit[(int) length];
        byte[] numArray1 = binaryReader.ReadBytes(3 * (int) length);
        short num1 = 0;
        for (short index1 = 0; (int) index1 < (int) length; ++index1)
        {
          Color24bit[] palette = this._Palette;
          int index2 = (int) index1;
          byte[] numArray2 = numArray1;
          int index3 = (int) num1;
          short num2 = (short) (index3 + 1);
          int r = (int) numArray2[index3];
          byte[] numArray3 = numArray1;
          int index4 = (int) num2;
          short num3 = (short) (index4 + 1);
          int g = (int) numArray3[index4];
          byte[] numArray4 = numArray1;
          int index5 = (int) num3;
          num1 = (short) (index5 + 1);
          int b = (int) numArray4[index5];
          Color24bit color24bit = new Color24bit((byte) r, (byte) g, (byte) b);
          palette[index2] = color24bit;
        }
      }
      for (int index6 = 0; index6 < this._Header.FrameCount; ++index6)
      {
        if (binaryReader.ReadInt32() == 0)
        {
          Frame frame = new Frame(binaryReader.ReadBytes(Frame.Length));
          if (this._Do16bit)
          {
            Color[] colorPalette = this.GetColorPalette();
            byte[] numArray = binaryReader.ReadBytes(frame.Width * frame.Height);
            Bitmap bitmap = new Bitmap(frame.Width, frame.Height);
            int index7 = 0;
            for (int x = 0; x < frame.Width; ++x)
            {
              for (int y = 0; y < frame.Height; ++y)
              {
                if (this._Header.Texture == SpriteTexture.indexalpha)
                  bitmap.SetPixel(x, y, Color.FromArgb((int) numArray[index7], colorPalette[(int) numArray[index7++]]));
                else
                  bitmap.SetPixel(x, y, colorPalette[(int) numArray[index7++]]);
              }
            }
            if (this._Header.Texture == SpriteTexture.alphatest)
              bitmap.MakeTransparent(colorPalette[(int) byte.MaxValue]);
            frame.Image = (Image) bitmap;
          }
          else
            frame.Image = (Image) new Bitmap(frame.Width, frame.Height, frame.Width, PixelFormat.Format8bppIndexed, Marshal.UnsafeAddrOfPinnedArrayElement((Array) binaryReader.ReadBytes(frame.Width * frame.Height), 0));
          this._FrameCollection.Add((object) frame);
        }
        else
        {
          Group group = new Group();
          int num = binaryReader.ReadInt32();
          for (int index8 = 0; index8 < num; ++index8)
            group.Intervals.Add(binaryReader.ReadSingle());
          for (int index9 = 0; index9 < num; ++index9)
          {
            Frame frame = new Frame(binaryReader.ReadBytes(Frame.Length));
            if (this._Do16bit)
            {
              Color[] colorPalette = this.GetColorPalette();
              byte[] numArray = binaryReader.ReadBytes(frame.Width * frame.Height);
              Bitmap bitmap = new Bitmap(frame.Width, frame.Height);
              int index10 = 0;
              for (int x = 0; x < frame.Width; ++x)
              {
                for (int y = 0; y < frame.Height; ++y)
                {
                  if (this._Header.Texture == SpriteTexture.indexalpha)
                    bitmap.SetPixel(x, y, Color.FromArgb((int) numArray[index10], colorPalette[(int) numArray[index10++]]));
                  else
                    bitmap.SetPixel(x, y, colorPalette[(int) numArray[index10++]]);
                }
              }
              if (this._Header.Texture == SpriteTexture.alphatest)
                bitmap.MakeTransparent(colorPalette[(int) byte.MaxValue]);
              frame.Image = (Image) bitmap;
            }
            else
              frame.Image = (Image) new Bitmap(frame.Width, frame.Height, frame.Width, PixelFormat.Format8bppIndexed, Marshal.UnsafeAddrOfPinnedArrayElement((Array) binaryReader.ReadBytes(frame.Width * frame.Height), 0));
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
      spzFile.Texture = (SpriteZTexture) Enum.Parse(typeof (SpriteZTexture), this._Header.Texture.ToString(), true);
      for (int index1 = 0; index1 < this._FrameCollection.Count; ++index1)
      {
        string str2 = str1 + "_" + (object) index1;
        if (this._FrameCollection[index1].GetType() == typeof (Frame))
        {
          ((Frame) this._FrameCollection[index1]).Image.Save(texturedir + "\\" + str2, ImageFormat.Png);
          spzFile.Images.Add(str2);
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
