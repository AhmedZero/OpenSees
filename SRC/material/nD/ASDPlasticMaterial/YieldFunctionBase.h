/* ****************************************************************** **
**    OpenSees - Open System for Earthquake Engineering Simulation    **
**          Pacific Earthquake Engineering Research Center            **
**                                                                    **
**                                                                    **
** (C) Copyright 1999, The Regents of the University of California    **
** All Rights Reserved.                                               **
**                                                                    **
** Commercial use of this program without express permission of the   **
** University of California, Berkeley, is strictly prohibited.  See   **
** file 'COPYRIGHT'  in main directory for information on usage and   **
** redistribution,  and for a DISCLAIMER OF ALL WARRANTIES.           **
**                                                                    **
** Developed by:                                                      **
**   Frank McKenna (fmckenna@ce.berkeley.edu)                         **
**   Gregory L. Fenves (fenves@ce.berkeley.edu)                       **
**   Filip C. Filippou (filippou@ce.berkeley.edu)                     **
**                                                                    **
** ****************************************************************** */
                                                                        
// Original implementation: José Abell (UANDES), Massimo Petracca (ASDEA)
//
// ASDPlasticMaterial
//
// Fully general templated material class for plasticity modeling

#ifndef YieldFunctionBase_H
#define YieldFunctionBase_H

#include "EigenAPI.h"
// #include "EvolvingVariable.h"
#include <Channel.h>
// #include "utuple_storage.h"
#include "std_tuple_concat.h"

// Helper template to check if a class has a parameters_t type alias
template <typename T, typename = void>
struct yf_has_parameters_t : std::false_type {};

template <typename T>
struct yf_has_parameters_t<T, typename std::enable_if<!std::is_same<typename T::parameters_t, void>::value>::type> : std::true_type {};

// Helper template to check if a class has a internal_variables_t type alias
template <typename T, typename = void>
struct yf_has_internal_variables_t : std::false_type {};

template <typename T>
struct yf_has_internal_variables_t<T, typename std::enable_if<!std::is_same<typename T::internal_variables_t, void>::value>::type> : std::true_type {};


template <class T>
class YieldFunctionBase
{
public:
    YieldFunctionBase() { 
        static_assert(yf_has_parameters_t<T>::value, "Derived class must have a 'parameters_t' type alias.");
        static_assert(yf_has_internal_variables_t<T>::value, "Derived class must have a 'internal_variables_t' type alias.");
    }

    template <typename IVStorageType, typename ParameterStorageType>
    double operator()( const VoigtVector& sigma, 
    	const IVStorageType& internal_variables_storage,
        const ParameterStorageType& parameters_storage) const
    {
        return static_cast<T*>(this)->operator()(sigma, internal_variables_storage, parameters_storage);
    }

    template <typename IVStorageType, typename ParameterStorageType>
    const VoigtVector& df_dsigma_ij(const VoigtVector& sigma,
    	const IVStorageType& internal_variables_storage,
        const ParameterStorageType& parameters_storage)
    {
        return static_cast<T*>(this)->df_dsigma_ij(sigma, internal_variables_storage, parameters_storage);
    }

    template <typename IVStorageType, typename ParameterStorageType>
    double xi_star_h_star(const VoigtVector& depsilon, 
        const VoigtVector& m, 
        const VoigtVector& sigma,
        const IVStorageType& internal_variables_storage,
        const ParameterStorageType& parameters_storage)
    {
        return static_cast<T*>(this)->df_dxi_star_h_star(depsilon, m , sigma, internal_variables_storage, parameters_storage);
    }

    inline const char* getName() const { return static_cast<T*>(this)->NAME; }

};

#endif