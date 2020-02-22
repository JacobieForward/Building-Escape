// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"

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
	targetYaw += initialYaw;

	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("Pressure Plate not set for actor: %s"), *GetOwner()->GetName());
	}

	FindAudioComponent();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > massRequiredForOpening) {
		OpenDoor(DeltaTime);
		doorLastOpened = GetWorld()->GetTimeSeconds();
	} else if (GetWorld()->GetTimeSeconds() - doorLastOpened > doorCloseDelay) {
		CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime) {
	currentYaw = GetOwner()->GetActorRotation().Yaw;
	currentYaw = FMath::FInterpTo(currentYaw, targetYaw, DeltaTime, doorOpenSpeed);
	FRotator newRotation(0.0f, currentYaw, 0.0f);
	GetOwner()->SetActorRotation(newRotation);
	// TODO: Sound not implemented
}

void UOpenDoor::CloseDoor(float DeltaTime) {
	currentYaw = GetOwner()->GetActorRotation().Yaw;
	currentYaw = FMath::FInterpTo(currentYaw, initialYaw, DeltaTime, doorCloseSpeed);
	FRotator newRotation(0.0f, currentYaw, 0.0f);
	GetOwner()->SetActorRotation(newRotation);
	// TODO: Sound not implemented
}

float UOpenDoor::TotalMassOfActors() const {
	float totalMass = 0.0f;

	TArray<AActor*> OverlappingActors;
	if (PressurePlate) {
		PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Pressure Plate not set for actor %s"), *GetOwner()->GetName());
		return totalMass;
	}

	for(AActor* actor : OverlappingActors) {
		totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return totalMass;
}

void UOpenDoor::FindAudioComponent() {
	audioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();

	if (!audioComponent) {
		UE_LOG(LogTemp, Error, TEXT("No audio component attached to actor: %s"), *GetOwner()->GetName());
	}
}