/*****************************************************************************/
/*                                                                           */
/*              8888888         88888888         88888888                    */
/*            8                8                8                            */
/*           8                 8                8                            */
/*           8                  88888888         88888888                    */
/*           8      8888                8                8                   */
/*            8       8                 8                8                   */
/*              888888         888888888        888888888                    */
/*                                                                           */
/*       A Two-Dimensional General Purpose Semiconductor Simulator.          */
/*                                                                           */
/*  GSS material database Version 0.4                                        */
/*  Last update: Feb 17, 2006                                                */
/*                                                                           */
/*  Gong Ding                                                                */
/*  gdiso@ustc.edu                                                           */
/*  NINT, No.69 P.O.Box, Xi'an City, China                                   */
/*                                                                           */
/*****************************************************************************/
//
// Material Type: P-type Poly Silicon


#include "PMI.h"

class GSS_PPolySi_BasicParameter : public PMIC_BasicParameter
{
private:

  PetscScalar PERMITTI;      // The relative dielectric permittivity.
  PetscScalar PERMEABI;      // The relative megnetic permeability.
  PetscScalar AFFINITY;      // The electron affinity for the material.
  PetscScalar DENSITY;       // Specific mass density for the material.
  PetscScalar IONDENSITY;    // Specific ion (free electron) density for the material.
  PetscScalar CONDUCTANCE;   // Specific conductance for the material.

  void   Basic_Init()
  {
    PERMITTI = 1.1800000e+01;
    PERMEABI = 1.0;//---not magnetism material,assigned its permeability equals 1.
    AFFINITY = 5.250000e+00*eV;
    DENSITY  = 2.320000e-03*kg*std::pow(cm,-3);
    IONDENSITY  = 1.000000e+21*std::pow(cm,-3);
    CONDUCTANCE = 1.0/(5.0e-3*V/A*cm); //FIXME, not so accurate

#ifdef __CALIBRATE__
    parameter_map.insert(para_item("PERMITTI",    PARA("PERMITTI",    "The relative dielectric permittivity", "-", 1.0, &PERMITTI)) );
    parameter_map.insert(para_item("PERMEABI",  PARA("PERMEABI",  "The relative megnetic permeability", "-", 1.0, &PERMEABI)) );
    parameter_map.insert(para_item("AFFINITY", PARA("AFFINITY", "The electron affinity for the material", "eV", eV, &AFFINITY)) );
    parameter_map.insert(para_item("DENSITY", PARA("DENSITY", "Specific mass density for the material", "kg*cm^-3", kg*std::pow(cm,-3), &DENSITY)) );
    parameter_map.insert(para_item("IONDENSITY", PARA("IONDENSITY", "Specific ion density for the material", "cm^-3", std::pow(cm,-3), &IONDENSITY)) );
    parameter_map.insert(para_item("CONDUCTANCE", PARA("CONDUCTANCE", "Specific conductance for the material", "(ohmic*m)^-1", A/V/m, &CONDUCTANCE)) );
#endif
  }
public:

  PetscScalar Density       (const PetscScalar &Tl) const { return DENSITY;     }
  PetscScalar IonDensity    (const PetscScalar &Tl) const { return IONDENSITY;  }
  PetscScalar Permittivity  ()                      const { return PERMITTI;    }
  PetscScalar Permeability  ()                      const { return PERMEABI;    }
  PetscScalar Affinity      (const PetscScalar &Tl) const { return AFFINITY;    }
  PetscScalar Conductance   ()                      const { return CONDUCTANCE; }
  PetscScalar ThermalVn     (const PetscScalar &Tl) const { return 1e6*cm/s;    }

  void atom_fraction(std::vector<std::string> &atoms, std::vector<double> & fraction) const
  {
    atoms.push_back("Si");//Silicon
    fraction.push_back(0.999);
    atoms.push_back("B");//Boron
    fraction.push_back(0.001);
  }

public:
  GSS_PPolySi_BasicParameter(const PMIC_Environment &env):PMIC_BasicParameter(env)
  {
    Basic_Init();
  }
  ~GSS_PPolySi_BasicParameter()
  {
  }
}
;

extern "C"
{
  DLL_EXPORT_DECLARE  PMIC_BasicParameter* PMIC_PPolySi_BasicParameter_Default (const PMIC_Environment& env)
  {
    return new GSS_PPolySi_BasicParameter(env);
  }
}
