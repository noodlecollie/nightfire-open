// Decompiled with JetBrains decompiler
// Type: JbnLib.Qc.QcFileV44
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
  public class QcFileV44 : QcFile
  {
    private List<CommandAnimation> _AnimationCollection;
    private List<CommandModel> _ModelCollection;
    private List<CommandSequenceV44> _SequenceCollection;

    public List<CommandAnimation> AnimationCollection
    {
      get => this._AnimationCollection;
      set => this._AnimationCollection = value;
    }

    public List<CommandModel> ModelCollection
    {
      get => this._ModelCollection;
      set => this._ModelCollection = value;
    }

    public List<CommandSequenceV44> SequenceCollection
    {
      get => this._SequenceCollection;
      set => this._SequenceCollection = value;
    }

    public QcFileV44()
    {
      Thread.CurrentThread.CurrentUICulture = new CultureInfo("en");
      Thread.CurrentThread.CurrentCulture = new CultureInfo("en-US");
      this.Clear();
    }

    public void Clear()
    {
      this._AnimationCollection = new List<CommandAnimation>();
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
      this._MirrorBone = (CommandMirrorBone) null;
      this._ModelCollection = new List<CommandModel>();
      this._ModelName = (CommandModelName) null;
      this._Origin = (CommandOrigin) null;
      this._Pivot = (CommandPivot) null;
      this._RenameBone = (CommandRenameBone) null;
      this._Root = (CommandRoot) null;
      this._Scale = (CommandScale) null;
      this._SequenceCollection = new List<CommandSequenceV44>();
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
            case "$animation":
              CommandAnimation commandAnimation = new CommandAnimation();
              this._Tokenizer.GetToken();
              commandAnimation.Name = this._Tokenizer.Token;
              this._Tokenizer.GetToken();
              commandAnimation.File = this._Tokenizer.Token;
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
                  case "{":
                    ++num1;
                    continue;
                  case "}":
                    --num1;
                    continue;
                  case "fps":
                    this._Tokenizer.GetToken();
                    commandAnimation.Fps = new OptionFps(Convert.ToSingle(this._Tokenizer.Token));
                    continue;
                  case "loop":
                    commandAnimation.Loop = new OptionLoop(true);
                    continue;
                  default:
                    continue;
                }
              }
              this._AnimationCollection.Add(commandAnimation);
              continue;
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
              StaticMethods.SmdFile = fileInfo.DirectoryName + "\\" + StaticMethods.StripExtension(commandBody.File) + ".smd";
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
                      StaticMethods.SmdFile = fileInfo.DirectoryName + "\\" + StaticMethods.StripExtension(bodyGroupItem.File) + ".smd";
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
            case "$mirrorbone":
              this._Tokenizer.GetToken();
              this._MirrorBone = new CommandMirrorBone(this._Tokenizer.Token);
              continue;
            case "$model":
              CommandModel commandModel = new CommandModel();
              this._Tokenizer.GetToken();
              commandModel.Name = this._Tokenizer.Token;
              this._Tokenizer.GetToken();
              commandModel.File = this._Tokenizer.Token;
              this._ModelCollection.Add(commandModel);
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
              CommandSequenceV44 commandSequenceV44 = new CommandSequenceV44();
              this._Tokenizer.GetToken();
              commandSequenceV44.Name = this._Tokenizer.Token;
              int num2 = 0;
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
                    commandSequenceV44.Animation = new OptionAnimation(this._Tokenizer.Token);
                    continue;
                  case "blend":
                    OptionBlend optionBlend = new OptionBlend();
                    this._Tokenizer.GetToken();
                    optionBlend.Type = Enumerators.ToMotionFlags(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionBlend.Start = Convert.ToSingle(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionBlend.End = Convert.ToSingle(this._Tokenizer.Token);
                    commandSequenceV44.BlendCollection.Add(optionBlend);
                    continue;
                  case "blendwidth":
                    this._Tokenizer.GetToken();
                    commandSequenceV44.BlendWidth = new OptionBlendWidth(Convert.ToInt32(this._Tokenizer.Token));
                    continue;
                  case "event":
                    OptionEvent optionEvent = new OptionEvent();
                    this._Tokenizer.GetToken();
                    optionEvent.EventValue = Convert.ToInt32(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionEvent.Frame = Convert.ToInt32(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionEvent.Options = this._Tokenizer.Token;
                    commandSequenceV44.EventCollection.Add(optionEvent);
                    continue;
                  case "fps":
                    this._Tokenizer.GetToken();
                    commandSequenceV44.Fps = new OptionFps(Convert.ToSingle(this._Tokenizer.Token));
                    continue;
                  case "frame":
                    OptionFrame optionFrame = new OptionFrame();
                    this._Tokenizer.GetToken();
                    optionFrame.Start = Convert.ToInt32(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionFrame.End = Convert.ToInt32(this._Tokenizer.Token);
                    commandSequenceV44.Frame = optionFrame;
                    continue;
                  case "loop":
                    commandSequenceV44.Loop = new OptionLoop(true);
                    continue;
                  case "node":
                    this._Tokenizer.GetToken();
                    commandSequenceV44.Node = new OptionNodeV44(this._Tokenizer.Token);
                    continue;
                  case "pivot":
                    OptionPivot optionPivot = new OptionPivot();
                    this._Tokenizer.GetToken();
                    optionPivot.Index = Convert.ToInt32(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionPivot.Start = (float) Convert.ToInt32(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionPivot.End = (float) Convert.ToInt32(this._Tokenizer.Token);
                    commandSequenceV44.PivotCollection.Add(optionPivot);
                    continue;
                  case "rotate":
                    this._Tokenizer.GetToken();
                    commandSequenceV44.Rotate = new OptionRotate(Convert.ToInt32(this._Tokenizer.Token));
                    continue;
                  case "rtransition":
                    OptionRTransition optionRtransition = new OptionRTransition();
                    this._Tokenizer.GetToken();
                    optionRtransition.EntryBone = Convert.ToInt32(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionRtransition.ExitBone = Convert.ToInt32(this._Tokenizer.Token);
                    commandSequenceV44.RTransition = optionRtransition;
                    continue;
                  case "scale":
                    this._Tokenizer.GetToken();
                    commandSequenceV44.Scale = new OptionScale(Convert.ToSingle(this._Tokenizer.Token));
                    continue;
                  case "transition":
                    OptionTransition optionTransition = new OptionTransition();
                    this._Tokenizer.GetToken();
                    optionTransition.EntryBone = Convert.ToInt32(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    optionTransition.ExitBone = Convert.ToInt32(this._Tokenizer.Token);
                    commandSequenceV44.Transition = optionTransition;
                    continue;
                  case "{":
                    ++num2;
                    continue;
                  case "}":
                    --num2;
                    continue;
                  default:
                    MotionFlags motionFlags = Enumerators.ToMotionFlags(this._Tokenizer.Token);
                    ActivityV44 activityV44;
                    try
                    {
                      activityV44 = (ActivityV44) Enum.Parse(typeof (ActivityV44), this._Tokenizer.Token);
                    }
                    catch (ArgumentException ex)
                    {
                      activityV44 = ActivityV44.ACT_INVALID;
                    }
                    if (motionFlags != MotionFlags.Invalid)
                    {
                      commandSequenceV44.Control.Flags |= motionFlags;
                      continue;
                    }
                    if (activityV44 != ActivityV44.ACT_INVALID)
                    {
                      OptionActivityV44 optionActivityV44 = new OptionActivityV44();
                      optionActivityV44.Activity = activityV44;
                      this._Tokenizer.GetToken();
                      optionActivityV44.ActivityWeight = Convert.ToSingle(this._Tokenizer.Token);
                      commandSequenceV44.Activity = optionActivityV44;
                      continue;
                    }
                    StaticMethods.SmdFile = fileInfo.DirectoryName + "\\" + StaticMethods.StripExtension(this._Tokenizer.Token) + ".smd";
                    commandSequenceV44.FileCollection.Add(this._Tokenizer.Token);
                    continue;
                }
              }
              this._SequenceCollection.Add(commandSequenceV44);
              continue;
            case "$texturegroup":
              CommandTextureGroup commandTextureGroup = new CommandTextureGroup();
              this._Tokenizer.GetToken();
              commandTextureGroup.Name = this._Tokenizer.Token;
              int num3 = 0;
              List<string> stringList = new List<string>();
              while (!this._Tokenizer.GetToken())
              {
                if (this._Tokenizer.Token == "{")
                  ++num3;
                else if (this._Tokenizer.Token == "}")
                {
                  --num3;
                  if (num3 != 0)
                  {
                    commandTextureGroup.SkinCollection.Add(stringList);
                    stringList = new List<string>();
                  }
                  else
                    break;
                }
                else if (num3 == 2)
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
          Messages.ThrowException("Qc.QcFileV44.Read(string)", ex.Message + " (" + Messages.LINE + (object) this._Tokenizer.Line + ", " + Messages.TOKEN + this._Tokenizer.Token + ")");
        }
      }
    }

    public void Write(string file)
    {
      StreamWriter streamWriter = new StreamWriter(file);
      if (this._Cd != null)
        streamWriter.WriteLine(this._Cd.ToString());
      if (this._CdTexture != null)
        streamWriter.WriteLine("$cdmaterials \"" + this._CdTexture.Directory + "\"");
      else
        streamWriter.WriteLine("$cdmaterials \"models\\props\\\"");
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
      streamWriter.WriteLine("$staticprop");
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
      foreach (CommandSequenceV44 sequence in this._SequenceCollection)
      {
        streamWriter.WriteLine(sequence.ToString());
        streamWriter.Flush();
      }
      streamWriter.Close();
    }

    public QcFileV10 ToV10()
    {
      QcFileV10 v10 = new QcFileV10();
      v10.Clear();
      v10.AttachmentCollection = this._AttachmentCollection;
      v10.BBox = this._BBox;
      v10.BodyCollection = this._BodyCollection;
      foreach (CommandModel model in this._ModelCollection)
        v10.BodyCollection.Add(new CommandBody(model.Name, StaticMethods.StripExtension(model.File)));
      v10.BodyGroupCollection = this._BodyGroupCollection;
      v10.ControllerCollection = this._ControllerCollection;
      v10.EyePosition = this._EyePosition;
      v10.Flags = this._Flags;
      v10.Gamma = this._Gamma;
      v10.HBoxCollection = this._HBoxCollection;
      v10.ModelName = this._ModelName;
      v10.Origin = this._Origin;
      foreach (CommandSequenceV44 sequence in this._SequenceCollection)
        v10.SequenceCollection.Add(sequence.ToV10());
      v10.TextureGroup = this._TextureGroup;
      foreach (OmittedCommand omittedCommand in this._OmittedCommands)
        v10.OmittedCommands.Add(omittedCommand);
      return v10;
    }

    public QcFileV11 ToV11()
    {
      QcFileV11 v11 = new QcFileV11();
      v11.Clear();
      v11.AttachmentCollection = this._AttachmentCollection;
      v11.BBox = this._BBox;
      v11.BodyCollection = this._BodyCollection;
      foreach (CommandModel model in this._ModelCollection)
        v11.BodyCollection.Add(new CommandBody(model.Name, StaticMethods.StripExtension(model.File)));
      v11.BodyGroupCollection = this._BodyGroupCollection;
      v11.ControllerCollection = this._ControllerCollection;
      v11.EyePosition = this._EyePosition;
      v11.Flags = this._Flags;
      v11.Gamma = this._Gamma;
      v11.HBoxCollection = this._HBoxCollection;
      v11.ModelName = this._ModelName;
      v11.Origin = this._Origin;
      foreach (CommandSequenceV44 sequence in this._SequenceCollection)
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
  }
}
