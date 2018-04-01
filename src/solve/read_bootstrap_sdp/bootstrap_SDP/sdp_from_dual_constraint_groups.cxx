#include "Dual_Constraint_Group.hxx"
#include "../../SDP.hxx"

// Collect a bunch of Dual_Constraint_Group's and a dual objective
// function into an SDP.

SDP sdp_from_dual_constraint_groups(
  const Vector &dual_objective, const Real &objective_const,
  const std::vector<Dual_Constraint_Group> &dualConstraintGroups)
{
  SDP sdp;
  sdp.dual_objective = dual_objective;
  sdp.objective_const = objective_const;

  for(std::vector<Dual_Constraint_Group>::const_iterator g
      = dualConstraintGroups.begin();
      g != dualConstraintGroups.end(); g++)
    {
      sdp.dimensions.push_back(g->dim);
      sdp.degrees.push_back(g->degree);

      // sdp.primal_objective is the concatenation of the
      // g.constraintConstants
      sdp.primal_objective.insert(sdp.primal_objective.end(),
                                  g->constraintConstants.begin(),
                                  g->constraintConstants.end());
    }
  sdp.free_var_matrix
    = Matrix(sdp.primal_objective.size(), sdp.dual_objective.size());

  int p = 0;
  // Each g corresponds to an index 0 <= j < J (not used explicitly here)
  for(std::vector<Dual_Constraint_Group>::const_iterator g
      = dualConstraintGroups.begin();
      g != dualConstraintGroups.end(); g++)
    {
      // sdp.bilinear_bases is the concatenation of the g.bilinearBases.
      // The matrix Y is a BlockDiagonalMatrix built from the
      // concatenation of the blocks for each individual
      // Dual_Constraint_Group.  sdp.blocks[j] = {b1, b2, ... } contains
      // the indices for the blocks of Y corresponding to the j-th
      // group.
      std::vector<int> blocks;
      for(std::vector<Matrix>::const_iterator b = g->bilinearBases.begin();
          b != g->bilinearBases.end(); b++)
        {
          // Ensure that each bilinearBasis is sampled the correct number
          // of times
          assert(b->cols == g->degree + 1);
          blocks.push_back(sdp.bilinear_bases.size());
          sdp.bilinear_bases.push_back(*b);
        }
      sdp.blocks.push_back(blocks);

      // sdp.free_var_matrix is the block-wise concatenation of the
      // g.constraintMatrix's
      for(int k = 0; k < g->constraintMatrix.rows; k++, p++)
        for(int n = 0; n < g->constraintMatrix.cols; n++)
          sdp.free_var_matrix.elt(p, n) = g->constraintMatrix.elt(k, n);
    }
  assert(p == static_cast<int>(sdp.primal_objective.size()));

  sdp.initialize_constraint_indices();
  return sdp;
}