// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "GameFramework/Character.h"
#include "WarriorCharacter.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EWarriorState :uint8
{
	EW_DEFAULT UMETA(DisplayName = "Default"),
	EW_SWORD UMETA(DisplayName = "Sword"),
	EW_GUN UMETA(DisplayName = "Gun")
};


UCLASS()
class UE4COMBAT01_API AWarriorCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	AWarriorCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UFUNCTION(BlueprintCallable, Category = Attack)
		void EndAttack();

	virtual void AddWeapon(AWeapon* weapon);

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void StartAttack();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/** State to Montage */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State, meta = (AllowPrivateAccess = "true"))
		TMap<EWarriorState, UAnimMontage*> State2Montage;

	/** Inventory */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		TArray<AWeapon *> WeaponList;

	/** Attack */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack, meta = (AllowPrivateAccess = "true"))
		bool isAttacking = false;

	/** Charater Property */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Property, meta = (AllowPrivateAccess = "true"))
		EWarriorState State = EWarriorState::EW_DEFAULT;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Property, meta = (AllowPrivateAccess = "true"))
		float Health = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Property, meta = (AllowPrivateAccess = "true"))
		float Mana = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Property, meta = (AllowPrivateAccess = "true"))
		float AttackRate = 1.0f;
};
