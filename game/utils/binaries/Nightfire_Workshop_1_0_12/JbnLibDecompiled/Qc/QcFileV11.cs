// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.QcFileV11
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
  public class QcFileV11 : QcFile
  {
    private List<CommandSequenceV11> _SequenceCollection;
    private CommandInclude _Include;
    private List<CommandSoundGroup> _SoundGroupCollection;
    private List<CommandTexture> _TextureCollection;

    public List<CommandSequenceV11> SequenceCollection
    {
      get => this._SequenceCollection;
      set => this._SequenceCollection = value;
    }

    public CommandInclude Include
    {
      get => this._Include;
      set => this._Include = value;
    }

    public List<CommandSoundGroup> SoundGroupCollection
    {
      get => this._SoundGroupCollection;
      set => this._SoundGroupCollection = value;
    }

    public List<CommandTexture> TextureCollection
    {
      get => this._TextureCollection;
      set => this._TextureCollection = value;
    }

    public QcFileV11()
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
      this._ControllerCollection = new List<CommandController>();
      this._EyePosition = (CommandEyePosition) null;
      this._Flags = (CommandFlags) null;
      this._Gamma = (CommandGamma) null;
      this._HBoxCollection = new List<CommandHBox>();
      this._Include = (CommandInclude) null;
      this._MirrorBone = (CommandMirrorBone) null;
      this._ModelName = (CommandModelName) null;
      this._Origin = (CommandOrigin) null;
      this._Pivot = (CommandPivot) null;
      this._RenameBone = (CommandRenameBone) null;
      this._Root = (CommandRoot) null;
      this._Scale = (CommandScale) null;
      this._SequenceCollection = new List<CommandSequenceV11>();
      this._SoundGroupCollection = new List<CommandSoundGroup>();
      this._TextureCollection = new List<CommandTexture>();
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
            case "$include":
              this._Tokenizer.GetToken();
              this._Include = new CommandInclude(this._Tokenizer.Token);
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
              CommandSequenceV11 commandSequenceV11 = new CommandSequenceV11();
              this._Tokenizer.GetToken();
              commandSequenceV11.Name = this._Tokenizer.Token;
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
                    commandSequenceV11.Animation = new OptionAnimation(this._Tokenizer.Token);
                    continue;
                  case "blend":
                    OptionBlend optionBlend = new OptionBlend();
                    this._Tokenizer.GetToken();
                    optionBlend.Type = Enumerators.ToMotionFlags(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionBlend.Start = Convert.ToSingle(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionBlend.End = Convert.ToSingle(this._Tokenizer.Token);
                    commandSequenceV11.BlendCollection.Add(optionBlend);
                    continue;
                  case "event":
                    OptionEvent optionEvent = new OptionEvent();
                    this._Tokenizer.GetToken();
                    optionEvent.EventValue = Convert.ToInt32(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionEvent.Frame = Convert.ToInt32(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionEvent.Options = this._Tokenizer.Token;
                    commandSequenceV11.EventCollection.Add(optionEvent);
                    continue;
                  case "fps":
                    this._Tokenizer.GetToken();
                    commandSequenceV11.Fps = new OptionFps(Convert.ToSingle(this._Tokenizer.Token));
                    continue;
                  case "frame":
                    OptionFrame optionFrame = new OptionFrame();
                    this._Tokenizer.GetToken();
                    optionFrame.Start = Convert.ToInt32(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionFrame.End = Convert.ToInt32(this._Tokenizer.Token);
                    commandSequenceV11.Frame = optionFrame;
                    continue;
                  case "loop":
                    commandSequenceV11.Loop = new OptionLoop(true);
                    continue;
                  case "node":
                    this._Tokenizer.GetToken();
                    commandSequenceV11.Node = new OptionNodeV10(Convert.ToSByte(this._Tokenizer.Token));
                    continue;
                  case "pivot":
                    OptionPivot optionPivot = new OptionPivot();
                    this._Tokenizer.GetToken();
                    optionPivot.Index = Convert.ToInt32(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionPivot.Start = (float) Convert.ToInt32(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionPivot.End = (float) Convert.ToInt32(this._Tokenizer.Token);
                    commandSequenceV11.PivotCollection.Add(optionPivot);
                    continue;
                  case "rotate":
                    this._Tokenizer.GetToken();
                    commandSequenceV11.Rotate = new OptionRotate(Convert.ToInt32(this._Tokenizer.Token));
                    continue;
                  case "rtransition":
                    OptionRTransition optionRtransition = new OptionRTransition();
                    this._Tokenizer.GetToken();
                    optionRtransition.EntryBone = Convert.ToInt32(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionRtransition.ExitBone = Convert.ToInt32(this._Tokenizer.Token);
                    commandSequenceV11.RTransition = optionRtransition;
                    continue;
                  case "scale":
                    this._Tokenizer.GetToken();
                    commandSequenceV11.Scale = new OptionScale(Convert.ToSingle(this._Tokenizer.Token));
                    continue;
                  case "transition":
                    OptionTransition optionTransition = new OptionTransition();
                    this._Tokenizer.GetToken();
                    optionTransition.EntryBone = Convert.ToInt32(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionTransition.ExitBone = Convert.ToInt32(this._Tokenizer.Token);
                    commandSequenceV11.Transition = optionTransition;
                    continue;
                  case "{":
                    ++num1;
                    continue;
                  case "}":
                    --num1;
                    continue;
                  default:
                    MotionFlags motionFlags = Enumerators.ToMotionFlags(this._Tokenizer.Token);
                    ActivityV11 activityV11;
                    try
                    {
                      activityV11 = (ActivityV11) Enum.Parse(typeof (ActivityV11), this._Tokenizer.Token);
                    }
                    catch (ArgumentException ex)
                    {
                      activityV11 = ActivityV11.ACT_INVALID;
                    }
                    if (motionFlags != MotionFlags.Invalid)
                    {
                      commandSequenceV11.Control.Flags |= motionFlags;
                      continue;
                    }
                    if (activityV11 != ActivityV11.ACT_INVALID)
                    {
                      OptionActivityV11 optionActivityV11 = new OptionActivityV11();
                      optionActivityV11.Activity = activityV11;
                      this._Tokenizer.GetToken();
                      optionActivityV11.ActivityWeight = Convert.ToSingle(this._Tokenizer.Token);
                      commandSequenceV11.Activity = optionActivityV11;
                      continue;
                    }
                    StaticMethods.SmdFile = fileInfo.DirectoryName + "\\" + this._Tokenizer.Token + ".smd";
                    commandSequenceV11.FileCollection.Add(this._Tokenizer.Token);
                    continue;
                }
              }
              this._SequenceCollection.Add(commandSequenceV11);
              continue;
            case "$soundgroup":
              CommandSoundGroup commandSoundGroup = new CommandSoundGroup();
              this._Tokenizer.GetToken();
              commandSoundGroup.Name = this._Tokenizer.Token;
              this._Tokenizer.GetToken();
              if (this._Tokenizer.Token == "{")
              {
                while (this._Tokenizer.Token[0] != '}')
                {
                  this._Tokenizer.GetToken();
                  if (this._Tokenizer.Token[0] != '}')
                    commandSoundGroup.Sounds.Add(this._Tokenizer.Token);
                }
              }
              this._SoundGroupCollection.Add(commandSoundGroup);
              continue;
            case "$texture":
              CommandTexture commandTexture = new CommandTexture();
              this._Tokenizer.GetToken();
              commandTexture.Texture = this._Tokenizer.Token;
              this._Tokenizer.GetToken();
              commandTexture.Material = this._Tokenizer.Token;
              this._TextureCollection.Add(commandTexture);
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
          Messages.ThrowException("Qc.QcFileV11.Read(string)", ex.Message + " (" + Messages.LINE + (object) this._Tokenizer.Line + ", " + Messages.TOKEN + this._Tokenizer.Token + ")");
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
      if (this._Include != null)
        streamWriter.WriteLine(this._Include.ToString());
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
      foreach (CommandTexture texture in this._TextureCollection)
        streamWriter.WriteLine(texture.ToString());
      streamWriter.WriteLine();
      streamWriter.Flush();
      if (this._TextureGroup != null)
        streamWriter.WriteLine(this._TextureGroup.ToString());
      streamWriter.WriteLine();
      streamWriter.Flush();
      foreach (CommandSoundGroup soundGroup in this._SoundGroupCollection)
        streamWriter.WriteLine(soundGroup.ToString());
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
      foreach (CommandSequenceV11 sequence in this._SequenceCollection)
      {
        streamWriter.WriteLine(sequence.ToString());
        streamWriter.Flush();
      }
      streamWriter.Close();
    }

    public QcFileV10 ToV10()
    {
      QcFileV10 v10 = new QcFileV10();
      v10.AttachmentCollection = this._AttachmentCollection;
      v10.BBox = this._BBox;
      v10.BodyCollection = this._BodyCollection;
      v10.BodyGroupCollection = this._BodyGroupCollection;
      v10.CBox = this._CBox;
      v10.Cd = this._Cd;
      v10.CdTexture = this._CdTexture;
      v10.ControllerCollection = this._ControllerCollection;
      v10.EyePosition = this._EyePosition;
      v10.Flags = this._Flags;
      v10.Gamma = this._Gamma;
      v10.HBoxCollection = this._HBoxCollection;
      v10.MirrorBone = this._MirrorBone;
      v10.ModelName = this._ModelName;
      v10.Origin = this._Origin;
      v10.Pivot = this._Pivot;
      v10.RenameBone = this._RenameBone;
      v10.Root = this._Root;
      v10.Scale = this._Scale;
      foreach (CommandSequenceV11 sequence in this._SequenceCollection)
        v10.SequenceCollection.Add(sequence.ToV10());
      v10.TextureGroup = this._TextureGroup;
      foreach (OmittedCommand omittedCommand in this._OmittedCommands)
        v10.OmittedCommands.Add(omittedCommand);
      foreach (CommandSoundGroup soundGroup in this._SoundGroupCollection)
      {
        string str = "$soundgroup\r" + soundGroup.Name + "\r{";
        foreach (string sound in soundGroup.Sounds)
          str = str + "\r" + sound;
        v10.OmittedCommands.Add(new OmittedCommand(0L, str + "\r}"));
      }
      if (this._Include != null)
        v10.OmittedCommands.Add(new OmittedCommand(0L, "$include\r" + this._Include.File));
      foreach (CommandTexture texture in this._TextureCollection)
        v10.OmittedCommands.Add(new OmittedCommand(0L, "$texture\r" + texture.Texture + "\r" + texture.Material));
      return v10;
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
      foreach (CommandSequenceV11 sequence in this._SequenceCollection)
        v44.SequenceCollection.Add(sequence.ToV44());
      v44.TextureGroup = this._TextureGroup;
      foreach (OmittedCommand omittedCommand in this._OmittedCommands)
        v44.OmittedCommands.Add(omittedCommand);
      foreach (CommandSoundGroup soundGroup in this._SoundGroupCollection)
      {
        string str = "$soundgroup\r" + soundGroup.Name + "\r{";
        foreach (string sound in soundGroup.Sounds)
          str = str + "\r" + sound;
        v44.OmittedCommands.Add(new OmittedCommand(0L, str + "\r}"));
      }
      if (this._Include != null)
        v44.OmittedCommands.Add(new OmittedCommand(0L, "$include\r" + this._Include.File));
      foreach (CommandTexture texture in this._TextureCollection)
        v44.OmittedCommands.Add(new OmittedCommand(0L, "$texture\r" + texture.Texture + "\r" + texture.Material));
      return v44;
    }
  }
}
