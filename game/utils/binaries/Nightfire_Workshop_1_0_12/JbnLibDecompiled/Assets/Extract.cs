// Decompiled with JetBrains decompiler
// Type: JbnLib.Assets.Extract
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

namespace JbnLib.Assets
{
  public class Extract
  {
    private List<DirNode> _Directories = new List<DirNode>();
    private List<FileNode> _Files = new List<FileNode>();
    private int _NumFiles;
    private long _DataOffset;
    private FileStream _Assets;
    private Stack<string> _Path = new Stack<string>();

    [DllImport("zlib")]
    private static extern int uncompress(
      byte[] dest,
      ref uint destlen,
      byte[] source,
      uint sourcelen);

    public event ExtractTaskChangedHandler ExtractTaskChanged;

    public event FileProgressChangedHandler FileProgressChanged;

    public List<DirNode> Directories => this._Directories;

    public List<FileNode> Files => this._Files;

    public Extract(string assets)
    {
      this._Directories = new List<DirNode>();
      this._Files = new List<FileNode>();
      this._Path = new Stack<string>();
      this._NumFiles = 0;
      this._Assets = new FileStream(assets, FileMode.Open);
      BinaryReader br = new BinaryReader((Stream) this._Assets);
      ExtractTaskChangedHandler extractTaskChanged1 = this.ExtractTaskChanged;
      if (extractTaskChanged1 != null)
        extractTaskChanged1((object) this, new ExtractTaskChangedEventArgs(ExtractTask.VerifyingID));
      if (br.ReadInt32() != 1)
        throw new Exception("Error: Incorrect archive ID.");
      if (br.ReadInt32() != 3)
        throw new Exception("Error: Incorrect archive ID.");
      ExtractTaskChangedHandler extractTaskChanged2 = this.ExtractTaskChanged;
      if (extractTaskChanged2 != null)
        extractTaskChanged2((object) this, new ExtractTaskChangedEventArgs(ExtractTask.ReadingTOC));
      this.ReadDirectory(ref br);
      this._Assets.Position += 4L;
      this._DataOffset = this._Assets.Position;
      long dataOffset = this._DataOffset;
      for (int index = 0; index < this._Files.Count; ++index)
      {
        this._Files[index].Offset = dataOffset;
        dataOffset += (long) this._Files[index].CompressedSize;
      }
    }

    public void ExtractAll(string savedir)
    {
      ExtractTaskChangedHandler extractTaskChanged1 = this.ExtractTaskChanged;
      if (extractTaskChanged1 != null)
        extractTaskChanged1((object) this, new ExtractTaskChangedEventArgs(ExtractTask.CreatingDirs));
      foreach (DirNode directory in this._Directories)
        Directory.CreateDirectory(savedir + directory.Destination);
      ExtractTaskChangedHandler extractTaskChanged2 = this.ExtractTaskChanged;
      if (extractTaskChanged2 != null)
        extractTaskChanged2((object) this, new ExtractTaskChangedEventArgs(ExtractTask.ExtractingFiles));
      this._Assets.Position = this._DataOffset;
      BinaryReader binaryReader = new BinaryReader((Stream) this._Assets);
      int done = 0;
      foreach (FileNode file in this._Files)
      {
        uint realSize = file.RealSize;
        byte[] numArray = new byte[(int) realSize];
        if (file.Compressed)
        {
          byte[] source = binaryReader.ReadBytes((int) file.CompressedSize);
          Extract.uncompress(numArray, ref realSize, source, file.CompressedSize);
        }
        else
          numArray = binaryReader.ReadBytes((int) file.RealSize);
        FileStream output = new FileStream(savedir + file.Destination, FileMode.Create);
        BinaryWriter binaryWriter = new BinaryWriter((Stream) output);
        binaryWriter.Write(numArray);
        binaryWriter.Flush();
        binaryWriter.Close();
        output.Close();
        ++done;
        FileProgressChangedHandler fileProgressChanged = this.FileProgressChanged;
        if (fileProgressChanged != null)
          fileProgressChanged((object) this, new FileProgressChangedEventArgs(done, this._Files.Count));
      }
      binaryReader.Close();
      ExtractTaskChangedHandler extractTaskChanged3 = this.ExtractTaskChanged;
      if (extractTaskChanged3 == null)
        return;
      extractTaskChanged3((object) this, new ExtractTaskChangedEventArgs(ExtractTask.Done));
    }

    public void ExtractFile(string relativename, string savepath)
    {
      ExtractTaskChangedHandler extractTaskChanged1 = this.ExtractTaskChanged;
      if (extractTaskChanged1 != null)
        extractTaskChanged1((object) this, new ExtractTaskChangedEventArgs(ExtractTask.CreatingDirs));
      FileInfo fileInfo = new FileInfo(savepath);
      if (!Directory.Exists(fileInfo.DirectoryName))
        Directory.CreateDirectory(fileInfo.DirectoryName);
      ExtractTaskChangedHandler extractTaskChanged2 = this.ExtractTaskChanged;
      if (extractTaskChanged2 != null)
        extractTaskChanged2((object) this, new ExtractTaskChangedEventArgs(ExtractTask.ExtractingFiles));
      this._Assets.Position = this._DataOffset;
      BinaryReader binaryReader = new BinaryReader((Stream) this._Assets);
      for (int index = 0; index < this._Files.Count; ++index)
      {
        if (this._Files[index].Destination == relativename)
        {
          uint realSize = this._Files[index].RealSize;
          byte[] numArray = new byte[(int) realSize];
          this._Assets.Position = this._Files[index].Offset;
          if (this._Files[index].Compressed)
          {
            byte[] source = binaryReader.ReadBytes((int) this._Files[index].CompressedSize);
            Extract.uncompress(numArray, ref realSize, source, this._Files[index].CompressedSize);
          }
          else
            numArray = binaryReader.ReadBytes((int) this._Files[index].RealSize);
          FileStream output = new FileStream(savepath, FileMode.Create);
          BinaryWriter binaryWriter = new BinaryWriter((Stream) output);
          binaryWriter.Write(numArray);
          binaryWriter.Flush();
          binaryWriter.Close();
          output.Close();
        }
        FileProgressChangedHandler fileProgressChanged = this.FileProgressChanged;
        if (fileProgressChanged != null)
          fileProgressChanged((object) this, new FileProgressChangedEventArgs(index, this._Files.Count));
      }
    }

