// Decompiled with JetBrains decompiler
// Type: JbnLib.Assets.Archive
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Text;

namespace JbnLib.Assets
{
  public class Archive
  {
    private int _NumFilesDone;
    private int _NumFiles;
    private int _NumDirsDone;
    private int _NumDirs;
    private string _TempDir = "";
    private string _Root;

    [DllImport("zlib")]
    private static extern int compress(
      byte[] dest,
      ref uint destlen,
      byte[] source,
      uint sourcelen);

    public event ArchiveTaskChangedHandler ArchiveTaskChanged;

    public event FileProgressChangedHandler FileProgressChanged;

    public string Root => this._Root;

    public int Directories => this._NumDirs;

    public int Files => this._NumFiles;

    public Archive(string root) => this._Root = root;

    public void ArchiveAll(string savepath)
    {
      this._NumFilesDone = 0;
      this._NumFiles = 0;
      this._NumDirsDone = 0;
      this._NumDirs = 0;
      ArchiveTaskChangedHandler archiveTaskChanged1 = this.ArchiveTaskChanged;
      if (archiveTaskChanged1 != null)
        archiveTaskChanged1((object) this, new ArchiveTaskChangedEventArgs(ArchiveTask.VerifyPath));
      DirectoryInfo directoryInfo = new DirectoryInfo(this._Root);
      if (directoryInfo.Name != "ROOT")
      {
        if (!(directoryInfo.Name.ToUpper() == "ROOT"))
          throw new Exception("Error: The variable 'root' must point to a folder named 'ROOT' (case-sensitive).");
        Random random = new Random();
        string str;
        do
        {
          str = directoryInfo.Parent.FullName + "\\temp_" + (object) random.Next();
        }
        while (Directory.Exists(str));
        Directory.Move(directoryInfo.FullName, str);
        this._Root = directoryInfo.Parent.FullName + "\\ROOT";
        Directory.Move(str, this._Root);
        directoryInfo = new DirectoryInfo(this._Root);
      }
      ArchiveTaskChangedHandler archiveTaskChanged2 = this.ArchiveTaskChanged;
      if (archiveTaskChanged2 != null)
        archiveTaskChanged2((object) this, new ArchiveTaskChangedEventArgs(ArchiveTask.CreateTempDir));
      this._TempDir = Directory.CreateDirectory(Environment.GetEnvironmentVariable("TEMP") + "\\Assets007").FullName;
      ArchiveTaskChangedHandler archiveTaskChanged3 = this.ArchiveTaskChanged;
      if (archiveTaskChanged3 != null)
        archiveTaskChanged3((object) this, new ArchiveTaskChangedEventArgs(ArchiveTask.CountDirsFiles));
      this.ScanDirectory(directoryInfo.FullName);
      ArchiveTaskChangedHandler archiveTaskChanged4 = this.ArchiveTaskChanged;
      if (archiveTaskChanged4 != null)
        archiveTaskChanged4((object) this, new ArchiveTaskChangedEventArgs(ArchiveTask.WriteID));
      FileInfo fileInfo1 = new FileInfo(savepath);
      Directory.CreateDirectory(fileInfo1.DirectoryName);
      FileStream fileStream = fileInfo1.Open(FileMode.Create);
      BinaryWriter bw = new BinaryWriter((Stream) fileStream);
      bw.Write(1);
      bw.Write(3);
      ArchiveTaskChangedHandler archiveTaskChanged5 = this.ArchiveTaskChanged;
      if (archiveTaskChanged5 != null)
        archiveTaskChanged5((object) this, new ArchiveTaskChangedEventArgs(ArchiveTask.WriteHeaderPrepFiles));
      this.WriteDirectory(directoryInfo.FullName, ref bw);
      long position1 = fileStream.Position;
      ArchiveTaskChangedHandler archiveTaskChanged6 = this.ArchiveTaskChanged;
      if (archiveTaskChanged6 != null)
        archiveTaskChanged6((object) this, new ArchiveTaskChangedEventArgs(ArchiveTask.WriteFiles));
      for (int index = 0; index < this._NumFiles; ++index)
      {
        FileInfo fileInfo2 = new FileInfo(this._TempDir + "\\" + (object) index);
        FileStream input = fileInfo2.OpenRead();
        BinaryReader binaryReader = new BinaryReader((Stream) input);
        bw.Write(binaryReader.ReadBytes((int) fileInfo2.Length));
        bw.Flush();
        binaryReader.Close();
        input.Close();
        fileInfo2.Delete();
      }
      ArchiveTaskChangedHandler archiveTaskChanged7 = this.ArchiveTaskChanged;
      if (archiveTaskChanged7 != null)
        archiveTaskChanged7((object) this, new ArchiveTaskChangedEventArgs(ArchiveTask.WriteHash));
      long position2 = fileStream.Position;
      fileStream.Position = 8L;
      BinaryReader binaryReader1 = new BinaryReader((Stream) fileStream);
      byte[] numArray = binaryReader1.ReadBytes(Convert.ToInt32(position1 - 8L));
      fileStream.Position = position2;
      bw.Write(16);
      bw.Write(this.Md5Hash(numArray));
      bw.Flush();
      binaryReader1.Close();
      bw.Close();
      fileStream.Close();
      ArchiveTaskChangedHandler archiveTaskChanged8 = this.ArchiveTaskChanged;
      if (archiveTaskChanged8 != null)
        archiveTaskChanged8((object) this, new ArchiveTaskChangedEventArgs(ArchiveTask.DeleteTempDir));
      Directory.Delete(this._TempDir, true);
      ArchiveTaskChangedHandler archiveTaskChanged9 = this.ArchiveTaskChanged;
      if (archiveTaskChanged9 == null)
        return;
      archiveTaskChanged9((object) this, new ArchiveTaskChangedEventArgs(ArchiveTask.Done));
    }

