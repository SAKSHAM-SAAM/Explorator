// Copyright belongs to SAAM corporations
#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT 
// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("No Physics Handle on -> %s"),*( GetOwner()->GetName() ));
	}
}

void UGrabber::SetupInputComponent()
{
	InputComp = GetOwner()->FindComponentByClass<UInputComponent>();
	if(InputComp)
	{
		UE_LOG(LogTemp,Warning,TEXT("Input found"));
		InputComp->BindAction("Grab",IE_Pressed,this, &UGrabber::Grab);
		InputComp->BindAction("Grab",IE_Released,this,&UGrabber::Release);
	}
}

void UGrabber::Grab()
{
	// UE_LOG(LogTemp,Error,TEXT("Grab function works"));
	FHitResult HitResult = FirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor *HitActor = HitResult.GetActor();
	// Safety from NULL POINTER
	if(HitActor)
	{
		if( !PhysicsHandle) {return ; }
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			GetPlayersReach()
		);
	}
}

FVector UGrabber::GetPlayersReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);
	FVector TraceLineEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	return TraceLineEnd;
}

void UGrabber::Release()
{
	// UE_LOG(LogTemp,Error,TEXT("release function works"));
	if( !PhysicsHandle) {return ; }
	PhysicsHandle->ReleaseComponent();
}
// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if( !PhysicsHandle) {return ; }
	if(PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}
}

FHitResult UGrabber::FirstPhysicsBodyInReach() const
{
	FHitResult hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT hit,
		GetPlayersWorldPos(),
		GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	AActor * ActorHit = hit.GetActor();
	if(ActorHit)
	{
		UE_LOG(LogTemp,Error,TEXT(" object in sight : %s"),*(ActorHit->GetName()));
	}
	return hit;
}

FVector UGrabber::GetPlayersWorldPos() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation, 
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation;
}
