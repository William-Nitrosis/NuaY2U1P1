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
	bool sprintPressed = false;
	bool crouchPressed = false;
	float forwardValue;
	float strafeValue;
	float mouseYValue;
	float mouseXValue;
	FRotator rawInput;
	FRotator rawInputX;
	float sens;
	bool firstPerson = false;

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
	void CrouchKeyPressed();
	void CrouchKeyReleased();
	void ToggleCameraPressed();
	void ToggleCameraReleased();

	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* ForwardArrow;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* FP_Camera;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* TP_Camera;
	

};