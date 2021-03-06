// $Id: side.h,v 1.1.1.1 2008/03/19 06:41:21 gdiso Exp $

// The libMesh Finite Element Library.
// Copyright (C) 2002-2007  Benjamin S. Kirk, John W. Peterson

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



#ifndef __side_h__
#define __side_h__

// C++ includes

// Local includes
#include "genius_common.h"
#include "elem.h"

// Forward declarations
class Point;
class Node;


/**
 * This defines the \p Side class.  A \p Side is basically a proxy
 * (or stand-in replacement) class for an element's side.  It acts
 * like a standard \p Elem, but allocates no additional memory for
 * storing connectivity.  Instead, its nodes are mapped directly from
 * the parent element (the element for which the side is created).
 * Similarly, you cannot access the neighbors of a side since it
 * does not store any.
 *
 * \author  Benjamin S. Kirk
 * \date    $Date: 2008/03/19 06:41:21 $
 * \version $Revision: 1.1.1.1 $
 */

// ------------------------------------------------------------
//Side class definition
template <class SideType, class ParentType>
class Side : public SideType
{
 public:

  /**
   * Constructor.  Creates a side from an element.
   */
  Side (const Elem* parent,
	const unsigned int side) :
    SideType(0,0,const_cast<Elem*>(parent)), // Allocate no storage for nodes or neighbors!
    _side_number(side)
  {
    assert (parent != NULL);
    // may not be true when building infinite element sides
    // assert (_side_number < this->parent()->n_sides());
    assert ((this->dim()+1) == this->parent()->dim());
  }

  /**
   * @returns the \p Point associated with local \p Node \p i.
   */
  virtual const Point & point (const unsigned int i) const
  {
    assert (i < this->n_nodes());
    return this->parent()->point (ParentType::side_nodes_map[_side_number][i]);
  }

  /**
   * @returns the \p Point associated with local \p Node \p i
   * as a writeable reference.
   */
  virtual Point & point (const unsigned int i)
  {
    assert (i < this->n_nodes());
    return this->parent()->point (ParentType::side_nodes_map[_side_number][i]);
  }

  /**
   * @returns the global id number of local \p Node \p i.
   */
  virtual unsigned int node (const unsigned int i) const
  {
    assert (i < this->n_nodes());
    return this->parent()->node (ParentType::side_nodes_map[_side_number][i]);
  }

  /**
   * @returns the pointer to local \p Node \p i.
   */
  virtual Node* get_node (const unsigned int i) const
  {
    assert (i < this->n_nodes());
    return this->parent()->get_node (ParentType::side_nodes_map[_side_number][i]);
  }

  /**
   * @returns the pointer to local \p Node \p i as a writeable reference.
   */
  virtual Node* & set_node (const unsigned int i)
  {
    assert (i < this->n_nodes());
    return this->parent()->set_node (ParentType::side_nodes_map[_side_number][i]);
  }

  /**
   * @returns 0. Sides effectively do not have sides, so
   * don't even ask!
   */
  virtual unsigned int n_sides () const { return 0; }


 private:


  /**
   * The side on the parent element
   */
  const unsigned int _side_number;
};



/**
 * This defines the \p SideEdge class.  Like \p Side, \p SideEdge is basically
 * a proxy (or stand-in replacement) class, this time for an element's edge.
 * It acts like a standard \p Elem, but allocates no additional memory for
 * storing connectivity.  Instead, its nodes are mapped directly from the
 * parent element (the element for which the side is created).  Similarly, you
 * cannot access the neighbors of a side since it does not store any.
 *
 * \author  Roy H. Stogner
 * \date    $Date: 2008/03/19 06:41:21 $
 * \version $Revision: 1.1.1.1 $
 */

// ------------------------------------------------------------
//SideEdge class definition
template <class EdgeType, class ParentType>
class SideEdge : public EdgeType
{
 public:

  /**
   * Constructor.  Creates a side from an element.
   */
  SideEdge (const Elem* parent,
	    const unsigned int edge) :
    EdgeType(0,0,const_cast<Elem*>(parent)), // Allocate no storage for nodes or neighbors!
    _edge_number(edge)
  {
    assert (parent != NULL);
    assert (_edge_number < this->parent()->n_edges());
    assert (this->dim() == 1);
  }

  /**
   * @returns the \p Point associated with local \p Node \p i.
   */
  virtual const Point & point (const unsigned int i) const
  {
    assert (i < this->n_nodes());
    return this->parent()->point (ParentType::edge_nodes_map[_edge_number][i]);
  }

  /**
   * @returns the \p Point associated with local \p Node \p i
   * as a writeable reference.
   */
  virtual Point & point (const unsigned int i)
  {
    assert (i < this->n_nodes());
    return this->parent()->point (ParentType::edge_nodes_map[_edge_number][i]);
  }

  /**
   * @returns the global id number of local \p Node \p i.
   */
  virtual unsigned int node (const unsigned int i) const
  {
    assert (i < this->n_nodes());
    return this->parent()->node (ParentType::edge_nodes_map[_edge_number][i]);
  }

  /**
   * @returns the pointer to local \p Node \p i.
   */
  virtual Node* get_node (const unsigned int i) const
  {
    assert (i < this->n_nodes());
    return this->parent()->get_node (ParentType::edge_nodes_map[_edge_number][i]);
  }

  /**
   * @returns the pointer to local \p Node \p i as a writeable reference.
   */
  virtual Node* & set_node (const unsigned int i)
  {
    assert (i < this->n_nodes());
    return this->parent()->set_node (ParentType::edge_nodes_map[_edge_number][i]);
  }

  /**
   * @returns 0. Sides effectively do not have sides, so
   * don't even ask!
   */
  virtual unsigned int n_sides () const { return 0; }


 private:


  /**
   * The side on the parent element
   */
  const unsigned int _edge_number;
};


#endif // end #ifndef __side_h__
