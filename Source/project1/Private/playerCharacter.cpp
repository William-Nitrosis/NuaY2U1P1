// Fill out your copyright notice in the Description page of Project Settings.

#include "playerCharacter.h"



// Sets default values
AplayerCharacter::AplayerCharacter() {
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// add comps
	ForwardArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ForwardArrow"));
	ForwardArrow->SetupAttachment(RootComponent);
	ForwardArrow->SetHiddenInGame(false);


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetHiddenInGame(false);


	TP_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("TP_Camera"));
	TP_Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);

	FP_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FP_Camera"));
	FP_Camera->SetupAttachment(RootComponent);
	

	this->GetCapsuleComponent()->SetVisibility(true);
	this->GetCapsuleComponent()->SetHiddenInGame(false);
}


/*void AplayerCharacter::OnConstruction(const FTransform & Transform) {
	ActivateCam();
}*/

void AplayerCharacter::ActivateCam() {
	TP_Camera->SetActive(!firstPerson);
	FP_Camera->SetActive(firstPerson);
}

// Called when the game starts or when spawned
void AplayerCharacter::BeginPlay() {
	Super::BeginPlay();
	movementDirection = FVector(0.0f, 0.0f, 0.0f);

	SpringArm->AddLocalOffset(FVector(0.f, 0.f, 50.f));
	SpringArm->TargetArmLength = 300.f;
	SpringArm->AddLocalRotation(FRotator(-10.f, 0.f, -10.f));

	//FP_Camera->AddRelativeLocation(FVector(0.f, 100.f, 0.f));
	//FP_Camera->SetWorldLocation(GetCapsuleComponent()->GetComponentLocation());
	FP_Camera->SetRelativeLocation(FVector(0.f, 0.f, 65.f));
	ActivateCam();

	sens = 1;
}

// Called every frame
void AplayerCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	rawInput = FRotator(mouseYValue, mouseXValue, 0.0f);
	rawInputX = FRotator(0.0f, mouseXValue, 0.0f);

	FRotator sensInput = (rawInput * sens);

	
	UE_LOG(LogTemp, Log, TEXT("%f"), FP_Camera->GetComponentRotation().Roll);

	if (sensInput.Pitch + FP_Camera->GetComponentRotation().Pitch > 85) {
		sensInput.Pitch = 0;
	}

	if (sensInput.Pitch + FP_Camera->GetComponentRotation().Pitch < -85) {
		sensInput.Pitch = 0;
	}

	FP_Camera->AddRelativeRotation(sensInput);
	FP_Camera->SetRelativeRotation(FRotator(FP_Camera->GetComponentRotation().Pitch, FP_Camera->GetComponentRotation().Yaw, 0));

	SpringArm->SetRelativeRotation(FRotator(FP_Camera->GetComponentRotation().Pitch, FP_Camera->GetComponentRotation().Yaw, 0));

	FRotator tempInputX = FP_Camera->GetComponentRotation();
	tempInputX.Roll = 0.f;
	tempInputX.Pitch = 0.f;

	ForwardArrow->SetRelativeRotation(tempInputX);

	FVector forward = ForwardArrow->GetForwardVector() * forwardValue;
	FVector strafe = ForwardArrow->GetRightVector() * strafeValue;

	FVector normal = forward + strafe;
	normal.Normalize();
	movementDirection = normal * GetCharacterMovement()->MaxWalkSpeed;

	AddMovementInput(movementDirection, 1.0f, false);
	
	//UE_LOG(LogTemp, Log, TEXT("%s"), *movementDirection.ToString());
}

void AplayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {

	PlayerInputComponent->BindAxis("ForwardKey", this, &AplayerCharacter::ForwardKeyPressed);
	PlayerInputComponent->BindAxis("strafeKey", this, &AplayerCharacter::strafeKeyPressed);

	PlayerInputComponent->BindAxis("MouseY", this, &AplayerCharacter::MouseY);
	PlayerInputComponent->BindAxis("MouseX", this, &AplayerCharacter::MouseX);

	PlayerInputComponent->BindAction("sprintKey", IE_Pressed, this, &AplayerCharacter::SprintKeyPressed);
	PlayerInputComponent->BindAction("sprintKey", IE_Released, this, &AplayerCharacter::SprintKeyReleased);

	PlayerInputComponent->BindAction("crouchKey", IE_Pressed, this, &AplayerCharacter::CrouchKeyPressed);
	PlayerInputComponent->BindAction("crouchKey", IE_Released, this, &AplayerCharacter::CrouchKeyReleased);

	PlayerInputComponent->BindAction("toggleCamera", IE_Pressed, this, &AplayerCharacter::ToggleCameraPressed);
	PlayerInputComponent->BindAction("toggleCamera", IE_Released, this, &AplayerCharacter::ToggleCameraReleased);
}


void AplayerCharacter::ForwardKeyPressed(float inputValue) {
	forwardValue = inputValue;

	// Check if moving forward, sprinting and not crouching
	if (inputValue > 0 && sprintPressed && !crouchPressed) {
		GetCharacterMovement()->MaxWalkSpeed = 1000;
		
	}
	else if (crouchPressed) {
		GetCharacterMovement()->MaxWalkSpeed = 250;
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = 500;
	}
}

void AplayerCharacter::strafeKeyPressed(float inputValue) {
	strafeValue = inputValue;
}

void AplayerCharacter::MouseY(float inputValue) {
	mouseYValue = inputValue;
}

void AplayerCharacter::MouseX(float inputValue) {
	mouseXValue = inputValue;
}


void AplayerCharacter::SprintKeyPressed() {
	sprintPressed = true;
	UE_LOG(LogTemp, Log, TEXT("%s"), sprintPressed ? TEXT("true") : TEXT("False"));
}



void AplayerCharacter::SprintKeyReleased() {
	sprintPressed = false;
	UE_LOG(LogTemp, Log, TEXT("%s"), sprintPressed ? TEXT("true") : TEXT("False"));
}

void AplayerCharacter::CrouchKeyPressed() {
	crouchPressed = true;
	UE_LOG(LogTemp, Log, TEXT("%s"), crouchPressed ? TEXT("true") : TEXT("False"));
}

void AplayerCharacter::CrouchKeyReleased() {
	crouchPressed = false;
	UE_LOG(LogTemp, Log, TEXT("%s"), crouchPressed ? TEXT("true") : TEXT("False"));
}

void AplayerCharacter::ToggleCameraPressed() {
	//toggle and switch camera.
	firstPerson = !firstPerson;
	ActivateCam();
}

void AplayerCharacter::ToggleCameraReleased() {

}