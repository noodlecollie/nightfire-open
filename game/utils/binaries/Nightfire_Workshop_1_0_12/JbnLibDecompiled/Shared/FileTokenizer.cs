// Decompiled with JetBrains decompiler
// Type: JbnLib.Shared.FileTokenizer
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using System.IO;

namespace JbnLib.Shared
{
  public class FileTokenizer
  {
    private long _Offset;
    private long _Length;
    private string _File;
    private char[] _Buffer = new char[4096];
    private short _BufferOffset;
    private short _BufferAvailable;
    private string _Token;
    private long _Line = 1;

    public string Token => this._Token;

    public long Line => this._Line;

    public FileTokenizer(string path)
    {
      if (File.Exists(path))
      {
        this._Offset = 0L;
        FileInfo fileInfo = new FileInfo(path);
        this._Length = (long) (int) fileInfo.Length;
        this._File = fileInfo.FullName;
      }
      else
        Messages.ThrowException("Shared.FileTokenizer.New(string)", Messages.FILE_NOT_FOUND + path);
    }

    public bool GetToken()
    {
      this._Token = "";
      bool flag1 = false;
      bool flag2 = false;
      bool flag3 = false;
      bool flag4 = false;
      while (!flag2)
      {
        if ((int) this._BufferAvailable == (int) this._BufferOffset && this._Offset < this._Length)
          this.Buffer();
        for (short index = this._BufferOffset; (int) index < (int) this._BufferAvailable; ++index)
        {
          if (flag3)
          {
            if (this._Buffer[(int) index] == '\n')
            {
              flag3 = false;
              ++this._Line;
            }
          }
          else if (flag4)
          {
            if (this._Buffer[(int) index] == '*' && this._Buffer[(int) index + 1] == '/')
            {
              flag4 = false;
              ++this._BufferOffset;
            }
            if (this._Buffer[(int) index] == '\n')
              ++this._Line;
          }
          else
          {
            switch (this._Buffer[(int) index])
            {
              case char.MinValue:
              case '\t':
              case '\r':
                break;
              case '\n':
                if (flag1)
                {
                  Messages.ThrowException("Shared.FileTokenizer.GetToken()", Messages.QUOTE_MISSING + Messages.LINE + (object) this._Line);
                  break;
                }
                if (this._Token.Length > 0)
                {
                  flag2 = true;
                  break;
                }
                ++this._Line;
                break;
              case ' ':
                if (flag1)
                {
                  this._Token += " ";
                  break;
                }
                if (this._Token.Length > 0)
                {
                  flag2 = true;
                  break;
                }
                break;
              case '"':
                if (flag1)
                {
                  ++this._BufferOffset;
                  flag2 = true;
                  break;
                }
                flag1 = true;
                break;
              case '/':
                if (flag1)
                {
                  this._Token += "/";
                  break;
                }
                if ((int) index + 1 >= (int) this._BufferAvailable)
                {
                  this.Buffer();
                  index = (short) -1;
                }
                if (this._Buffer[(int) index + 1] == '/')
                {
                  if (this._Token.Length > 0)
                    flag2 = true;
                  flag3 = true;
                  break;
                }
                if (this._Buffer[(int) index + 1] == '*')
                {
                  flag4 = true;
                  break;
                }
                this._Token += "/";
                break;
              default:
                this._Token += this._Buffer[(int) index].ToString();
                break;
            }
          }
          if (!flag2)
            ++this._BufferOffset;
          else
            break;
        }
        if (this._Offset == this._Length && (int) this._BufferOffset >= (int) this._BufferAvailable)
        {
          if (this._Token.Length <= 0)
            return true;
          flag2 = true;
        }
      }
      return false;
    }

    private void Buffer()
    {
      StreamReader streamReader = new StreamReader(this._File);
      streamReader.BaseStream.Position = this._Offset;
      if ((long) this._Buffer.Length >= this.Available())
      {
        this._BufferAvailable = (short) streamReader.ReadBlock(this._Buffer, 0, (int) this.Available());
        this._Offset += this.Available();
      }
      else
      {
        this._BufferAvailable = (short) streamReader.ReadBlock(this._Buffer, 0, this._Buffer.Length);
        this._Offset += (long) this._Buffer.Length;
      }
      this._BufferOffset = (short) 0;
      streamReader.Close();
    }

    public long Available() => this._Length - this._Offset;
  }
}
