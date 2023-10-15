// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.QcFileV10
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using JbnLib.Mdl;
using JbnLib.Shared;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Threading;

namespace JbnLib.Qc
{
  public class QcFileV10 : QcFile
  {
    private List<CommandSequenceV10> _SequenceCollection;
    private CommandClipToTextures _ClipToTextures;
    private CommandExternalTextures _ExternalTextures;

    public List<CommandSequenceV10> SequenceCollection
    {
      get => this._SequenceCollection;
      set => this._SequenceCollection = value;
    }

    public CommandClipToTextures ClipToTextures
    {
      get => this._ClipToTextures;
      set => this._ClipToTextures = value;
    }

    public CommandExternalTextures ExternalTextures
    {
      get => this._ExternalTextures;
      set => this._ExternalTextures = value;
    }

    public QcFileV10()
    {
      Thread.CurrentThread.CurrentUICulture = new CultureInfo("en");
      Thread.CurrentThread.CurrentCulture = new CultureInfo("en-US");
      this.Clear();
    }

    public void Clear()
    {
      this._AttachmentCollection = new List<CommandAttachment>();
      this._BBox = (CommandBBox) null;
      this._BodyCollection = new List<CommandBody>();
      this._BodyGroupCollection = new List<CommandBodyGroup>();
      this._CBox = (CommandCBox) null;
      this._Cd = (CommandCd) null;
      this._CdTexture = (CommandCdTexture) null;
      this._ClipToTextures = (CommandClipToTextures) null;
      this._ControllerCollection = new List<CommandController>();
      this._ExternalTextures = (CommandExternalTextures) null;
      this._EyePosition = (CommandEyePosition) null;
      this._Flags = (CommandFlags) null;
      this._Gamma = (CommandGamma) null;
      this._HBoxCollection = new List<CommandHBox>();
      this._MirrorBone = (CommandMirrorBone) null;
      this._ModelName = (CommandModelName) null;
      this._Origin = (CommandOrigin) null;
      this._Pivot = (CommandPivot) null;
      this._RenameBone = (CommandRenameBone) null;
      this._Root = (CommandRoot) null;
      this._Scale = (CommandScale) null;
      this._SequenceCollection = new List<CommandSequenceV10>();
      this._TextureGroup = (CommandTextureGroup) null;
      this._OmittedCommands = new List<OmittedCommand>();
    }

