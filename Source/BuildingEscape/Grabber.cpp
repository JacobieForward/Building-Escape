// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

// Sets default values for this component's properties
UGrabber::UGrabber() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay() {
	Super::BeginPlay();

	FindPhysicsHandle();

	SetupInputComponent();
}

void UGrabber::FindPhysicsHandle() {
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!physicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle Component added to object: %s with component Grabber."), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent() {	
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (inputComponent) {
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	} else {
		UE_LOG(LogTemp, Error, TEXT("No Input Component added to object: %s with component Grabber."), *GetOwner()->GetName());
	}
}

void UGrabber::Grab() {
	FHitResult hitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = hitResult.GetComponent();
	AActor* actorHit = hitResult.GetActor();

	if (actorHit) {
		if (!physicsHandle) {return;}
		physicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, hitResult.ImpactPoint);
	}
}

void UGrabber::Release() {
	if (!physicsHandle) {return;}
	physicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (physicsHandle->GrabbedComponent) {
		physicsHandle->SetTargetLocation(CalculateLineTraceEnd());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const {
	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());

	FHitResult hit;

	GetWorld()->LineTraceSingleByObjectType(OUT hit, GetPlayerWorldPosition(), CalculateLineTraceEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), traceParams);
	
	AActor* hitActor = hit.GetActor();
	if (hitActor) {
		UE_LOG(LogTemp, Warning, TEXT("Hit actor %s"), *(hitActor->GetName()));
	}

	return hit;
}

FVector UGrabber::CalculateLineTraceEnd() const {
	FVector playerViewpointLocation;
	FRotator playerViewpointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerViewpointLocation, playerViewpointRotation);

	FHitResult hit;

	return playerViewpointLocation + playerViewpointRotation.Vector() * Reach;
}

FVector UGrabber::GetPlayerWorldPosition() const {
	FVector playerViewpointLocation;
	FRotator playerViewpointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(playerViewpointLocation, playerViewpointRotation);

	return playerViewpointLocation;
}

//DrawDebugLine(GetWorld(), playerViewpointLocation, lineTraceEnd, FColor(255, 0, 0), false, 0.0f, 0, 3);