// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h "
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARTIFEX_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	void FindPressurePlate();

	bool OpenDoorSound = false;
	bool CloseDoorSound = true;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float initialYaw;
	float currentYaw;

	UPROPERTY(EditAnywhere);
	float OpenAngle=90.f;
		
	UPROPERTY(EditAnywhere);
	ATriggerVolume *PressurePlate;

	UPROPERTY(EditAnywhere);
	AActor *ActorThatOpens;

	float DoorLastOpened = 0.f;
	UPROPERTY(EditAnywhere);
	float DoorCloseDelay = 1.f;
	UPROPERTY(EditAnywhere);
	float OpenDoorSpeed = 1.f;
	UPROPERTY(EditAnywhere);
	float CloseDoorSpeed = 1.5f;
	UPROPERTY(EditAnywhere);
	float MassToOpenDoor = 50.f;
	UPROPERTY(EditAnywhere);
	UAudioComponent *DoorAudio=nullptr;
};
