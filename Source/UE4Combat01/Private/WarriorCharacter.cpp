// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "WarriorAnimInstance.h"

AWarriorCharacter::AWarriorCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Load resources
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_warrior(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (SK_warrior.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_warrior.Object);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -95));
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	}
	
	
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> anim_default(TEXT("AnimBlueprint'/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP'"));
	if (anim_default.Succeeded())
	{
		GetMesh()->SetAnimClass(anim_default.Object->GeneratedClass);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> attack_montage(TEXT("AnimMontage'/Game/Mannequin/Animations/WarriorAttack_Montage.WarriorAttack_Montage'"));
	if (attack_montage.Succeeded())
	{
		State2Montage.Add(EWarriorState::EW_SWORD, attack_montage.Object);
	}
	
}

//Input
void AWarriorCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AWarriorCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AWarriorCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AWarriorCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AWarriorCharacter::LookUpAtRate);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AWarriorCharacter::StartAttack);
}

void AWarriorCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AWarriorCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
void AWarriorCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AWarriorCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AWarriorCharacter::StartAttack()
{
	if (!isAttacking && State != EWarriorState::EW_DEFAULT)
	{
		isAttacking = true;
		GetMovementComponent()->Deactivate();

		PlayAnimMontage(State2Montage[State], AttackRate, TEXT("Uppercut"));
	}
}

void AWarriorCharacter::EndAttack()
{
	isAttacking = false;
	GetMovementComponent()->Activate();
	StopAnimMontage(State2Montage[State]);
}

void AWarriorCharacter::AddWeapon(AWeapon* weapon)
{
	if (WeaponList.Num() == 0 || State == EWarriorState::EW_DEFAULT)
	{
		
		weapon->Mesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, "Sword");
		weapon->Mesh->SetRelativeTransform(FTransform());
		State = EWarriorState::EW_SWORD;
	}
	WeaponList.Add(weapon);
}