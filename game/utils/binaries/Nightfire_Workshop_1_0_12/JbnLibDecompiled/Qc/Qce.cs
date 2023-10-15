// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.Qce
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using JbnLib.Shared;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Threading;

namespace JbnLib.Qc
{
  public class Qce
  {
    private FileTokenizer _Tokenizer;
    private CommandQceVersion _QceVersion;
    private List<CommandReplaceActivity> _ReplaceActivityCollection;
    private List<CommandLodTemp> _LodTempCollection;
    private CommandLodFlags _LodFlags;

    public CommandQceVersion QceVersion
    {
      get => this._QceVersion;
      set => this._QceVersion = value;
    }

    public List<CommandReplaceActivity> ReplaceActivityCollection
    {
      get => this._ReplaceActivityCollection;
      set => this._ReplaceActivityCollection = value;
    }

    public List<CommandLodTemp> LodTempCollection
    {
      get => this._LodTempCollection;
      set => this._LodTempCollection = value;
    }

    public CommandLodFlags LodFlags
    {
      get => this._LodFlags;
      set => this._LodFlags = value;
    }

    public Qce()
    {
      Thread.CurrentThread.CurrentUICulture = new CultureInfo("en");
      Thread.CurrentThread.CurrentCulture = new CultureInfo("en-US");
      this.Clear();
    }

    public void Clear()
    {
      this._QceVersion = (CommandQceVersion) null;
      this._ReplaceActivityCollection = new List<CommandReplaceActivity>();
      this._LodTempCollection = new List<CommandLodTemp>();
      this._LodFlags = new CommandLodFlags();
    }

    public void Read(string file)
    {
      this.Clear();
      this._Tokenizer = new FileTokenizer(file);
      try
      {
        while (!this._Tokenizer.GetToken())
        {
          switch (this._Tokenizer.Token)
          {
            case "$qceversion":
              this._Tokenizer.GetToken();
              this._QceVersion = new CommandQceVersion(this._Tokenizer.Token);
              continue;
            case "$replaceactivity":
              this._Tokenizer.GetToken();
              CommandReplaceActivity commandReplaceActivity = new CommandReplaceActivity();
              commandReplaceActivity.SequenceName = this._Tokenizer.Token;
              this._Tokenizer.GetToken();
              commandReplaceActivity.Activity = this._Tokenizer.Token;
              this._ReplaceActivityCollection.Add(commandReplaceActivity);
              continue;
            case "$lodtemp":
              this._Tokenizer.GetToken();
              CommandLodTemp commandLodTemp = new CommandLodTemp();
              commandLodTemp.Index = Convert.ToInt32(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              byte num = Convert.ToByte(this._Tokenizer.Token);
              for (byte index = 0; (int) index < (int) num; ++index)
              {
                this._Tokenizer.GetToken();
                commandLodTemp.Distances.Add(Convert.ToInt32(this._Tokenizer.Token));
              }
              this._LodTempCollection.Add(commandLodTemp);
              continue;
            case "$lodflags":
              this._Tokenizer.GetToken();
              this._LodFlags = new CommandLodFlags(Convert.ToInt32(this._Tokenizer.Token));
              continue;
            default:
              continue;
          }
        }
      }
      catch (Exception ex)
      {
        Messages.ThrowException("Qc.Qce.Read(string)", Messages.LINE + (object) this._Tokenizer.Line + ", " + Messages.TOKEN + this._Tokenizer.Token + "\n" + ex.Message);
      }
    }

    public void Write(string file)
    {
      StreamWriter streamWriter = new StreamWriter(file);
      if (this._QceVersion == null)
        Messages.ThrowException("Qc.Qce.Write(string)", Messages.QCEVERSION_NULL);
      streamWriter.WriteLine(this._QceVersion.ToString());
      streamWriter.WriteLine();
      streamWriter.Flush();
      foreach (CommandReplaceActivity replaceActivity in this._ReplaceActivityCollection)
        streamWriter.WriteLine(replaceActivity.ToString());
      streamWriter.WriteLine();
      streamWriter.Flush();
      if (this._LodFlags.Value != 0)
        streamWriter.WriteLine(this._LodFlags.ToString());
      foreach (CommandLodTemp lodTemp in this._LodTempCollection)
        streamWriter.WriteLine(lodTemp.ToString());
      streamWriter.WriteLine();
      streamWriter.Flush();
      streamWriter.Close();
    }
  }
}
