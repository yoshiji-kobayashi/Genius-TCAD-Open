/********************************************************************************/
/*     888888    888888888   88     888  88888   888      888    88888888       */
/*   8       8   8           8 8     8     8      8        8    8               */
/*  8            8           8  8    8     8      8        8    8               */
/*  8            888888888   8   8   8     8      8        8     8888888        */
/*  8      8888  8           8    8  8     8      8        8            8       */
/*   8       8   8           8     8 8     8      8        8            8       */
/*     888888    888888888  888     88   88888     88888888     88888888        */
/*                                                                              */
/*       A Three-Dimensional General Purpose Semiconductor Simulator.           */
/*                                                                              */
/*                                                                              */
/*  Copyright (C) 2007-2008                                                     */
/*  Cogenda Pte Ltd                                                             */
/*                                                                              */
/*  Please contact Cogenda Pte Ltd for license information                      */
/*                                                                              */
/*  Author: Gong Ding   gdiso@ustc.edu                                          */
/*                                                                              */
/********************************************************************************/


#ifndef __hall_h__
#define __hall_h__

#include "ddm_solver.h"
#include "solver_specify.h"
#include "boundary_info.h"


/**
 * Solve the Level 1 Drift-Diffusion Model equations!
 */
class HallSolver : public DDMSolverBase
{
public:
  HallSolver(SimulationSystem & system): DDMSolverBase(system)
  {system.record_active_solver(this->solver_type());}


  ~HallSolver()
  {}

  /**
   * @return the solver type
   */
  virtual SolverSpecify::SolverType solver_type() const
  {return SolverSpecify::HALLDDML1;}

  /**
   * virtual function, create the solver
   */
  virtual int create_solver();

  /**
   * virtual function, do the solve process
   */
  virtual int solve();

  /**
   * do pre-process before each solve action
   */
  virtual int pre_solve_process(bool load_solution=true);

  /**
   * do post-process after each solve action
   */
  virtual int post_solve_process();

  /**
   * load previous state into solution vector
   */
  virtual int diverged_recovery();

  /**
   * @return node's dof for each region.
   */
  virtual unsigned int node_dofs(const SimulationRegion * region) const
  {
    switch(region->type())
    {
      case SemiconductorRegion : return 3; //semiconductor node has 3 dof
      case InsulatorRegion     : return 1; //insulator node has 1 dof
      case ElectrodeRegion     : return 1; //conductor node has 1 dof
      case MetalRegion         : return 1; //resistance node has 1 dof
      default : return 0;
    }
  }

  /**
   * @return the extra dofs of each boundary condition.
   */
  virtual unsigned int bc_dofs(const BoundaryCondition * bc) const
  {
    switch (bc->bc_type())
    {
      case OhmicContact      :
      case SchottkyContact   :
      case SimpleGateContact :
      case GateContact       :
      case SolderPad         :
      case ChargedContact    :
      case InterConnect      : return 1; // the above bcs has one extra equation
      default                : return 0; // others, no extra bc equation
    }
  }


  /**
   * @return the matrix bandwidth of each boundary condition which owns extra dofs
   */
  virtual unsigned int bc_bandwidth(const BoundaryCondition * bc) const
  {
    switch (bc->bc_type())
    {
      case OhmicContact      :
      case SchottkyContact   :
      case SimpleGateContact :
      case SolderPad         :
      case GateContact       :  return 2; // the above bc equation has a max bandwidth of 2
      case ChargedContact    :  return 1; // this bc equation has a bandwidth of 1
      case InterConnect      :  return bc->inter_connect().size()+1;
      default                :  return 0;  // others, bandwidth is zero
    }
  }


  /**
   * @return the bandwidth contributed by each boundary node
   */
  virtual unsigned int bc_node_dofs(const BoundaryCondition * bc) const
  {
    switch (bc->bc_type())
    {
      case OhmicContact      : return 3; // ohmic electrode current
      case SchottkyContact   : return 3; // displacement current
      case SimpleGateContact : return 1; // displacement current
      case GateContact       : return 1; // displacement current
      case SolderPad         : return 1; // conductance current
      case ChargedContact    : return 1; // for electrostatic Gauss's law
      default: return 0;
    }
  }


  /**
   * indicates if PDE involves all neighbor elements.
   * when it is true, the matrix bandwidth will include all the nodes belongs to neighbor elements, i.e. DDM solver
   * when it is false, only neighbor nodes (link local node by edge) are appeared in matrix bandwidth, i.e. poisson solver.
   */
  virtual bool all_neighbor_elements_involved(const SimulationRegion * region) const
  {
    switch(region->type())
    {
      case SemiconductorRegion : return true;
      case InsulatorRegion     : return false;
      case ElectrodeRegion     : return false;
      case MetalRegion         : return false;
      default : return false;
    }
  }

  /**
   * wrap function for evaluating the residual of function f at x
   */
  virtual void build_petsc_sens_residual(Vec x, Vec r);

  /**
   * wrap function for evaluating the Jacobian J of function f at x
   */
  virtual void build_petsc_sens_jacobian(Vec x, Mat *jac, Mat *pc);

  /**
   * function for line search post check. do Newton damping here
   */
  virtual void sens_line_search_post_check(Vec x, Vec y, Vec w, PetscBool *changed_y, PetscBool *changed_w)
  {
    switch( SolverSpecify::Damping )
    {
      case SolverSpecify::DampingPotential : potential_damping(x, y, w, changed_y, changed_w); break;
      case SolverSpecify::DampingBankRose  : bank_rose_damping(x, y, w, changed_y, changed_w); break;
      case SolverSpecify::DampingNo        : positive_density_damping(x, y, w, changed_y, changed_w); break;
      default: positive_density_damping(x, y, w, changed_y, changed_w);
    }
    FVM_NonlinearSolver::sens_line_search_post_check(x, y, w, changed_y, changed_w);
  }

  /**
   * test if BDF2 can be used for next time step
   */
  virtual bool BDF2_positive_defined() const;

  /**
   * compute the norm of local truncate error (LTE)
   */
  virtual PetscReal LTE_norm();

  /**
   * check carrier density after projection
   */
  virtual void projection_positive_density_check(Vec x, Vec xo);

  /**
   * compute the abs and relative error norm of the solution
   */
  virtual void error_norm();

  /**
   * function for convergence test of pseudo time step method
   */
  virtual bool pseudo_time_step_convergence_test() { return true; }

private:

  /**
   * Potential Newton damping scheme
   */
  void potential_damping(Vec x, Vec y, Vec w, PetscBool *changed_y, PetscBool *changed_w);

  /**
   * Bank-Rose Newton damping scheme
   */
  void bank_rose_damping(Vec x, Vec y, Vec w, PetscBool *changed_y, PetscBool *changed_w);

  /**
   * Positive carrier density Newton damping scheme
   */
  void positive_density_damping(Vec x, Vec y, Vec w, PetscBool *changed_y, PetscBool *changed_w);
};


#endif // #define __hall_h__
