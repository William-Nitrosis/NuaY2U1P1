// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "playerCharacter.generated.h"


UCLASS()
class PROJECT1_API AplayerCharacter : public ACharacter {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AplayerCharacter();
	FVector movementDirection;
	FVector jumpRayStart;
	FVector jumpRayEnd;
	ECollisionChannel collisionChannel;
	bool sprintPressed = false;
	bool crouchPressed = false;
	bool jumpCastBool = false;
	float forwardValue;
	float strafeValue;
	float mouseYValue;
	float mouseXValue;
	FRotator rawInput;
	FRotator rawInputX;
	float sens;
	bool firstPerson = false;
	int jumpCount = 0;
	FKey currentSprintKey;
	FKey currentForwardKey;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Overrides, meta = (PinHiddenByDefault, InlineEditConditionToggle))
	uint32 bOverride_NumberWang : 1;

	/** Strength of motion blur, 0:off, should be renamed to intensity */
	UPROPERTY(interp, BlueprintReadWrite, Category = "Number Wang", meta = (ClampMin = "0.0", ClampMax = "50.0", Delta = "1.01", editcondition = "bOverride_NumberWang", DisplayName = "Amount"))
	float NumberWang1;

	UPROPERTY(interp, BlueprintReadWrite, Category = "Number Wang", meta = (ClampMin = "0", ClampMax = "10", Delta = "1", editcondition = "bOverride_NumberWang", DisplayName = "Amount"))
	int NumberWang2;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//virtual void OnConstruction(const FTransform & Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ActivateCam();
	void ForwardKeyPressed(float inputValue);
	void strafeKeyPressed(float inputValue);
	void MouseX(float inputValue);
	void MouseY(float inputValue);
	

	void SprintKeyPressed();
	void SprintKeyReleased();
	void JumpKeyPressed();
	void JumpKeyReleased();
	void CrouchKeyPressed();
	void CrouchKeyReleased();
	void ToggleCameraPressed();
	void ToggleCameraReleased();

	UPROPERTY(EditAnywhere)
		class UArrowComponent* ForwardArrow;

	UPROPERTY(EditAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
		class UCameraComponent* FP_Camera;

	UPROPERTY(EditAnywhere)
		class UCameraComponent* TP_Camera;

	UPROPERTY(EditAnywhere)
		float jumpPower = 500.f;

	UPROPERTY(EditAnywhere)
		int jumpLimit = 2;
	

};