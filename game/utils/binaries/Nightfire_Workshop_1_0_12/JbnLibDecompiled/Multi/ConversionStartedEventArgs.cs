﻿// Decompiled with JetBrains decompiler
// Type: JbnLib.Multi.ConversionStartedEventArgs
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

namespace JbnLib.Multi
{
  public class ConversionStartedEventArgs
  {
    private int _Id;

    public int Id => this._Id;

    public ConversionStartedEventArgs(int id) => this._Id = id;
  }
}