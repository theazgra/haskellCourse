int tmp;
for (long resRow = 0; resRow < result.rows(); resRow++)
{
  for (long resCol = 0; resCol < result.cols(); resCol++)
  {
    tmp = 0;
    for (long aCol = 0; aCol < A.cols(); aCol++)
    {
      tmp += (A(resRow, aCol) * B(aCol, resCol));
    }
    result(resRow, resCol) = tmp;
  }
}