    public void Read(string file)
    {
      this.Clear();
      FileInfo fileInfo = new FileInfo(file);
      this._Tokenizer = new FileTokenizer(file);
      bool flag1 = false;
      bool flag2 = false;
      while (!flag1)
      {
        try
        {
          if (!flag2)
            flag1 = this._Tokenizer.GetToken();
          flag2 = false;
          switch (this._Tokenizer.Token.ToLower())
          {
            case "$attachment":
              CommandAttachment commandAttachment = new CommandAttachment();
              this._Tokenizer.GetToken();
              commandAttachment.Name = this._Tokenizer.Token;
              this._Tokenizer.GetToken();
              commandAttachment.Bone = this._Tokenizer.Token;
              this._Tokenizer.GetToken();
              commandAttachment.Position.X = Convert.ToSingle(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              commandAttachment.Position.Y = Convert.ToSingle(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              commandAttachment.Position.Z = Convert.ToSingle(this._Tokenizer.Token);
              this._AttachmentCollection.Add(commandAttachment);
              continue;
            case "$bbox":
              this._BBox = new CommandBBox();
              this._Tokenizer.GetToken();
              this._BBox.Min.X = Convert.ToSingle(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              this._BBox.Min.Y = Convert.ToSingle(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              this._BBox.Min.Z = Convert.ToSingle(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              this._BBox.Max.X = Convert.ToSingle(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              this._BBox.Max.Y = Convert.ToSingle(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              this._BBox.Max.Z = Convert.ToSingle(this._Tokenizer.Token);
              continue;
            case "$body":
              CommandBody commandBody = new CommandBody();
              this._Tokenizer.GetToken();
              commandBody.Name = this._Tokenizer.Token;
              this._Tokenizer.GetToken();
              commandBody.File = this._Tokenizer.Token;
              StaticMethods.SmdFile = fileInfo.DirectoryName + "\\" + commandBody.File + ".smd";
              this._BodyCollection.Add(commandBody);
              continue;
            case "$bodygroup":
              CommandBodyGroup commandBodyGroup = new CommandBodyGroup();
              this._Tokenizer.GetToken();
              commandBodyGroup.Name = this._Tokenizer.Token;
              this._Tokenizer.GetToken();
              if (this._Tokenizer.Token == "{")
              {
                while (true)
                {
                  this._Tokenizer.GetToken();
                  if (this._Tokenizer.Token != "}")
                  {
                    if (this._Tokenizer.Token.ToLower() != "blank")
                    {
                      BodyGroupItem bodyGroupItem = new BodyGroupItem();
                      bodyGroupItem.Name = this._Tokenizer.Token;
                      this._Tokenizer.GetToken();
                      bodyGroupItem.File = this._Tokenizer.Token;
                      StaticMethods.SmdFile = fileInfo.DirectoryName + "\\" + bodyGroupItem.File + ".smd";
                      commandBodyGroup.BodyCollection.Add(bodyGroupItem);
                    }
                    else
                      commandBodyGroup.BodyCollection.Add(new BodyGroupItem()
                      {
                        Name = this._Tokenizer.Token
                      });
                  }
                  else
                    break;
                }
              }
              this._BodyGroupCollection.Add(commandBodyGroup);
              continue;
            case "$cbox":
              this._CBox = new CommandCBox();
              this._Tokenizer.GetToken();
              this._CBox.Min.X = Convert.ToSingle(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              this._CBox.Min.Y = Convert.ToSingle(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              this._CBox.Min.Z = Convert.ToSingle(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              this._CBox.Max.X = Convert.ToSingle(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              this._CBox.Max.Y = Convert.ToSingle(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              this._CBox.Max.Z = Convert.ToSingle(this._Tokenizer.Token);
              continue;
            case "$cd":
              this._Tokenizer.GetToken();
              this._Cd = new CommandCd(this._Tokenizer.Token);
              continue;
            case "$cdtexture":
              this._Tokenizer.GetToken();
              this._CdTexture = new CommandCdTexture(this._Tokenizer.Token);
              continue;
            case "$cliptotextures":
              this._ClipToTextures = new CommandClipToTextures();
              continue;
            case "$controller":
              CommandController commandController = new CommandController();
              this._Tokenizer.GetToken();
              commandController.Index = !(this._Tokenizer.Token == "mouth") ? Convert.ToInt32(this._Tokenizer.Token) : 4;
              this._Tokenizer.GetToken();
              commandController.Bone = this._Tokenizer.Token;
              this._Tokenizer.GetToken();
              commandController.Type = Enumerators.ToMotionFlags(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              commandController.Start = (float) Convert.ToInt32(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              commandController.End = (float) Convert.ToInt32(this._Tokenizer.Token);
              this._ControllerCollection.Add(commandController);
              continue;
            case "$externaltextures":
              this._ExternalTextures = new CommandExternalTextures();
              continue;
            case "$eyeposition":
              CommandEyePosition commandEyePosition = new CommandEyePosition();
              this._Tokenizer.GetToken();
              commandEyePosition.Value.X = Convert.ToSingle(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              commandEyePosition.Value.Y = Convert.ToSingle(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              commandEyePosition.Value.Z = Convert.ToSingle(this._Tokenizer.Token);
              this._EyePosition = commandEyePosition;
              continue;
            case "$flags":
              this._Tokenizer.GetToken();
              this._Flags = new CommandFlags((TypeFlag) Convert.ToInt32(this._Tokenizer.Token));
              continue;
            case "$gamma":
              this._Tokenizer.GetToken();
              this._Gamma = new CommandGamma(Convert.ToSingle(this._Tokenizer.Token));
              continue;
            case "$hbox":
              CommandHBox commandHbox = new CommandHBox();
              this._Tokenizer.GetToken();
              commandHbox.Group = Convert.ToInt32(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              commandHbox.Bone = this._Tokenizer.Token;
              this._Tokenizer.GetToken();
              commandHbox.Min.X = Convert.ToSingle(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              commandHbox.Min.Y = Convert.ToSingle(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              commandHbox.Min.Z = Convert.ToSingle(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              commandHbox.Max.X = Convert.ToSingle(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              commandHbox.Max.Y = Convert.ToSingle(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              commandHbox.Max.Z = Convert.ToSingle(this._Tokenizer.Token);
              this._HBoxCollection.Add(commandHbox);
              continue;
            case "$mirrorbone":
              this._Tokenizer.GetToken();
              this._MirrorBone = new CommandMirrorBone(this._Tokenizer.Token);
              continue;
            case "$modelname":
              this._Tokenizer.GetToken();
              this._ModelName = new CommandModelName(this._Tokenizer.Token);
              continue;
            case "$origin":
              CommandOrigin commandOrigin = new CommandOrigin();
              this._Tokenizer.GetToken();
              commandOrigin.Value.X = Convert.ToSingle(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              commandOrigin.Value.Y = Convert.ToSingle(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              commandOrigin.Value.Z = Convert.ToSingle(this._Tokenizer.Token);
              this._Origin = commandOrigin;
              continue;
            case "$pivot":
              CommandPivot commandPivot = new CommandPivot();
              this._Tokenizer.GetToken();
              commandPivot.Index = Convert.ToInt32(this._Tokenizer.Token);
              this._Tokenizer.GetToken();
              commandPivot.Bone = this._Tokenizer.Token;
              this._Pivot = commandPivot;
              continue;
            case "$renamebone":
              CommandRenameBone commandRenameBone = new CommandRenameBone();
              this._Tokenizer.GetToken();
              commandRenameBone.OldName = this._Tokenizer.Token;
              this._Tokenizer.GetToken();
              commandRenameBone.NewName = this._Tokenizer.Token;
              continue;
            case "$root":
              this._Tokenizer.GetToken();
              this._Root = new CommandRoot(this._Tokenizer.Token);
              continue;
            case "$scale":
              this._Tokenizer.GetToken();
              this._Scale = new CommandScale(Convert.ToSingle(this._Tokenizer.Token));
              continue;
            case "$sequence":
              CommandSequenceV10 commandSequenceV10 = new CommandSequenceV10();
              this._Tokenizer.GetToken();
              commandSequenceV10.Name = this._Tokenizer.Token;
              int num1 = 0;
              while (!this._Tokenizer.GetToken())
              {
                if (this._Tokenizer.Token.StartsWith("$"))
                {
                  flag2 = true;
                  break;
                }
                switch (this._Tokenizer.Token.ToLower())
                {
                  case "animation":
                    this._Tokenizer.GetToken();
                    commandSequenceV10.Animation = new OptionAnimation(this._Tokenizer.Token);
                    continue;
                  case "blend":
                    OptionBlend optionBlend = new OptionBlend();
                    this._Tokenizer.GetToken();
                    optionBlend.Type = Enumerators.ToMotionFlags(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionBlend.Start = Convert.ToSingle(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionBlend.End = Convert.ToSingle(this._Tokenizer.Token);
                    commandSequenceV10.BlendCollection.Add(optionBlend);
                    continue;
                  case "event":
                    OptionEvent optionEvent = new OptionEvent();
                    this._Tokenizer.GetToken();
                    optionEvent.EventValue = Convert.ToInt32(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionEvent.Frame = Convert.ToInt32(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionEvent.Options = this._Tokenizer.Token;
                    commandSequenceV10.EventCollection.Add(optionEvent);
                    continue;
                  case "fps":
                    this._Tokenizer.GetToken();
                    commandSequenceV10.Fps = new OptionFps(Convert.ToSingle(this._Tokenizer.Token));
                    continue;
                  case "frame":
                    OptionFrame optionFrame = new OptionFrame();
                    this._Tokenizer.GetToken();
                    optionFrame.Start = Convert.ToInt32(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionFrame.End = Convert.ToInt32(this._Tokenizer.Token);
                    commandSequenceV10.Frame = optionFrame;
                    continue;
                  case "loop":
                    commandSequenceV10.Loop = new OptionLoop(true);
                    continue;
                  case "node":
                    this._Tokenizer.GetToken();
                    commandSequenceV10.Node = new OptionNodeV10(Convert.ToSByte(this._Tokenizer.Token));
                    continue;
                  case "pivot":
                    OptionPivot optionPivot = new OptionPivot();
                    this._Tokenizer.GetToken();
                    optionPivot.Index = Convert.ToInt32(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionPivot.Start = (float) Convert.ToInt32(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionPivot.End = (float) Convert.ToInt32(this._Tokenizer.Token);
                    commandSequenceV10.PivotCollection.Add(optionPivot);
                    continue;
                  case "rotate":
                    this._Tokenizer.GetToken();
                    commandSequenceV10.Rotate = new OptionRotate(Convert.ToInt32(this._Tokenizer.Token));
                    continue;
                  case "rtransition":
                    OptionRTransition optionRtransition = new OptionRTransition();
                    this._Tokenizer.GetToken();
                    optionRtransition.EntryBone = Convert.ToInt32(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionRtransition.ExitBone = Convert.ToInt32(this._Tokenizer.Token);
                    commandSequenceV10.RTransition = optionRtransition;
                    continue;
                  case "scale":
                    this._Tokenizer.GetToken();
                    commandSequenceV10.Scale = new OptionScale(Convert.ToSingle(this._Tokenizer.Token));
                    continue;
                  case "transition":
                    OptionTransition optionTransition = new OptionTransition();
                    this._Tokenizer.GetToken();
                    optionTransition.EntryBone = Convert.ToInt32(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionTransition.ExitBone = Convert.ToInt32(this._Tokenizer.Token);
                    commandSequenceV10.Transition = optionTransition;
                    continue;
                  case "{":
                    ++num1;
                    continue;
                  case "}":
                    --num1;
                    continue;
                  default:
                    MotionFlags motionFlags = Enumerators.ToMotionFlags(this._Tokenizer.Token);
                    ActivityV10 activityV10;
                    try
                    {
                      activityV10 = (ActivityV10) Enum.Parse(typeof (ActivityV10), this._Tokenizer.Token);
                    }
                    catch (ArgumentException ex)
                    {
                      activityV10 = ActivityV10.ACT_INVALID;
                    }
                    if (motionFlags != MotionFlags.Invalid)
                    {
                      commandSequenceV10.Control.Flags |= motionFlags;
                      continue;
                    }
                    if (activityV10 != ActivityV10.ACT_INVALID)
                    {
                      OptionActivityV10 optionActivityV10 = new OptionActivityV10();
                      optionActivityV10.Activity = activityV10;
                      this._Tokenizer.GetToken();
                      optionActivityV10.ActivityWeight = Convert.ToSingle(this._Tokenizer.Token);
                      commandSequenceV10.Activity = optionActivityV10;
                      continue;
                    }
                    StaticMethods.SmdFile = fileInfo.DirectoryName + "\\" + this._Tokenizer.Token + ".smd";
                    commandSequenceV10.FileCollection.Add(this._Tokenizer.Token);
                    continue;
                }
              }
              this._SequenceCollection.Add(commandSequenceV10);
              continue;
            case "$texturegroup":
              CommandTextureGroup commandTextureGroup = new CommandTextureGroup();
              this._Tokenizer.GetToken();
              commandTextureGroup.Name = this._Tokenizer.Token;
              int num2 = 0;
              List<string> stringList = new List<string>();
              while (!this._Tokenizer.GetToken())
              {
                if (this._Tokenizer.Token == "{")
                  ++num2;
                else if (this._Tokenizer.Token == "}")
                {
                  --num2;
                  if (num2 != 0)
                  {
                    commandTextureGroup.SkinCollection.Add(stringList);
                    stringList = new List<string>();
                  }
                  else
                    break;
                }
                else if (num2 == 2)
                  stringList.Add(this._Tokenizer.Token);
              }
              continue;
            default:
              if (this._Tokenizer.Token.StartsWith("$"))
              {
                long line = this._Tokenizer.Line;
                string command = this._Tokenizer.Token;
                while (!this._Tokenizer.GetToken())
                {
                  if (this._Tokenizer.Token.StartsWith("$") && this._Tokenizer.Line != line)
                  {
                    flag2 = true;
                    break;
                  }
                  command = command + "\r" + this._Tokenizer.Token;
                }
                this._OmittedCommands.Add(new OmittedCommand(line, command));
                continue;
              }
              continue;
          }
        }
        catch (Exception ex)
        {
          Messages.ThrowException("Qc.QcFileV10.Read(string)", ex.Message + " (" + Messages.LINE + (object) this._Tokenizer.Line + ", " + Messages.TOKEN + this._Tokenizer.Token + ")");
        }
      }
    }

    public void Write(string file)
    {
      StreamWriter streamWriter = new StreamWriter(file);
      if (this._Cd != null)
        streamWriter.WriteLine(this._Cd.ToString());
      if (this._CdTexture != null)
        streamWriter.WriteLine(this._CdTexture.ToString());
      if (this._ClipToTextures != null)
        streamWriter.WriteLine(this._ClipToTextures.ToString());
      if (this._ExternalTextures != null)
        streamWriter.WriteLine(this._ExternalTextures.ToString());
      if (this._ModelName != null)
        streamWriter.WriteLine(this._ModelName.ToString());
      streamWriter.WriteLine();
      streamWriter.Flush();
      if (this._Root != null)
        streamWriter.WriteLine(this._Root.ToString());
      if (this._Pivot != null)
        streamWriter.WriteLine(this._Pivot.ToString());
      if (this._MirrorBone != null)
        streamWriter.WriteLine(this._MirrorBone.ToString());
      if (this._RenameBone != null)
        streamWriter.WriteLine(this._RenameBone.ToString());
      streamWriter.WriteLine();
      streamWriter.Flush();
      if (this._Gamma != null)
        streamWriter.WriteLine(this._Gamma.ToString());
      if (this._Scale != null)
        streamWriter.WriteLine(this._Scale.ToString());
      if (this._Flags != null)
        streamWriter.WriteLine(this._Flags.ToString());
      if (this._Origin != null)
        streamWriter.WriteLine(this._Origin.ToString());
      if (this._EyePosition != null)
        streamWriter.WriteLine(this._EyePosition.ToString());
      if (this._BBox != null)
        streamWriter.WriteLine(this._BBox.ToString());
      if (this._CBox != null)
        streamWriter.WriteLine(this._CBox.ToString());
      streamWriter.WriteLine();
      streamWriter.Flush();
      if (this._TextureGroup != null)
        streamWriter.WriteLine(this._TextureGroup.ToString());
      streamWriter.WriteLine();
      streamWriter.Flush();
      foreach (CommandBody body in this._BodyCollection)
        streamWriter.WriteLine(body.ToString());
      streamWriter.WriteLine();
      streamWriter.Flush();
      foreach (CommandBodyGroup bodyGroup in this._BodyGroupCollection)
        streamWriter.WriteLine(bodyGroup.ToString());
      streamWriter.WriteLine();
      streamWriter.Flush();
      foreach (CommandAttachment attachment in this._AttachmentCollection)
        streamWriter.WriteLine(attachment.ToString());
      streamWriter.WriteLine();
      streamWriter.Flush();
      foreach (CommandController controller in this._ControllerCollection)
        streamWriter.WriteLine(controller.ToString());
      streamWriter.WriteLine();
      streamWriter.Flush();
      foreach (CommandHBox hbox in this._HBoxCollection)
        streamWriter.WriteLine(hbox.ToString());
      streamWriter.WriteLine();
      streamWriter.Flush();
      foreach (CommandSequenceV10 sequence in this._SequenceCollection)
      {
        streamWriter.WriteLine(sequence.ToString());
        streamWriter.Flush();
      }
      streamWriter.Close();
    }

    public QcFileV11 ToV11()
    {
      QcFileV11 v11 = new QcFileV11();
      v11.AttachmentCollection = this._AttachmentCollection;
      v11.BBox = this._BBox;
      v11.BodyCollection = this._BodyCollection;
      v11.BodyGroupCollection = this._BodyGroupCollection;
      v11.CBox = this._CBox;
      v11.Cd = this._Cd;
      v11.CdTexture = this._CdTexture;
      v11.ControllerCollection = this._ControllerCollection;
      v11.EyePosition = this._EyePosition;
      v11.Flags = this._Flags;
      v11.Gamma = this._Gamma;
      v11.HBoxCollection = this._HBoxCollection;
      v11.MirrorBone = this._MirrorBone;
      v11.ModelName = this._ModelName;
      v11.Origin = this._Origin;
      v11.Pivot = this._Pivot;
      v11.RenameBone = this._RenameBone;
      v11.Root = this._Root;
      v11.Scale = this._Scale;
      foreach (CommandSequenceV10 sequence in this._SequenceCollection)
        v11.SequenceCollection.Add(sequence.ToV11());
      v11.TextureGroup = this._TextureGroup;
      foreach (OmittedCommand omittedCommand in this._OmittedCommands)
      {
        string[] array = omittedCommand.Command.Split(new char[1]
        {
          '\r'
        }, StringSplitOptions.RemoveEmptyEntries);
        if (array[0].ToLower() == "$texture" && array.Length > 2)
          v11.TextureCollection.Add(new CommandTexture(array[1], array[2]));
        else if (array[0].ToLower() == "$include" && array.Length > 1)
          v11.Include = new CommandInclude(array[1]);
        else if (array[0].ToLower() == "$soundgroup" && array.Length > 3)
        {
          CommandSoundGroup commandSoundGroup = new CommandSoundGroup();
          commandSoundGroup.Name = array[1];
          for (int index = 3; index < Array.IndexOf<string>(array, "}"); ++index)
            commandSoundGroup.Sounds.Add(array[index]);
          v11.SoundGroupCollection.Add(commandSoundGroup);
        }
        else
          v11._OmittedCommands.Add(omittedCommand);
      }
      return v11;
    }

    public QcFileV44 ToV44()
    {
      QcFileV44 v44 = new QcFileV44();
      v44.Clear();
      v44.AttachmentCollection = this._AttachmentCollection;
      v44.BBox = this._BBox;
      v44.BodyCollection = this._BodyCollection;
      v44.BodyGroupCollection = this._BodyGroupCollection;
      v44.ControllerCollection = this._ControllerCollection;
      v44.EyePosition = this._EyePosition;
      v44.Flags = this._Flags;
      v44.Gamma = this._Gamma;
      v44.HBoxCollection = this._HBoxCollection;
      v44.ModelName = this._ModelName;
      v44.Origin = this._Origin;
      foreach (CommandSequenceV10 sequence in this._SequenceCollection)
        v44.SequenceCollection.Add(sequence.ToV44());
      v44.TextureGroup = this._TextureGroup;
      foreach (OmittedCommand omittedCommand in this._OmittedCommands)
        v44.OmittedCommands.Add(omittedCommand);
      return v44;
    }
  }
}
