// Decompiled with JetBrains decompiler
// Type: JbnLib.Shared.MemoryTokenizer
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;

namespace JbnLib.Shared
{
  public class MemoryTokenizer
  {
    private char[] _Buffer;
    private int _BufferOffset;
    private string _Token;
    private long _Line = 1;

    public string Token => this._Token;

    public long Line => this._Line;

    public MemoryTokenizer(char[] buffer) => this._Buffer = buffer;

    public bool GetToken()
    {
      this._Token = "";
      bool flag1 = false;
      bool flag2 = false;
      bool flag3 = false;
      bool flag4 = false;
      while (!flag2)
      {
        for (int bufferOffset = this._BufferOffset; bufferOffset < this._Buffer.Length; ++bufferOffset)
        {
          if (flag3)
          {
            if (this._Buffer[bufferOffset] == '\n')
            {
              flag3 = false;
              ++this._Line;
            }
          }
          else if (flag4)
          {
            if (this._Buffer[bufferOffset] == '*' && this._Buffer[bufferOffset + 1] == '/')
            {
              flag4 = false;
              ++this._BufferOffset;
            }
            if (this._Buffer[bufferOffset] == '\n')
              ++this._Line;
          }
          else
          {
            switch (this._Buffer[bufferOffset])
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
                if (this._Buffer[bufferOffset + 1] == '/')
                {
                  if (this._Token.Length > 0)
                    flag2 = true;
                  flag3 = true;
                  break;
                }
                if (this._Buffer[bufferOffset + 1] == '*')
                {
                  flag4 = true;
                  break;
                }
                this._Token += "/";
                break;
              default:
                this._Token += this._Buffer[bufferOffset].ToString();
                break;
            }
          }
          if (!flag2)
            ++this._BufferOffset;
          else
            break;
        }
        if (this._BufferOffset == this._Buffer.Length)
        {
          if (this._Token.Length <= 0)
            return true;
          flag2 = true;
        }
      }
      return false;
    }
  }
}
