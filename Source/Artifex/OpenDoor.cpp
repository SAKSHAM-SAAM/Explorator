// Fill out your copyright notice in the Description page of Project Settings.
#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

#define OUT
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	initialYaw = GetOwner()->GetActorRotation().Yaw;
	currentYaw = initialYaw;
	OpenAngle = currentYaw - OpenAngle;
	FindAudioComponent();
	FindPressurePlate();
	// ActorThatOpens=GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::FindAudioComponent()
{
	DoorAudio = GetOwner()->FindComponentByClass<UAudioComponent>();
	if(DoorAudio == nullptr)
	{
		UE_LOG(LogTemp,Error,TEXT("No Audio component on door %"),*(GetOwner()->GetName()));
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(TotalMassOfActors() >= MassToOpenDoor)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened=GetWorld()->GetTimeSeconds();
	}else{
		if(GetWorld()->GetTimeSeconds() - DoorLastOpened >= DoorCloseDelay){
			CloseDoor(DeltaTime);
		}
	}
	
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	currentYaw = FMath::Lerp(currentYaw,OpenAngle,DeltaTime * OpenDoorSpeed);
	FRotator OpenDoor=GetOwner()->GetActorRotation();
	OpenDoor.Yaw = currentYaw;
	GetOwner()->SetActorRotation(OpenDoor);

	if(!DoorAudio )
	{ return;}
	if(!OpenDoorSound)
	{
		DoorAudio->Play();
		OpenDoorSound=true;
		CloseDoorSound=false;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	currentYaw = FMath::Lerp(currentYaw,initialYaw,DeltaTime * CloseDoorSpeed );
	FRotator OpenDoor=GetOwner()->GetActorRotation();
	OpenDoor.Yaw = currentYaw;
	GetOwner()->SetActorRotation(OpenDoor);

	if(!DoorAudio )
	{ return;}
	if(!CloseDoorSound)
	{
		DoorAudio->Play();
		CloseDoorSound=true;
		OpenDoorSound=false;
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	for(AActor * actor:OverlappingActors)
	{
		TotalMass = TotalMass + actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp,Warning,TEXT("Component in overlap -> %s"),*(actor->GetName()));
	}
	return TotalMass;
}

void UOpenDoor::FindPressurePlate()
{
	if(PressurePlate == NULL)
	{
		UE_LOG(LogTemp, Error,TEXT("%s has no pressure plate component set for opendoor component"),*(GetOwner()->GetName()));
	}
}