    public void ExtractDir(string relativename, string savedir)
    {
      ExtractTaskChangedHandler extractTaskChanged1 = this.ExtractTaskChanged;
      if (extractTaskChanged1 != null)
        extractTaskChanged1((object) this, new ExtractTaskChangedEventArgs(ExtractTask.CreatingDirs));
      foreach (DirNode directory in this._Directories)
      {
        if (directory.Destination.StartsWith(relativename))
          Directory.CreateDirectory(savedir + directory.Destination.Replace(relativename, ""));
      }
      ExtractTaskChangedHandler extractTaskChanged2 = this.ExtractTaskChanged;
      if (extractTaskChanged2 != null)
        extractTaskChanged2((object) this, new ExtractTaskChangedEventArgs(ExtractTask.ExtractingFiles));
      this._Assets.Position = this._DataOffset;
      BinaryReader binaryReader = new BinaryReader((Stream) this._Assets);
      for (int index = 0; index < this._Files.Count; ++index)
      {
        if (this._Files[index].Destination.StartsWith(relativename))
        {
          uint realSize = this._Files[index].RealSize;
          byte[] numArray = new byte[(int) realSize];
          this._Assets.Position = this._Files[index].Offset;
          if (this._Files[index].Compressed)
          {
            byte[] source = binaryReader.ReadBytes((int) this._Files[index].CompressedSize);
            Extract.uncompress(numArray, ref realSize, source, this._Files[index].CompressedSize);
          }
          else
            numArray = binaryReader.ReadBytes((int) this._Files[index].RealSize);
          FileStream output = new FileStream(savedir + this._Files[index].Destination.Replace(relativename, ""), FileMode.Create);
          BinaryWriter binaryWriter = new BinaryWriter((Stream) output);
          binaryWriter.Write(numArray);
          binaryWriter.Flush();
          binaryWriter.Close();
          output.Close();
        }
        FileProgressChangedHandler fileProgressChanged = this.FileProgressChanged;
        if (fileProgressChanged != null)
          fileProgressChanged((object) this, new FileProgressChangedEventArgs(index, this._Files.Count));
      }
    }

    public byte[] GetFile(string relativename)
    {
      this._Assets.Position = this._DataOffset;
      BinaryReader binaryReader = new BinaryReader((Stream) this._Assets);
      for (int index = 0; index < this._Files.Count; ++index)
      {
        if (this._Files[index].Destination == relativename)
        {
          uint realSize = this._Files[index].RealSize;
          byte[] dest = new byte[(int) realSize];
          this._Assets.Position = this._Files[index].Offset;
          if (this._Files[index].Compressed)
          {
            byte[] source = binaryReader.ReadBytes((int) this._Files[index].CompressedSize);
            Extract.uncompress(dest, ref realSize, source, this._Files[index].CompressedSize);
          }
          else
            dest = binaryReader.ReadBytes((int) this._Files[index].RealSize);
          return dest;
        }
      }
      throw new Exception("Error: File not found in the archive:" + relativename);
    }

    private void ReadDirectory(ref BinaryReader br)
    {
      DirNode dirNode = new DirNode()
      {
        DirNameLength = br.ReadInt32()
      };
      dirNode.DirName = Encoding.ASCII.GetString(br.ReadBytes(dirNode.DirNameLength));
      this._Path.Push(dirNode.DirName);
      dirNode.NumSubDirs = br.ReadInt32();
      dirNode.FileNamesLength = br.ReadInt32();
      dirNode.Destination = this.PathsToString();
      this._Directories.Add(dirNode);
      int num1 = 0;
      int num2 = 0;
      int num3 = 0;
      while (num1 < dirNode.NumSubDirs || num3 < dirNode.FileNamesLength)
      {
        int num4 = br.ReadInt32();
        if (num4 != 0)
        {
          ++this._NumFiles;
          ++dirNode.NumFiles;
          ++num2;
          if (dirNode.NumFiles == 1)
            num3 = 0;
          FileNode fileNode = new FileNode()
          {
            FileNameLength = num4
          };
          fileNode.FileName = Encoding.ASCII.GetString(br.ReadBytes(fileNode.FileNameLength));
          fileNode.Compressed = br.ReadBoolean();
          fileNode.RealSize = br.ReadUInt32();
          fileNode.CompressedSize = br.ReadUInt32();
          fileNode.Destination = this.PathsToString() + "\\" + fileNode.FileName;
          this._Files.Add(fileNode);
          num3 += fileNode.FileNameLength + 1;
        }
        else if (num1 < dirNode.NumSubDirs)
        {
          this.ReadDirectory(ref br);
          ++num1;
        }
      }
      this._Path.Pop();
    }

    private string PathsToString()
    {
      string str1 = "";
      string[] array = this._Path.ToArray();
      Array.Reverse((Array) array);
      foreach (string str2 in array)
        str1 = str1 + "\\" + str2;
      return str1;
    }
  }
}
