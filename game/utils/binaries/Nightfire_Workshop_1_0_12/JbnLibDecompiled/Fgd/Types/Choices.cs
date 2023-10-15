// Decompiled with JetBrains decompiler
// Type: JbnLib.Fgd.Types.Choices
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Fgd.Interfaces;
using JbnLib.Shared;
using System.Collections.Generic;

namespace JbnLib.Fgd.Types
{
  public class Choices : IChoices, INameTextValue, INameText
  {
    private string _Name;
    private string _Text;
    private string _Value;
    private List<Choice> _ChoiceCollection = new List<Choice>();

    public string Name
    {
      get => this._Name;
      set => this._Name = value;
    }

    public string Text
    {
      get => this._Text;
      set => this._Text = value;
    }

    public string Value
    {
      get => this._Value;
      set => this._Value = value;
    }

    public List<Choice> ChoiceCollection
    {
      get => this._ChoiceCollection;
      set => this._ChoiceCollection = value;
    }

    public Choices()
    {
    }

    public Choices(string name, string text, string value, List<Choice> collection)
    {
      this._Name = name;
      this._Text = text;
      this._Value = value;
      this._ChoiceCollection = collection;
    }

    public Choices(string line)
    {
      string[] strArray = line.Substring(0, line.IndexOf('=')).Trim().Split(':');
      this._Name = strArray[0].Trim().ToLower().Replace("(choices)", "");
      if (strArray.Length == 2)
      {
        this._Text = "";
        this._Value = strArray[1].Trim().Trim('"');
      }
      else
      {
        this._Text = strArray[1].Trim().Trim('"');
        this._Value = strArray[2].Trim().Trim('"');
      }
      string str = line.Substring(line.IndexOf('[') + 1, line.LastIndexOf(']') - line.IndexOf('[') - 1).Trim();
      char[] chArray = new char[1]{ '\n' };
      foreach (string line1 in str.Split(chArray))
      {
        if (line1.Contains(":"))
          this._ChoiceCollection.Add(new Choice(line1));
      }
    }

    public new string ToString()
    {
      string str = this._Name + "(choices) : \"" + this._Text + "\" : " + StaticMethods.MagicQuotes(this._Value) + " =\n[\n";
      foreach (Choice choice in this._ChoiceCollection)
        str = str + "\t" + choice.ToString() + "\n";
      return str + "]";
    }
  }
}
