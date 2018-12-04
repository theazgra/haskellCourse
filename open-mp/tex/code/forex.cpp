for (long resRow = 0; resRow < result.rows(); resRow++)
{
  for (long resCol = 0; resCol < result.cols(); resCol++)
  {
    for (long aCol = 0; aCol < A.cols(); aCol++)
    {
      result(resRow, resCol) += 
        (A(resRow, aCol) * B(aCol, resCol));
    }
  }
}