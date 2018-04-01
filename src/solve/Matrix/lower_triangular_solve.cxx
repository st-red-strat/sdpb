#include "../Matrix.hxx"

// B := L^{-1} B, where L is lower-triangular and B is a matrix
// pointed to by b
void lower_triangular_solve(Matrix &L, Real *b, int bcols, int ldb)
{
  int dim = L.rows;
  assert(L.cols == dim);

  Rtrsm("Left", "Lower", "NoTranspose", "NonUnitDiagonal", dim, bcols, 1,
        &L.elements[0], dim, b, ldb);
}

// b := L^{-1} b, where L is lower-triangular
void lower_triangular_solve(Matrix &L, Vector &b)
{
  assert(static_cast<int>(b.size()) == L.rows);
  lower_triangular_solve(L, &b[0], 1, b.size());
}