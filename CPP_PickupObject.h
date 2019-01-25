// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AkAudioEvent.h"
#include "AkGameplayStatics.h"
#include "CPP_PickupObject.generated.h"

UCLASS()
class DINODELIVERY_API ACPP_PickupObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_PickupObject();

protected:
  UPROPERTY(BlueprintReadWrite, Category = "Audio")
    UAkAudioEvent* LetterSoundStartEvent;

  UPROPERTY(BlueprintReadWrite, Category = "Audio")
    UAkAudioEvent* LetterSoundEndEvent;

  UPROPERTY(BlueprintReadWrite, Category = "Audio")
    UAkGameplayStatics* LetterStartAudio;

  UPROPERTY(BlueprintReadWrite, Category = "Audio")
    UAkGameplayStatics* LetterEndAudio;

public:	
  /* Components */
  UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Component")
  class UStaticMeshComponent* StaticMesh;

  UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Component")
  class USphereComponent* SphereCollision;

  UFUNCTION(BlueprintImplementableEvent)
  void Pickup();

private:
  /* Variables */
  UPROPERTY(EditAnywhere)
  bool InRange = false;

  /* Materials and Mesh */
  UPROPERTY(EditAnywhere, Category = "StaticMesh")
  UStaticMesh* Mesh;

  UPROPERTY(EditAnywhere, Category = "Material")
  UMaterial* Material;

  /* Collision - Sphere Collision */
  UFUNCTION()
  void BeginOverlap(UPrimitiveComponent* overlapped,
    AActor* otherActor,
    UPrimitiveComponent* otherComp,
    int32 otherBodyIndex,
    bool fromSweep,
    const FHitResult& SweepResult);

  UFUNCTION()
  void EndOverlap(UPrimitiveComponent* overlapped,
      AActor* otherActor,
      UPrimitiveComponent* otherComp,
      int32 otherBodyIndex);

  UFUNCTION()
    void StaticMeshBeginOverlap(UPrimitiveComponent* overlapped,
      AActor* otherActor,
      UPrimitiveComponent* otherComp,
      int32 otherBodyIndex,
      bool fromSweep,
      const FHitResult& SweepResult);
};
