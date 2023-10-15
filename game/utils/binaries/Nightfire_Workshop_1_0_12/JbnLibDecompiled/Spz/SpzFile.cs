// Decompiled with JetBrains decompiler
// Type: JbnLib.Spz.SpzFile
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Text;

namespace JbnLib.Spz
{
  public class SpzFile
  {
    public const string IDENTIFIER = "SPZ2";
    private SpriteZType _Type;
    private SpriteZTexture _Texture;
    private byte _Unknown;
    private List<string> _Images;

    [Category("Sprite")]
    [DefaultValue(SpriteZType.vp_parallel)]
    [Description("The type of sprite this is.")]
    public SpriteZType Type
    {
      get => this._Type;
      set => this._Type = value;
    }

    [Category("Sprite")]
    [DefaultValue(SpriteZTexture.normal)]
    [Description("How the textures are handled.")]
    public SpriteZTexture Texture
    {
      get => this._Texture;
      set => this._Texture = value;
    }

    [Category("Sprite")]
    [Description("List of textures. One per line including the .png extension.  The name cannot exceed 67 bytes in length.")]
    public List<string> Images
    {
      get => this._Images;
      set => this._Images = value;
    }

    public SpzFile() => this.Clear();

    public void Clear()
    {
      this._Type = SpriteZType.vp_parallel;
      this._Texture = SpriteZTexture.normal;
      this._Unknown = (byte) 0;
      this._Images = new List<string>();
    }

    public int Read(string file)
    {
      this.Clear();
      FileStream input = new FileStream(file, FileMode.Open, FileAccess.Read);
      BinaryReader binaryReader = new BinaryReader((Stream) input);
      string str = Encoding.ASCII.GetString(binaryReader.ReadBytes(4));
      if (str != "SPZ2")
        Messages.ThrowException("Spz.SpzFile.Read(string)", Messages.IDENTIFIER_MISMATCH + Messages.EXPECTING + "SPZ2 " + Messages.GOT + str);
      this._Type = (SpriteZType) binaryReader.ReadByte();
      this._Texture = (SpriteZTexture) binaryReader.ReadByte();
      this._Unknown = binaryReader.ReadByte();
      byte num = binaryReader.ReadByte();
      for (int index = 0; index < (int) num; ++index)
        this._Images.Add(Encoding.ASCII.GetString(binaryReader.ReadBytes(68)).TrimEnd(new char[1]));
      binaryReader.Close();
      input.Close();
      return this._Images.Count;
    }

    public string Write(string file)
    {
      if (this._Images.Count > (int) byte.MaxValue)
        Messages.ThrowException("Spz.SpzFile.Write(string)", Messages.MAX_IMAGE);
      FileStream output = new FileStream(file, FileMode.Create, FileAccess.Write);
      BinaryWriter binaryWriter = new BinaryWriter((Stream) output);
      binaryWriter.Write(Encoding.ASCII.GetBytes("SPZ2"));
      binaryWriter.Write((byte) this._Type);
      binaryWriter.Write((byte) this._Texture);
      binaryWriter.Write(this._Unknown);
      binaryWriter.Write(Convert.ToByte(this._Images.Count));
      for (int index = 0; index < this._Images.Count; ++index)
        binaryWriter.Write(SpzFile.TrimImage(this._Images[index]));
      binaryWriter.Flush();
      binaryWriter.Close();
      output.Close();
      return file;
    }

    public int ParseQc(string file)
    {
      this.Clear();
      StreamReader streamReader = new StreamReader(file);
      while (streamReader.Peek() != -1)
      {
        string[] strArray = SpzFile.Split(streamReader.ReadLine());
        if (strArray[0] != "NULL")
        {
          switch (strArray[0].ToLower())
          {
            case "$texture":
              switch (strArray[1].ToLower())
              {
                case "normal":
                  this._Texture = SpriteZTexture.normal;
                  continue;
                case "additive":
                  this._Texture = SpriteZTexture.additive;
                  continue;
                case "indexalpha":
                  this._Texture = SpriteZTexture.indexalpha;
                  continue;
                case "alphatest":
                  this._Texture = SpriteZTexture.alphatest;
                  continue;
                default:
                  this._Texture = SpriteZTexture.normal;
                  continue;
              }
            case "$type":
              switch (strArray[1].ToLower())
              {
                case "vp_parallel":
                  this._Type = SpriteZType.vp_parallel;
                  continue;
                case "vp_parallel_upright":
                  this._Type = SpriteZType.vp_parallel_upright;
                  continue;
                case "oriented":
                  this._Type = SpriteZType.oriented;
                  continue;
                case "vp_parallel_oriented":
                  this._Type = SpriteZType.vp_parallel_oriented;
                  continue;
                case "facing_upright":
                  this._Type = SpriteZType.facing_upright;
                  continue;
                default:
                  this._Type = SpriteZType.vp_parallel;
                  continue;
              }
            case "$load":
              this._Images.Add(strArray[1]);
              continue;
            default:
              continue;
          }
        }
      }
      streamReader.Close();
      return this._Images.Count;
    }

    public string DumpQc(string file)
    {
      FileInfo fileInfo = new FileInfo(file);
      return this.DumpQc(file, fileInfo.Name.Substring(0, fileInfo.Name.Length - fileInfo.Extension.Length));
    }

    public string DumpQc(string file, string spritename)
    {
      FileInfo fileInfo = new FileInfo(file);
      StreamWriter streamWriter = new StreamWriter(fileInfo.FullName);
      streamWriter.WriteLine("$spritename {0}", (object) spritename);
      streamWriter.WriteLine("$texture {0}", (object) this._Texture.ToString());
      streamWriter.WriteLine("$type {0}", (object) this._Type.ToString());
      streamWriter.WriteLine();
      foreach (string image in this._Images)
        streamWriter.WriteLine("$load {0}", (object) image);
      streamWriter.Flush();
      streamWriter.Close();
      return fileInfo.DirectoryName + "\\" + spritename + ".qc";
    }

    private static byte[] TrimImage(string image)
    {
      byte[] bytes = Encoding.ASCII.GetBytes(image);
      byte[] numArray = new byte[68];
      if (bytes.Length < numArray.Length - 1)
      {
        bytes.CopyTo((Array) numArray, 0);
      }
      else
      {
        for (int index = 0; index < numArray.Length - 1; ++index)
          numArray[index] = bytes[index];
      }
      return numArray;
    }

    private static string[] Split(string line)
    {
      string[] strArray1 = new string[2];
      string[] strArray2 = line.Split(' ');
      if (strArray2.Length > 2)
      {
        strArray1[0] = strArray2[0];
        strArray1[1] = strArray2[1];
        for (int index = 2; index < strArray2.Length; ++index)
        {
          ref string local = ref strArray1[2];
          local = local + " " + strArray2[index];
        }
      }
      else if (strArray2.Length == 2)
      {
        strArray1[0] = strArray2[0];
        strArray1[1] = strArray2[1];
      }
      else
        strArray1 = new string[1]{ "NULL" };
      return strArray1;
    }
  }
}
