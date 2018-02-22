// Copyright PerfectoGames 2018

#include "Grabber.h"
#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty"));
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get the player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewRotation);
	UE_LOG(LogTemp, Warning, TEXT("PlayerViewPoint is %s and PlayerViewRotation is %s"), *PlayerViewPointLocation.ToString(), *PlayerViewRotation.ToString());
	// Ray-cast out to reach distance

	// See what we hit
}
