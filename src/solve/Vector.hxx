//=======================================================================
// Copyright 2014-2015 David Simmons-Duffin.
// Distributed under the MIT License.
// (See accompanying file LICENSE or copy at
//  http://opensource.org/licenses/MIT)
//=======================================================================

#pragma once

#include "ostream.hxx"
#include "../types.hxx"

#include <algorithm>
#include <assert.h>
#include <iostream>

// a Vector is just an STL vector of Real's
using Vector = std::vector<Real>;

// The maximal absolute value of the components of v
inline Real max_abs_vector(const Vector &v)
{
  return abs(*std::max_element(v.begin(), v.end(), compare_abs));
}

// v := v + a*u
inline void add_scaled_vector(Vector &v, const Real &a, const Vector &u)
{
  assert(v.size() == u.size());

  for(size_t i = 0; i < v.size(); i++)
    {
      v[i] += a * u[i];
    }
}

inline Real dot_product(const Vector &u, const Vector v)
{
  Real result = 0;
  for(size_t i = 0; i < u.size(); i++)
    result += u[i] * v[i];
  return result;
}

// The component-wise product w = (v[0] u[0], ..., v[n] u[n])
// This routine is used only once, so we needn't worry about
// allocation.
inline Vector multiply_vectors(const Vector &u, const Vector &v)
{
  Vector w(u.size());
  for(size_t i = 0; i < w.size(); i++)
    {
      w[i] = u[i] * v[i];
    }
  return w;
}
