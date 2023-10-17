﻿// Decompiled with JetBrains decompiler
// Type: JbnLib.Multi.ConversionCompletedEventArgs
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Multi
{
  public class ConversionCompletedEventArgs
  {
    private int _Id;
    private TimeSpan _Duration;
    private string _File;

    public int Id => this._Id;

    public TimeSpan Duration => this._Duration;

    public string File => this._File;

    public ConversionCompletedEventArgs(int id, string file, TimeSpan duration)
    {
      this._Id = id;
      this._Duration = duration;
      this._File = file;
    }
  }
}