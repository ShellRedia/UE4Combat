// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType :uint8
{
	EW_SWORD UMETA(DisplayName="Sword"), 
	EW_GUN UMETA(DisplayName = "Gun")
};

USTRUCT(BlueprintType)
struct FWeaponTable :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		USkeletalMesh* WeaponMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		UTexture2D* WeaponImage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		EWeaponType WeaponType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		FString WeaponName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float Attack;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
		float AttackRate;
};

UCLASS()
class UE4COMBAT01_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	friend class AWarriorCharacter;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void Pickup(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void Hurt(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Range, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* AttackBoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Range, meta = (AllowPrivateAccess = "true"))
		class USphereComponent* PickupSphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Texture, meta = (AllowPrivateAccess = "true"))
		class UTexture2D* WeaponImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Property, meta = (AllowPrivateAccess = "true"))
		FString WeaponName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Property, meta = (AllowPrivateAccess = "true"))
		EWeaponType WeaponType;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Property, meta = (AllowPrivateAccess = "true"))
		float Attack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Property, meta = (AllowPrivateAccess = "true"))
		float AttackRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Warrior, meta = (AllowPrivateAccess = "true"))
		class AWarriorCharacter* owner_character = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = DataTable, meta = (AllowPrivateAccess = "true"))
		UDataTable* ItableInfo = nullptr;
};
