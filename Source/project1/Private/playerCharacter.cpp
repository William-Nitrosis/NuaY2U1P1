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
	SpringArm->TargetArmLength = 300.f;
	SpringArm->SetRelativeLocation(FVector(0.f, 0.f, 90.f));


	TP_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("TP_Camera"));
	TP_Camera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
	TP_Camera->SetRelativeLocation(FVector(20.f, 40.f, 0.f));
	TP_Camera->PostProcessSettings.bOverride_MotionBlurAmount = 0;
	TP_Camera->PostProcessSettings.MotionBlurAmount = 0.f;
	TP_Camera->PostProcessSettings.bOverride_MotionBlurMax = 0;
	TP_Camera->PostProcessSettings.MotionBlurMax = 0.f;
	TP_Camera->PostProcessSettings.bOverride_MotionBlurPerObjectSize = 0;
	TP_Camera->PostProcessSettings.MotionBlurPerObjectSize = 0.f;

	FP_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FP_Camera"));
	FP_Camera->SetupAttachment(RootComponent);
	FP_Camera->SetRelativeLocation(FVector(0.f, 0.f, 65.f));
	FP_Camera->PostProcessSettings.bOverride_MotionBlurAmount = 0;
	FP_Camera->PostProcessSettings.MotionBlurAmount = 0.f;
	FP_Camera->PostProcessSettings.bOverride_MotionBlurMax = 0;
	FP_Camera->PostProcessSettings.MotionBlurMax = 0.f;
	FP_Camera->PostProcessSettings.bOverride_MotionBlurPerObjectSize = 0;
	FP_Camera->PostProcessSettings.MotionBlurPerObjectSize = 0.f;

	// body geom
	//BodyBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BodyBox"));
	
	GetCharacterMovement()->AirControl = 1.f;

	this->AutoPossessPlayer = EAutoReceiveInput::Player0;
	this->AIControllerClass = APlayerController::StaticClass();


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

	ActivateCam();

	crouchAdjustHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() / 2;
}

// Called every frame
void AplayerCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	rawInput = FRotator(mouseYValue, mouseXValue, 0.0f);
	rawInputX = FRotator(0.0f, mouseXValue, 0.0f);
	

	FRotator sensInput = (rawInput * sens);


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
	
	//Re-initialize hit info
	FHitResult jumpRayReturn(ForceInit);

	jumpRayStart = GetCapsuleComponent()->GetComponentLocation();
	jumpRayEnd = FVector(GetCapsuleComponent()->GetComponentLocation() + FVector(0.f, 0.f, ((0 - GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 22))));
	collisionChannel = ECollisionChannel::ECC_Visibility;

	jumpCastBool = GetWorld()->LineTraceSingleByChannel(jumpRayReturn, jumpRayStart, jumpRayEnd, collisionChannel);

	if (jumpCastBool) {
		jumpCount = 1;

		if (this->GetWorld()->GetFirstPlayerController()->GetInputKeyTimeDown(currentSprintKey) > 0.05f) {
			sprintPressed = true;
		}
		else if (sprintToggle == false) {
			sprintPressed = false;
		}
	} else {
		sprintPressed = false;
	}

	if (0.1f <= this->InputComponent->GetAxisValue(FName(TEXT("ForwardKey")))) {} else {
		sprintPressed = false;
	}

	//UE_LOG(LogTemp, Log, TEXT("%s"), jumpCastBool ? TEXT("true") : TEXT("false"));

}

void AplayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {

	PlayerInputComponent->BindAxis("ForwardKey", this, &AplayerCharacter::ForwardKeyPressed);
	PlayerInputComponent->BindAxis("strafeKey", this, &AplayerCharacter::strafeKeyPressed);

	PlayerInputComponent->BindAxis("MouseY", this, &AplayerCharacter::MouseY);
	PlayerInputComponent->BindAxis("MouseX", this, &AplayerCharacter::MouseX);

	PlayerInputComponent->BindAxis("MouseWheel", this, &AplayerCharacter::MouseWheel);

	PlayerInputComponent->BindAction("sprintKey", IE_Pressed, this, &AplayerCharacter::SprintKeyPressed);
	PlayerInputComponent->BindAction("sprintKey", IE_Released, this, &AplayerCharacter::SprintKeyReleased);

	PlayerInputComponent->BindAction("crouchKey", IE_Pressed, this, &AplayerCharacter::CrouchKeyPressed);
	PlayerInputComponent->BindAction("crouchKey", IE_Released, this, &AplayerCharacter::CrouchKeyReleased);

	PlayerInputComponent->BindAction("toggleCamera", IE_Pressed, this, &AplayerCharacter::ToggleCameraPressed);
	PlayerInputComponent->BindAction("toggleCamera", IE_Released, this, &AplayerCharacter::ToggleCameraReleased);

	PlayerInputComponent->BindAction("jumpKey", IE_Pressed, this, &AplayerCharacter::JumpKeyPressed);
	PlayerInputComponent->BindAction("jumpKey", IE_Released, this, &AplayerCharacter::JumpKeyReleased);
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

	TArray<FInputAxisKeyMapping> forwardInputMapping = this->GetWorld()->GetFirstPlayerController()->PlayerInput->GetKeysForAxis(TEXT("ForwardKey"));
	for (int i = 0; i < forwardInputMapping.Num(); i++) {

		if (this->GetWorld()->GetFirstPlayerController()->IsInputKeyDown(forwardInputMapping[i].Key)) {
			currentForwardKey = forwardInputMapping[i].Key; //this is either W or S.
		}
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

void AplayerCharacter::MouseWheel(float inputValue) {
	mouseWheelValue = inputValue;

	FVector springDistance = TP_Camera->GetComponentLocation() - SpringArm->GetComponentLocation();	
	float springLength = springDistance.Size();

	if (springLength - (mouseWheelValue * mouseWheelScale) < 160 || springLength - (mouseWheelValue * mouseWheelScale) > 350) {

	}
	else {
		SpringArm->TargetArmLength -= (mouseWheelValue * mouseWheelScale);
	}
	   
	//UE_LOG(LogTemp, Log, TEXT("%f"), springLength);
}


void AplayerCharacter::SprintKeyPressed() {
	sprintPressed = true;
	//UE_LOG(LogTemp, Log, TEXT("%s"), sprintPressed ? TEXT("true") : TEXT("False"));
	TArray<FInputActionKeyMapping> sprintInputMapping = this->GetWorld()->GetFirstPlayerController()->PlayerInput->GetKeysForAction(TEXT("sprintKey"));
	for (int i = 0; i < sprintInputMapping.Num(); i++) {

		if (this->GetWorld()->GetFirstPlayerController()->IsInputKeyDown(sprintInputMapping[i].Key)) {
			currentSprintKey = sprintInputMapping[i].Key;

		}
	}
}

void AplayerCharacter::SprintKeyReleased() {
	if (forwardValue > 0.3) {
		sprintPressed = true;
	} else {
		sprintPressed = false;
	}
	//UE_LOG(LogTemp, Log, TEXT("%s"), sprintPressed ? TEXT("true") : TEXT("False"));
}

void AplayerCharacter::CrouchKeyPressed() {
	crouchPressed = true;
	sprintPressed = false;
	//UE_LOG(LogTemp, Log, TEXT("%s"), crouchPressed ? TEXT("true") : TEXT("False"));

	GetCapsuleComponent()->SetCapsuleHalfHeight(GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() / crouchAmount);
	GetCapsuleComponent()->AddLocalOffset(FVector(0.f, 0.f, -crouchAdjustHeight));
}

void AplayerCharacter::CrouchKeyReleased() {
	crouchPressed = false;
	//UE_LOG(LogTemp, Log, TEXT("%s"), crouchPressed ? TEXT("true") : TEXT("False"));

	GetCapsuleComponent()->SetCapsuleHalfHeight(GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() * crouchAmount);
	GetCapsuleComponent()->AddLocalOffset(FVector(0.f, 0.f, crouchAdjustHeight));
}

void AplayerCharacter::ToggleCameraPressed() {
	//toggle and switch camera.
	firstPerson = !firstPerson;
	ActivateCam();
}

void AplayerCharacter::ToggleCameraReleased() {

}

void AplayerCharacter::JumpKeyPressed() {
	if (jumpCastBool) {
		jumpCount = 1;
		this->LaunchCharacter(FVector(0.f, 0.f, jumpPower), false, false);
	} else {
		jumpCount++;
		if (jumpCount <= 2) {
			this->LaunchCharacter(FVector(0.f, 0.f, jumpPower), false, false);
		}
	}
}

void AplayerCharacter::JumpKeyReleased() {

}