﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MP_Player.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UMP_Player : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MP_CPP_API IMP_Player
{
	GENERATED_BODY()

	
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	USkeletalMeshComponent* GetSkeletalMeshComponent()const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GrantArmor(float ArmorAmount);

	
};
