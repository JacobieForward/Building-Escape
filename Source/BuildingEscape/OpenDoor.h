// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"
#include "Engine/TriggerVolume.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();

	bool openDoorSound = false;
	bool closeDoorSound = false;

private:
	UPROPERTY(EditAnywhere)
	float targetYaw = -90.0f;

	UPROPERTY(EditAnywhere)
	float doorCloseDelay = 0.5f;

	UPROPERTY(EditAnywhere)
	float doorOpenSpeed = 1.0f;

	UPROPERTY(EditAnywhere)
	float doorCloseSpeed = 2.0f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float massRequiredForOpening = 50.0f;

	UPROPERTY()
	UAudioComponent* audioComponent = nullptr;

	float currentYaw;
	float initialYaw;
	float doorLastOpened = 0.0f;

};
