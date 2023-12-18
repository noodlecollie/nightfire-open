// Decompiled with JetBrains decompiler
// Type: JbnLib.Shared.Matrix
// Assembly: JbnLib, Version=1.0.12.40333, Culture=neutral, PublicKeyToken=null
// MVID: 6F6E5167-35CC-4CD8-AB0C-40DC9A8EA330
// Assembly location: C:\Users\Admin\Documents\GitRepos\nightfire-open\game\utils\binaries\Nightfire_Workshop_1_0_12\JbnLib.dll

using JbnLib.Lang;
using System;

namespace JbnLib.Shared
{
  public class Matrix
  {
    private int _rows = 1;
    private int _cols = 1;
    private double[,] _items = new double[1, 1];
    private bool _isSquare = true;

    public int Length => this._rows * this._cols * 4;

    public int Rows => this._rows;

    public int Cols => this._cols;

    public double[,] Items
    {
      get => this._items;
      set
      {
        if (value.Rank == 2)
        {
          if (value.GetLength(0) == this._rows && value.GetLength(1) == this._cols)
            this._items = value;
          else
            Messages.ThrowException("Matrix.Items[,]", "Cannot set matrix items because they are a different size than the matrix!");
        }
        else
          Messages.ThrowException("Matrix.Items[,]", "Cannot create matrix with other than 2 dimensions!");
      }
    }

    public double this[int i, int j]
    {
      get => this._items[i, j];
      set => this._items[i, j] = value;
    }

    public bool IsSquare => this._isSquare;

    public Matrix(int rows, int cols)
    {
      if (rows < 1 || cols < 1)
        Messages.ThrowException("Matrix(int, int)", "Cannot construct matrix smaller than 1x1!");
      this._rows = rows;
      this._cols = cols;
      this._items = new double[rows, cols];
      this._isSquare = this._rows == this._cols;
    }

    public Matrix(double[,] items)
    {
      if (items.Rank == 2)
      {
        this._rows = items.GetLength(0);
        this._cols = items.GetLength(1);
        if (this._rows < 1 || this._cols < 1)
          Messages.ThrowException("Matrix(double[,])", "Cannot construct matrix smaller than 1x1!");
        this._items = items;
        this._isSquare = this._rows == this._cols;
      }
      else
        Messages.ThrowException("Matrix(double[,])", "Cannot create matrix with other than 2 dimensions!");
    }

    public Matrix(Point3D items, bool isrow)
    {
      if (isrow)
      {
        this._rows = 1;
        this._cols = 3;
        this._items = new double[1, 3]
        {
          {
            items.dX,
            items.dY,
            items.dZ
          }
        };
      }
      else
      {
        this._rows = 3;
        this._cols = 1;
        this._items = new double[3, 1]
        {
          {
            items.dX
          },
          {
            items.dY
          },
          {
            items.dZ
          }
        };
      }
      this._isSquare = this._rows == this._cols;
    }

    public Matrix(Point3D[] items, bool arerows)
    {
      int length = items.GetLength(0);
      if (length < 1)
        Messages.ThrowException("Matrix(Point3D[])", "Cannot construct matrix with dimension smaller than 1!");
      if (arerows)
      {
        this._rows = length;
        this._cols = 3;
        this._items = new double[this._rows, 3];
        for (int index = 0; index < this._rows; ++index)
        {
          this._items[index, 0] = items[index].dX;
          this._items[index, 1] = items[index].dY;
          this._items[index, 2] = items[index].dZ;
        }
      }
      else
      {
        this._rows = 3;
        this._cols = length;
        this._items = new double[3, this._cols];
        for (int index = 0; index < this._cols; ++index)
        {
          this._items[0, index] = items[index].dX;
          this._items[1, index] = items[index].dY;
          this._items[2, index] = items[index].dZ;
        }
      }
      this._isSquare = this._rows == this._cols;
    }

    public Matrix(int rows, int cols, bool isidentity)
    {
      if (rows < 1 || cols < 1)
        Messages.ThrowException("Matrix(int, int, bool)", "Cannot construct matrix smaller than 1x1!");
      this._rows = rows;
      this._cols = cols;
      this._items = new double[rows, cols];
      for (int index1 = 0; index1 < rows; ++index1)
      {
        for (int index2 = 0; index2 < cols; ++index2)
          this._items[index1, index2] = (index1 + index2 & 1) == 0 & isidentity ? 1.0 : 0.0;
      }
      this._isSquare = this._rows == this._cols;
    }

