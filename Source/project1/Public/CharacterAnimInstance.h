// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT1_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isJumpingDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isJumpingUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float movementAxisForward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float movementAxisSide;
};
