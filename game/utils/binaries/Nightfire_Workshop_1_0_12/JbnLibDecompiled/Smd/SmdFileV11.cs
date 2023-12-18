// Decompiled with JetBrains decompiler
// Type: JbnLib.Smd.SmdFileV11
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

namespace JbnLib.Smd
{
  public class SmdFileV11 : SmdFile
  {
    private List<Triangle<VertexV11>> _TriangleCollection;

    public List<Triangle<VertexV11>> TriangleCollection
    {
      get => this._TriangleCollection;
      set
      {
        if (this._Reference)
          this._TriangleCollection = value;
        else
          Messages.ThrowException("Smd.SmdFileV11.TriangleCollection", Messages.ANIMATION_TRIS);
      }
    }

    public SmdFileV11(SmdType type)
    {
      Thread.CurrentThread.CurrentUICulture = new CultureInfo("en");
      Thread.CurrentThread.CurrentCulture = new CultureInfo("en-US");
      if (type == SmdType.Animation)
        this._Reference = false;
      else
        this._Reference = true;
      this.Clear();
    }

    public void Clear()
    {
      this._NodeCollection = new List<Node>();
      this._TimeCollection = new List<Frame>();
      this._TriangleCollection = new List<Triangle<VertexV11>>();
    }

    public void Read(string file)
    {
      this.Clear();
      this._Tokenizer = new FileTokenizer(file);
label_30:
      while (!this._Tokenizer.GetToken())
      {
        switch (this._Tokenizer.Token)
        {
          case "version":
            this._Tokenizer.GetToken();
            if (Convert.ToInt32(this._Tokenizer.Token) != 1)
            {
              Messages.ThrowException("Smd.SmdFileV11.Read(string)", Messages.VERSION_ONE);
              continue;
            }
            continue;
          case "nodes":
            while (true)
            {
              if (!this._Tokenizer.GetToken() && !(this._Tokenizer.Token == "end"))
              {
                Node node = new Node();
                node.Index = Convert.ToSByte(this._Tokenizer.Token);
                this._Tokenizer.GetToken();
                node.Name = this._Tokenizer.Token;
                this._Tokenizer.GetToken();
                node.Parent = Convert.ToSByte(this._Tokenizer.Token);
                this._NodeCollection.Add(node);
              }
              else
                goto label_30;
            }
          case "skeleton":
            this._Tokenizer.GetToken();
            while (this._Tokenizer.Token == "time")
            {
              this._Tokenizer.GetToken();
              Frame frame = new Frame();
              frame.FrameValue = Convert.ToInt32(this._Tokenizer.Token);
              try
              {
                while (!this._Tokenizer.GetToken())
                {
                  Time time = new Time();
                  time.Node = Convert.ToInt32(this._Tokenizer.Token);
                  this._Tokenizer.GetToken();
                  time.Position.X = Convert.ToSingle(this._Tokenizer.Token);
                  this._Tokenizer.GetToken();
                  time.Position.Y = Convert.ToSingle(this._Tokenizer.Token);
                  this._Tokenizer.GetToken();
                  time.Position.Z = Convert.ToSingle(this._Tokenizer.Token);
                  this._Tokenizer.GetToken();
                  time.Rotation.X = Convert.ToSingle(this._Tokenizer.Token);
                  this._Tokenizer.GetToken();
                  time.Rotation.Y = Convert.ToSingle(this._Tokenizer.Token);
                  this._Tokenizer.GetToken();
                  time.Rotation.Z = Convert.ToSingle(this._Tokenizer.Token);
                  frame.Bones.Add(time);
                }
              }
              catch
              {
                this._TimeCollection.Add(frame);
              }
            }
            continue;
          case "triangles":
            if (this._Reference)
            {
              while (true)
              {
                if (!this._Tokenizer.GetToken() && !(this._Tokenizer.Token == "end"))
                {
                  Triangle<VertexV11> triangle1 = new Triangle<VertexV11>();
                  triangle1.Texture = this._Tokenizer.Token;
                  long line = this._Tokenizer.Line;
                  this._Tokenizer.GetToken();
                  while (this._Tokenizer.Line == line)
                  {
                    Triangle<VertexV11> triangle2 = triangle1;
                    triangle2.Texture = triangle2.Texture + " " + this._Tokenizer.Token;
                    this._Tokenizer.GetToken();
                  }
                  triangle1.Texture = StaticMethods.StripExtension(triangle1.Texture);
                  for (int index1 = 0; index1 < 3; ++index1)
                  {
                    VertexV11 vertexV11 = new VertexV11();
                    if (this._Tokenizer.Token != "$blended")
                      Messages.ThrowException("Smd.SmdFileV11.Read(string)", Messages.INVALID_TRIS);
                    this._Tokenizer.GetToken();
                    vertexV11.Blends = new BlendV11[Convert.ToInt32(this._Tokenizer.Token)];
                    for (byte index2 = 0; (int) index2 < vertexV11.Blends.Length; ++index2)
                    {
                      this._Tokenizer.GetToken();
                      sbyte bone = Convert.ToSByte(this._Tokenizer.Token);
                      this._Tokenizer.GetToken();
                      vertexV11.Blends[(int) index2] = new BlendV11(bone, Convert.ToSingle(this._Tokenizer.Token));
                    }
                    this._Tokenizer.GetToken();
                    vertexV11.Position.X = Convert.ToSingle(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    vertexV11.Position.Y = Convert.ToSingle(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    vertexV11.Position.Z = Convert.ToSingle(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    vertexV11.Normal.X = Convert.ToSingle(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    vertexV11.Normal.Y = Convert.ToSingle(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    vertexV11.Normal.Z = Convert.ToSingle(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    vertexV11.TextureCoordinate.U = Convert.ToSingle(this._Tokenizer.Token);
                    this._Tokenizer.GetToken();
                    vertexV11.TextureCoordinate.V = Convert.ToSingle(this._Tokenizer.Token);
                    if (index1 != 2)
                      this._Tokenizer.GetToken();
                    triangle1.Vertices.Add(vertexV11);
                  }
                  this._TriangleCollection.Add(triangle1);
                }
                else
                  goto label_30;
              }
            }
            else
              continue;
          default:
            continue;
        }
      }
    }

    public void Write(string file)
    {
      StreamWriter streamWriter = new StreamWriter(file);
      streamWriter.WriteLine("version 1");
      streamWriter.WriteLine("nodes");
      foreach (Node node in this._NodeCollection)
        streamWriter.WriteLine(node.ToString());
      streamWriter.WriteLine("end");
      streamWriter.Flush();
      streamWriter.WriteLine("skeleton");
      foreach (Frame time in this._TimeCollection)
        streamWriter.WriteLine(time.ToString());
      streamWriter.WriteLine("end");
      streamWriter.Flush();
      if (this._Reference)
      {
        streamWriter.WriteLine("triangles");
        foreach (Triangle<VertexV11> triangle in this._TriangleCollection)
        {
          streamWriter.WriteLine(triangle.Texture + ".png");
          foreach (VertexV11 vertex in triangle.Vertices)
            streamWriter.WriteLine(vertex.ToString());
          streamWriter.Flush();
        }
        streamWriter.WriteLine("end");
        streamWriter.Flush();
      }
      streamWriter.Close();
    }

    public SmdFileV10 ToV10()
    {
      SmdFileV10 v10 = !this._Reference ? new SmdFileV10(SmdType.Animation) : new SmdFileV10(SmdType.Reference);
      v10.NodeCollection = this._NodeCollection;
      v10.TimeCollection = this._TimeCollection;
      foreach (Triangle<VertexV11> triangle1 in this._TriangleCollection)
      {
        Triangle<VertexV10> triangle2 = new Triangle<VertexV10>();
        triangle2.Texture = triangle1.Texture;
        foreach (VertexV11 vertex in triangle1.Vertices)
          triangle2.Vertices.Add(new VertexV10(vertex.Blends[0].Bone, vertex.Position, vertex.Normal, vertex.TextureCoordinate));
        v10.TriangleCollection.Add(triangle2);
      }
      return v10;
    }

    public SmdFileV44 ToV44()
    {
      SmdFileV44 v44 = !this._Reference ? new SmdFileV44(SmdType.Animation) : new SmdFileV44(SmdType.Reference);
      v44.NodeCollection = this._NodeCollection;
      v44.TimeCollection = this._TimeCollection;
      foreach (Triangle<VertexV11> triangle1 in this._TriangleCollection)
      {
        Triangle<VertexV44> triangle2 = new Triangle<VertexV44>();
        triangle2.Texture = triangle1.Texture;
        foreach (VertexV11 vertex in triangle1.Vertices)
        {
          List<LinkV44> links = new List<LinkV44>();
          for (int index = 0; index < vertex.Blends.Length; ++index)
          {
            if (vertex.Blends[index].Bone != (sbyte) -1)
              links.Add(new LinkV44(vertex.Blends[index].Bone, vertex.Blends[index].Scale));
          }
          triangle2.Vertices.Add(new VertexV44(vertex.Blends[0].Bone, vertex.Position, vertex.Normal, vertex.TextureCoordinate, links));
        }
        v44.TriangleCollection.Add(triangle2);
      }
      return v44;
    }

    public string[] GetTextures()
    {
      string[] input = new string[this._TriangleCollection.Count];
      for (int index = 0; index < input.Length; ++index)
        input[index] = this._TriangleCollection[index].Texture;
      return StaticMethods.EliminateDuplicates(input);
    }
  }
}
