// @HEADER
// ***********************************************************************
//
//          PyTrilinos: Python Interfaces to Trilinos Packages
//                 Copyright (2014) Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia
// Corporation, the U.S. Government retains certain rights in this
// software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Questions? Contact William F. Spotz (wfspotz@sandia.gov)
//
// ***********************************************************************
// @HEADER

#ifndef EPETRA_NUMPYINTVECTOR_HPP
#define EPETRA_NUMPYINTVECTOR_HPP

#define NO_IMPORT_ARRAY
#include "numpy_include.hpp"

#include "PyTrilinos_PythonException.hpp"
#ifdef HAVE_INTTYPES_H
#undef HAVE_INTTYPES_H
#endif
#ifdef HAVE_STDINT_H
#undef HAVE_STDINT_H
#endif
#include "Epetra_SerialComm.h"
#include "Epetra_BlockMap.h"
#include "Epetra_Map.h"
#include "Epetra_IntVector.h"

namespace PyTrilinos
{

class Epetra_NumPyIntVector : public Epetra_IntVector
{
public:

  // Constructors
  Epetra_NumPyIntVector(const Epetra_BlockMap  & blockMap, bool zeroOut=true);
  Epetra_NumPyIntVector(const Epetra_IntVector & source);
  Epetra_NumPyIntVector(const Epetra_BlockMap  & blockMap, PyObject * pyObject);
  Epetra_NumPyIntVector(PyObject * pyObject);

  // Destructor
  ~Epetra_NumPyIntVector();

  // Overridden Epetra_IntVector methods with more python-like signatures
  PyObject * ExtractCopy() const;
  PyObject * ExtractView() const;
  PyObject * Values() const;

  // Static cleanup function, to be called when python exceptions are
  // encountered
  static void cleanup();

private:

  // Private method thus not callable
  Epetra_NumPyIntVector();

  // This static private constant is the default communicator for any
  // Epetra_NumPyIntVector constructed without specifying an
  // Epetra_BlockMap
  static const Epetra_SerialComm defaultComm;

  // These private static pointers are for use with constructors only.
  // Outside of a constructor, they should be NULL.  They should only
  // be set by the static helper functions below, and when a
  // constructor sets any of these pointers, it should reset it to
  // NULL when done.
  static PyArrayObject * tmp_array;
  static Epetra_Map    * tmp_map;

  // Static helper functions.  These are intended to be called from
  // the constructors, specifically to compute arguments in the
  // Epetra_IntVector constructors called in the constructor
  // initialization lists.  They all assume that if tmp_array or
  // tmp_map is already set, they have been set by the same PyObject.
  static int        * getArray(PyObject *);
  static Epetra_Map & getMap(  PyObject *);
  static int        * getArray(const Epetra_BlockMap &, PyObject *);

  // Private data
  Epetra_BlockMap * map;
  PyArrayObject   * array;

};

}  // Namepace PyTrilinos

#endif