    public Matrix(Quaternion quat)
    {
      this._rows = 3;
      this._cols = 3;
      this._isSquare = this._rows == this._cols;
      this._items = new double[3, 3]
      {
        {
          1.0 - 2.0 * (double) quat[1] * (double) quat[1] - 2.0 * (double) quat[2] * (double) quat[2],
          2.0 * (double) quat[0] * (double) quat[1] - 2.0 * (double) quat[3] * (double) quat[2],
          2.0 * (double) quat[0] * (double) quat[2] + 2.0 * (double) quat[3] * (double) quat[1]
        },
        {
          2.0 * (double) quat[0] * (double) quat[1] + 2.0 * (double) quat[3] * (double) quat[2],
          1.0 - 2.0 * (double) quat[0] * (double) quat[0] - 2.0 * (double) quat[2] * (double) quat[2],
          2.0 * (double) quat[1] * (double) quat[2] - 2.0 * (double) quat[3] * (double) quat[0]
        },
        {
          2.0 * (double) quat[0] * (double) quat[2] - 2.0 * (double) quat[3] * (double) quat[1],
          2.0 * (double) quat[1] * (double) quat[2] + 2.0 * (double) quat[3] * (double) quat[0],
          1.0 - 2.0 * (double) quat[0] * (double) quat[0] - 2.0 * (double) quat[1] * (double) quat[1]
        }
      };
    }

    public Matrix(Matrix mtx, int nrows, int ncols)
      : this(nrows, ncols, true)
    {
      for (int index1 = 0; index1 < nrows; ++index1)
      {
        for (int index2 = 0; index2 < ncols; ++index2)
        {
          if (index1 < mtx._rows && index2 < mtx._cols)
            this._items[index1, index2] = mtx._items[index1, index2];
        }
      }
    }

    public static Matrix operator +(Matrix a, Matrix b)
    {
      if (a == null || b == null)
        Messages.ThrowException("Matrix +(Matrix, Matrix)", "Cannot add to a null instance!");
      if (a._rows != b._rows || a._cols != b._cols)
        Messages.ThrowException("Matrix +(Matrix, Matrix)", "Cannot add two matrices of different sizes.");
      Matrix matrix = new Matrix(a._rows, a._cols);
      for (int i = 0; i < a._rows; ++i)
      {
        for (int j = 0; j < a._cols; ++j)
          matrix[i, j] = a[i, j] + b[i, j];
      }
      return matrix;
    }

    public static Matrix operator +(Matrix a) => a + a;

    public static Matrix operator -(Matrix a, Matrix b)
    {
      if (a == null || b == null)
        Messages.ThrowException("Matrix -(Matrix, Matrix)", "Cannot negate by a null instance!");
      if (a._rows != b._rows || a._cols != b._cols)
        Messages.ThrowException("Matrix -(Matrix, Matrix)", "Cannot negate two matrices of different sizes.");
      Matrix matrix = new Matrix(a._rows, a._cols);
      for (int i = 0; i < a._rows; ++i)
      {
        for (int j = 0; j < a._cols; ++j)
          matrix[i, j] = a[i, j] - b[i, j];
      }
      return matrix;
    }

    public static Matrix operator *(Matrix a, Matrix b)
    {
      if (a == null || b == null)
        Messages.ThrowException("Matrix *(Matrix, Matrix)", "Cannot multiply by a null instance!");
      if (a._cols != b._rows)
        Messages.ThrowException("Matrix *(Matrix, Matrix)", "Cannot multiply two matrices of different inner-dimensions.");
      Matrix matrix = new Matrix(a._rows, b._cols);
      for (int i = 0; i < a._rows; ++i)
      {
        for (int j = 0; j < b._cols; ++j)
        {
          double num = 0.0;
          for (int index = 0; index < b._rows; ++index)
            num += a[i, index] * b[index, j];
          matrix[i, j] = num;
        }
      }
      return matrix;
    }

    public static Matrix operator *(double a, Matrix b)
    {
      if (b == null)
        Messages.ThrowException("Matrix *(double, Matrix)", "Cannot multiply by a null instance!");
      for (int i = 0; i < b._rows; ++i)
      {
        for (int j = 0; j < b._cols; ++j)
          b[i, j] *= a;
      }
      return b;
    }

    public static Matrix operator *(Matrix a, double b) => b * a;

    public static Matrix operator /(Matrix a, double b)
    {
      if (a == null)
        Messages.ThrowException("Matrix /(Matrix, double)", "Cannot divide with a null instance!");
      for (int i = 0; i < a._rows; ++i)
      {
        for (int j = 0; j < a._cols; ++j)
          a[i, j] /= b;
      }
      return a;
    }

    public static Matrix operator ^(Matrix a, int b)
    {
      if (a == null)
        Messages.ThrowException("Matrix ^(Matrix, int)", "Cannot power with a null instance!");
      if (!a._isSquare)
        Messages.ThrowException("Matrix ^(Matrix, int)", "Cannot multiply a non-square matrix by itself!");
      Matrix matrix = a;
      for (int index = 2; index < b; ++index)
        matrix *= a;
      return matrix;
    }

