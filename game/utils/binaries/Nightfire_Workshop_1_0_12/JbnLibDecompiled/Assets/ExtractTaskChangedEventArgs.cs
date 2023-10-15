// Decompiled with JetBrains decompiler
// Type: JbnLib.Assets.ExtractTaskChangedEventArgs
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using System;

namespace JbnLib.Assets
{
  public class ExtractTaskChangedEventArgs : EventArgs
  {
    private ExtractTask _ExtractTask = ExtractTask.NotSpecified;

    public ExtractTask ExtractTask => this._ExtractTask;

    public ExtractTaskChangedEventArgs(ExtractTask task) => this._ExtractTask = task;
  }
}
