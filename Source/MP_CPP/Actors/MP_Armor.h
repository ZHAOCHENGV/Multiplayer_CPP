// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MP_Armor.generated.h"

class USphereComponent;

UCLASS()
class MP_CPP_API AMP_Armor : public AActor
{
	GENERATED_BODY()

public:
	
	AMP_Armor();
	
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
protected:
	
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName SphereMeshInSocketName;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName BootMeshLInSocketName;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	FName BootMeshRInSocketName;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	float ArmorValue;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SphereMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BootMesh_L;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> BootMesh_R;

	

};