    public bool ChangeVal(int row, int col, double newval)
    {
      if (row >= this._rows || row < 0 || col >= this._cols || col < 0)
        Messages.ThrowException("Matrix.ChangeVal(int, int, double)", "Cannot change value of an element outside the matrix!");
      this._items[row, col] = newval;
      return true;
    }

    public double Dot(Matrix a)
    {
      if (a._rows != this._cols || a._cols != 1 || this._rows != 1)
        Messages.ThrowException("Matrix.Dot(Matrix)", "Cannot dot two matrices with different internal dimensions or outer dimensions other than 1!");
      double num = 0.0;
      for (int index = 0; index < this._rows; ++index)
        num += this._items[index, 0] * a._items[0, index];
      return num;
    }

    public double Minor(int row, int col)
    {
      if (row >= this._rows || row < 0 || col >= this._cols || col < 0)
        Messages.ThrowException("Matrix.Minor(int, int)", "Cannot find the minor of an element with position outside the bounds of the matrix!");
      if (!this._isSquare)
        Messages.ThrowException("Matrix.Minor(int, int)", "Cannot find the minor of an element in a non-square matrix!");
      if (this._rows < 2 || this._cols < 2)
        Messages.ThrowException("Matrix.Minor(int, int)", "Cannot find a minor in a matrix with a dimension less than 2!");
      Matrix matrix = new Matrix(this._rows - 1, this._cols - 1);
      for (int index1 = 0; index1 < this._rows; ++index1)
      {
        if (index1 != row)
        {
          for (int index2 = 0; index2 < this._cols; ++index2)
          {
            if (index2 != col)
              matrix[index1 > row ? index1 - 1 : index1, index2 > col ? index2 - 1 : index2] = this._items[index1, index2];
          }
        }
      }
      return matrix.Det();
    }

    public double Det()
    {
      if (!this._isSquare)
        Messages.ThrowException("Matrix.Det()", "Cannot find determinant of non-square matrix!");
      double num = 0.0;
      if (this._rows == 1)
        return this._items[0, 0];
      if (this._rows == 2)
        return this._items[0, 0] * this._items[1, 1] - this._items[0, 1] * this._items[1, 0];
      for (int index1 = 0; index1 < this._rows; ++index1)
      {
        if (this._items[0, index1] != 0.0)
        {
          Matrix matrix = new Matrix(this._rows - 1, this._rows - 1);
          for (int index2 = 1; index2 < this._rows; ++index2)
          {
            for (int index3 = 0; index3 < this._rows; ++index3)
            {
              if (index3 != index1)
                matrix[index2 - 1, index3 > index1 ? index3 - 1 : index3] = this._items[index2, index3];
            }
          }
          num += ((index1 & 1) == 0 ? 1.0 : -1.0) * this._items[0, index1] * matrix.Det();
        }
      }
      return num;
    }

    public Matrix Adj()
    {
      Matrix matrix = new Matrix(this._rows, this._cols);
      for (int i = 0; i < this._rows; ++i)
      {
        for (int j = 0; j < this._cols; ++j)
          matrix[i, j] = ((i + j & 1) == 0 ? 1.0 : -1.0) * this._items[j, i];
      }
      return matrix;
    }

    public Matrix Inverse()
    {
      if (!this._isSquare)
        Messages.ThrowException("Matrix.Inverse()", "Cannot find determinant of non-square matrix!");
      if (this._rows == 1)
        return new Matrix(new double[1, 1]
        {
          {
            1.0 / this._items[0, 0]
          }
        });
      double num = this.Det();
      if (num == 0.0)
        return (Matrix) null;
      Matrix matrix = new Matrix(this._rows, this._cols);
      for (int index1 = 0; index1 < this._rows; ++index1)
      {
        for (int index2 = 0; index2 < this._cols; ++index2)
          matrix[index1, index2] = ((index1 + index2 & 1) == 0 ? 1.0 : -1.0) * this.Minor(index1, index2) / num;
      }
      return matrix;
    }

    public new string ToString()
    {
      string str1 = "[";
      for (int index1 = 0; index1 < this._rows; ++index1)
      {
        string str2 = index1 > 0 ? "/" : "";
        float single = Convert.ToSingle(this._items[index1, 0]);
        string str3 = single.ToString();
        string str4 = str2 + "[" + str3;
        for (int index2 = 1; index2 < this._cols; ++index2)
        {
          string str5 = str4;
          single = Convert.ToSingle(this._items[index1, index2]);
          string str6 = single.ToString();
          str4 = str5 + ", " + str6;
        }
        str1 = str1 + str4 + "]";
      }
      return str1 + "]";
    }
  }
}