    private void WriteDirectory(string path, ref BinaryWriter bw)
    {
      ++this._NumDirsDone;
      DirectoryInfo directoryInfo = new DirectoryInfo(path);
      FileInfo[] files = directoryInfo.GetFiles();
      DirectoryInfo[] directories = directoryInfo.GetDirectories();
      bw.Write(directoryInfo.Name.Length);
      bw.Write(Encoding.ASCII.GetBytes(directoryInfo.Name));
      bw.Write(directories.Length);
      long position1 = bw.BaseStream.Position;
      int num = 0;
      bw.BaseStream.Position += 4L;
      foreach (FileInfo file in files)
      {
        bw.Write(file.Name.Length);
        bw.Write(Encoding.ASCII.GetBytes(file.Name));
        if (this.Compress(file))
        {
          bw.Write(true);
          bw.Write((int) file.Length);
          FileStream input = new FileStream(file.FullName, FileMode.Open);
          BinaryReader binaryReader = new BinaryReader((Stream) input);
          byte[] numArray = new byte[file.Length];
          uint length = (uint) file.Length;
          Archive.compress(numArray, ref length, binaryReader.ReadBytes((int) file.Length), (uint) file.Length);
          binaryReader.Close();
          input.Close();
          bw.Write(length);
          FileStream output = new FileStream(this._TempDir + "\\" + (object) this._NumFilesDone, FileMode.Create);
          BinaryWriter binaryWriter = new BinaryWriter((Stream) output);
          binaryWriter.Write(numArray, 0, (int) length);
          binaryWriter.Flush();
          binaryWriter.Close();
          output.Close();
        }
        else
        {
          bw.Write(false);
          bw.Write((int) file.Length);
          bw.Write((int) file.Length);
          File.Copy(file.FullName, this._TempDir + "\\" + (object) this._NumFilesDone, true);
        }
        ++this._NumFilesDone;
        num += file.Name.Length + 1;
        bw.Flush();
        FileProgressChangedHandler fileProgressChanged = this.FileProgressChanged;
        if (fileProgressChanged != null)
          fileProgressChanged((object) this, new FileProgressChangedEventArgs(this._NumFilesDone, this._NumFiles));
      }
      bw.Write(0);
      long position2 = bw.BaseStream.Position;
      bw.BaseStream.Position = position1;
      bw.Write(num);
      bw.BaseStream.Position = position2;
      bw.Flush();
      foreach (FileSystemInfo fileSystemInfo in directories)
        this.WriteDirectory(fileSystemInfo.FullName, ref bw);
    }

    private void ScanDirectory(string path)
    {
      ++this._NumDirs;
      this._NumFiles += Directory.GetFiles(path).Length;
      foreach (string directory in Directory.GetDirectories(path))
        this.ScanDirectory(directory);
    }

    private bool Compress(FileInfo file)
    {
      if (file.Length <= 16L)
        return false;
      string lower = file.Extension.ToLower();
      return !(lower == ".png") && !(lower == ".plr");
    }

    private byte[] Md5Hash(byte[] value) => new MD5CryptoServiceProvider().ComputeHash(value);
  }
}
