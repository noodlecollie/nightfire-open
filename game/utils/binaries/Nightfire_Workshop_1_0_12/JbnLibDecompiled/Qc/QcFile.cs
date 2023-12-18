// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.QcFile
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using JbnLib.Shared;
using System;
using System.Collections.Generic;
using System.IO;

namespace JbnLib.Qc
{
  public class QcFile
  {
    internal List<CommandAttachment> _AttachmentCollection;
    internal CommandBBox _BBox;
    internal List<CommandBody> _BodyCollection;
    internal List<CommandBodyGroup> _BodyGroupCollection;
    internal CommandCBox _CBox;
    internal CommandCd _Cd;
    internal CommandCdTexture _CdTexture;
    internal List<CommandController> _ControllerCollection;
    internal CommandEyePosition _EyePosition;
    internal CommandFlags _Flags;
    internal CommandGamma _Gamma;
    internal List<CommandHBox> _HBoxCollection;
    internal CommandMirrorBone _MirrorBone;
    internal CommandModelName _ModelName;
    internal CommandOrigin _Origin;
    internal CommandPivot _Pivot;
    internal CommandRenameBone _RenameBone;
    internal CommandRoot _Root;
    internal CommandScale _Scale;
    internal CommandTextureGroup _TextureGroup;
    internal FileTokenizer _Tokenizer;
    internal List<OmittedCommand> _OmittedCommands;

    public List<CommandAttachment> AttachmentCollection
    {
      get => this._AttachmentCollection;
      set => this._AttachmentCollection = value;
    }

    public CommandBBox BBox
    {
      get => this._BBox;
      set => this._BBox = value;
    }

    public List<CommandBody> BodyCollection
    {
      get => this._BodyCollection;
      set => this._BodyCollection = value;
    }

    public List<CommandBodyGroup> BodyGroupCollection
    {
      get => this._BodyGroupCollection;
      set => this._BodyGroupCollection = value;
    }

    public CommandCBox CBox
    {
      get => this._CBox;
      set => this._CBox = value;
    }

    public CommandCd Cd
    {
      get => this._Cd;
      set => this._Cd = value;
    }

    public CommandCdTexture CdTexture
    {
      get => this._CdTexture;
      set => this._CdTexture = value;
    }

    public List<CommandController> ControllerCollection
    {
      get => this._ControllerCollection;
      set => this._ControllerCollection = value;
    }

    public CommandEyePosition EyePosition
    {
      get => this._EyePosition;
      set => this._EyePosition = value;
    }

    public CommandFlags Flags
    {
      get => this._Flags;
      set => this._Flags = value;
    }

    public CommandGamma Gamma
    {
      get => this._Gamma;
      set => this._Gamma = value;
    }

    public List<CommandHBox> HBoxCollection
    {
      get => this._HBoxCollection;
      set => this._HBoxCollection = value;
    }

    public CommandMirrorBone MirrorBone
    {
      get => this._MirrorBone;
      set => this._MirrorBone = value;
    }

    public CommandModelName ModelName
    {
      get => this._ModelName;
      set => this._ModelName = value;
    }

    public CommandOrigin Origin
    {
      get => this._Origin;
      set => this._Origin = value;
    }

    public CommandPivot Pivot
    {
      get => this._Pivot;
      set => this._Pivot = value;
    }

    public CommandRenameBone RenameBone
    {
      get => this._RenameBone;
      set => this._RenameBone = value;
    }

    public CommandRoot Root
    {
      get => this._Root;
      set => this._Root = value;
    }

    public CommandScale Scale
    {
      get => this._Scale;
      set => this._Scale = value;
    }

    public CommandTextureGroup TextureGroup
    {
      get => this._TextureGroup;
      set => this._TextureGroup = value;
    }

    public List<OmittedCommand> OmittedCommands
    {
      get => this._OmittedCommands;
      set => this._OmittedCommands = value;
    }

    public static string GetModelName(string file)
    {
      FileTokenizer fileTokenizer = new FileTokenizer(file);
      while (!fileTokenizer.GetToken())
      {
        if (fileTokenizer.Token == "$modelname")
        {
          fileTokenizer.GetToken();
          return fileTokenizer.Token;
        }
      }
      return "";
    }

    public static void FixModelName(string file)
    {
      FileInfo fileInfo = new FileInfo(file);
      if (fileInfo.Exists)
      {
        if (File.Exists(Environment.GetEnvironmentVariable("TEMP") + "\\" + fileInfo.Name + ".bak"))
          File.Delete(Environment.GetEnvironmentVariable("TEMP") + "\\" + fileInfo.Name + ".bak");
        File.Move(fileInfo.FullName, Environment.GetEnvironmentVariable("TEMP") + "\\" + fileInfo.Name + ".bak");
      }
      else
        Messages.ThrowException("Qc.QcFile.FixModelName(string)", Messages.FILE_NOT_FOUND + file);
      StreamReader streamReader = new StreamReader(Environment.GetEnvironmentVariable("TEMP") + "\\" + fileInfo.Name + ".bak");
      StreamWriter streamWriter = new StreamWriter(fileInfo.FullName);
      streamWriter.AutoFlush = true;
      while (streamReader.Peek() != -1)
      {
        string str1 = streamReader.ReadLine();
        if (str1.StartsWith("$modelname"))
        {
          if (str1.IndexOf('"') == -1)
          {
            string str2 = str1.Replace("$modelname ", "");
            streamWriter.WriteLine("$modelname \"" + str2 + "\"");
          }
          else
            streamWriter.WriteLine(str1);
        }
        else
          streamWriter.WriteLine(str1);
      }
      streamReader.Close();
      streamWriter.Flush();
      streamWriter.Close();
      File.Delete(Environment.GetEnvironmentVariable("TEMP") + "\\" + fileInfo.Name + ".bak");
    }
  }
}
