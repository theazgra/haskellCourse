template <typename T>
class ArrayMatrix : public BasicMatrix<T>
{
  T& operator()(long row, long col)
  {
    return data[((row * this->colCount) + col)];
  }

  const T& operator()(long row,long col) const
  {
    return data[((row * this->colCount) + col)];
  }
};
