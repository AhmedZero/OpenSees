#!/usr/bin/python
from itertools import product

EL = [
    "LinearIsotropic3D_EL",
]

YF = [
    "VonMises_YF",
    "DruckerPrager_YF",
]

PF = [
    "VonMises_PF",
]

IV_YF = [
    "BackStress<TensorLinearHardeningFunction>,VonMisesRadius<ScalarLinearHardeningFunction>",
    "BackStress<ArmstrongFrederickHardeningFunction>,VonMisesRadius<ScalarLinearHardeningFunction>",
]

IV_PF = [
    "BackStress<TensorLinearHardeningFunction>",
    "BackStress<ArmstrongFrederickHardeningFunction>",
]

template = """

createASDPlasticMaterial<
        {EL}, 
        {YF}<
            {IV_YF}
            >, 
        {PF}<
            {IV_PF}
            >
        > (instance_tag, yf_type, pf_type, el_type, iv_type, instance_pointers);


"""


with open("ASD_material_definitions.cpp","w") as fid:
    for el, yf, pf, iv_yf, iv_pf in product(EL, YF, PF, IV_YF, IV_PF):
        fid.write(template.format(EL=el, YF=yf, PF=pf, IV_YF=iv_yf, IV_PF=iv_pf))