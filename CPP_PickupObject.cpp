// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_PickupObject.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Engine/GameEngine.h"

// Sets default values
ACPP_PickupObject::ACPP_PickupObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
  
  /********************* StaticMesh ***************************/

  /* Create Static Mesh Component */
  StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

  /* Setting Material and StaticMesh */
  ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/Assets/Base/Meshes/SM_ReferenceCube"));
  Mesh = MeshAsset.Object;
  StaticMesh->SetStaticMesh(Mesh);

  ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("/Engine/EngineMaterials/WorldGridMaterial"));
  Material = MaterialAsset.Object;
  StaticMesh->SetMaterial(0, Material);

  /* Setting Physics and Collision*/
  StaticMesh->SetGenerateOverlapEvents(true);
  StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
  StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  StaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, 
    ECollisionResponse::ECR_Overlap);

  RootComponent = StaticMesh;
  
  /********************* SphereCollision ***************************/

  /* Create Sphere Collision Component */
  SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));

  /*Setting Radius*/
  SphereCollision->SetSphereRadius(1300.0f);
  
  /* Physics and Collision */
  SphereCollision->SetGenerateOverlapEvents(true);
  SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  SphereCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
  SphereCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  SphereCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1,
    ECollisionResponse::ECR_Overlap);

  /* Attach to Root */
  //SphereCollision->AttachTo(RootComponent);
  //SphereCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
  
  /********************* Sound ***************************/

  /* Create Sound */
  ConstructorHelpers::FObjectFinder<UAkAudioEvent> SoundStartCue(TEXT("AkAudioEvent'/Game/WwiseAudio/Events/Play_LetterNear.Play_LetterNear'"));
  ConstructorHelpers::FObjectFinder<UAkAudioEvent> SoundEndCue(TEXT("AkAudioEvent'/Game/WwiseAudio/Events/Stop_LetterNear.Stop_LetterNear'"));
  LetterSoundStartEvent = SoundStartCue.Object;
  LetterSoundEndEvent = SoundEndCue.Object;

  LetterStartAudio = CreateDefaultSubobject<UAkGameplayStatics>(TEXT("LetterNearSoundStart"));
  LetterEndAudio = CreateDefaultSubobject<UAkGameplayStatics>(TEXT("LetterNearSoundEnd"));

  StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ACPP_PickupObject::StaticMeshBeginOverlap);
  SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ACPP_PickupObject::BeginOverlap);
  SphereCollision->OnComponentEndOverlap.AddDynamic(this, &ACPP_PickupObject::EndOverlap);
}

void ACPP_PickupObject::BeginOverlap(UPrimitiveComponent* overlapped,
  AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex,
  bool fromSweep, const FHitResult& SweepResult)
{
  if (otherActor == nullptr || otherActor == this || otherComp == nullptr)
    return;

  if (!InRange)
  {
    InRange = true;
    
    LetterStartAudio->PostEventAttached(LetterSoundStartEvent, otherActor);
  }
}

void ACPP_PickupObject::EndOverlap(UPrimitiveComponent* overlapped,
  AActor* otherActor,
  UPrimitiveComponent* otherComp,
  int32 otherBodyIndex)
{
  if (otherActor == nullptr || otherActor == this || otherComp == nullptr)
    return;

  if (InRange)
  {
    InRange = false;
    LetterEndAudio->PostEventAttached(LetterSoundEndEvent, otherActor);
  }
}

void ACPP_PickupObject::StaticMeshBeginOverlap(UPrimitiveComponent* overlapped,
  AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex,
  bool fromSweep, const FHitResult& SweepResult)
{
  if (otherActor == nullptr || otherActor == this || otherComp == nullptr)
    return;

  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("StaticMesh!"));
  Pickup();
  this->Destroy();
}