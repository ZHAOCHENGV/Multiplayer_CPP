// Fill out your copyright notice in the Description page of Project Settings.


#include "MP_Armor.h"


#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Interface/MP_Player.h"


AMP_Armor::AMP_Armor()
{
	
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>("SphereMesh");
	SphereMesh->SetupAttachment(RootComponent);
	SphereMesh->SetIsReplicated(true);
	SphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BootMesh_L = CreateDefaultSubobject<UStaticMeshComponent>("BootMesh_L");
	BootMesh_L->SetupAttachment(RootComponent);
	BootMesh_L->SetIsReplicated(true);
	BootMesh_L->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BootMesh_R = CreateDefaultSubobject<UStaticMeshComponent>("BootMesh_R");
	BootMesh_R->SetupAttachment(RootComponent);
	BootMesh_R->SetIsReplicated(true);
	BootMesh_R->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollision->SetupAttachment(RootComponent);
	
	
	
}


void AMP_Armor::BeginPlay()
{
	Super::BeginPlay();

}


void AMP_Armor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMP_Armor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!OtherActor->HasAuthority()) return;
	if (OtherActor->Implements<UMP_Player>())
	{
		USkeletalMeshComponent* Mesh = IMP_Player::Execute_GetSkeletalMeshComponent(OtherActor);
		SphereMesh->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale,SphereMeshInSocketName);
		BootMesh_L->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale,BootMeshLInSocketName);
		BootMesh_R->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale,BootMeshRInSocketName);
		SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		IMP_Player::Execute_GrantArmor(OtherActor, ArmorValue);
	}


		
	
	
}

