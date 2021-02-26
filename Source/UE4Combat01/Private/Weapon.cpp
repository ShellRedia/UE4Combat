// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "WarriorCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Initialize
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	Mesh->SetGenerateOverlapEvents(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent = Mesh;

	AttackBoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackBoxCollision"));
	AttackBoxCollision->InitBoxExtent(FVector(2,10,40));
	AttackBoxCollision->SetupAttachment(Mesh); 
	AttackBoxCollision->BodyInstance.SetCollisionProfileName("OverlapAllDynamic");
	AttackBoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::Hurt);
	AttackBoxCollision->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform, "AttackRange");

	PickupSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphereCollision"));
	PickupSphereCollision->InitSphereRadius(50);
	PickupSphereCollision->SetupAttachment(Mesh);
	PickupSphereCollision->BodyInstance.SetCollisionProfileName("OverlapAllDynamic");
	PickupSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::Pickup);
	PickupSphereCollision->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform, "PickupRange");

	static ConstructorHelpers::FObjectFinder<UDataTable> weapon_table(TEXT("DataTable'/Game/WarriorWeapons/WeaponDataTable.WeaponDataTable'"));
	if (weapon_table.Succeeded())
	{
		ItableInfo = weapon_table.Object;
		TArray<FName> RowNames = ItableInfo->GetRowNames();
		if (RowNames.Num() != 0)
		{
			FString ContextString;
			srand(time(nullptr));
			int idx = rand() % RowNames.Num();
			FWeaponTable* WeaponInfo = ItableInfo->FindRow<FWeaponTable>(RowNames[idx], ContextString);
			if (WeaponInfo)
			{
				Mesh->SetSkeletalMesh(WeaponInfo->WeaponMesh);
				WeaponImage = WeaponInfo->WeaponImage;
				WeaponName = WeaponInfo->WeaponName;
				WeaponType = WeaponInfo->WeaponType;
				Attack = WeaponInfo->Attack;
				AttackRate = WeaponInfo->AttackRate;
			}
		}
	}
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Collision functions : 
void AWeapon::Pickup(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (owner_character == nullptr && OtherActor->GetClass()->IsChildOf(AWarriorCharacter::StaticClass()))
	{
		owner_character = Cast<AWarriorCharacter>(OtherActor);
		owner_character->AddWeapon(this);
	}
}

void AWeapon::Hurt(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{}

