template<typename T>
class BasicMatrix
{
protected:
  long rowCount;
  long colCount;
public:
  virtual T& operator()(long row, long col) = 0;
  virtual const T& operator()(long row, long col) const = 0;
};
