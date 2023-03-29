// Fill out your copyright notice in the Description page of Project Settings.


#include "PBStaticPermutation.h"

void UPBStaticPermutation::SetStaticBool(UMaterialInstance* Mat, FName Name, bool Value)
{
    if (!Mat)
        return;

    FStaticParameterSet Params;
    Mat->GetStaticParameterValues(Params);
    for (auto& Param : Params.StaticSwitchParameters)
    {
        if (Param.ParameterInfo.Name == Name)
        {
            Param.Value = Value;
            break;
        }
    }

    Mat->UpdateStaticPermutation(Params);
}