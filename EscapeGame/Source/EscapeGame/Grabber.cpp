// Copyright PerfectoGames 2018

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

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
	/// look for attached Physics Handle
	FindPhysicsComponent();
	InputController();
	
}

void UGrabber::FindPhysicsComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics handle %s"), *GetOwner()->GetName());
	}
}

void UGrabber::InputController()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component found %s"), *GetOwner()->GetName());
		//Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Released);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Input Component found %s"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed%s"), *GetOwner()->GetName());

	//Line and trace and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();

	// if we hit something then attach a physics handle
	//attach physics handle
	if (ActorHit)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, ActorHit->GetActorLocation(),FRotator(0));
	};
}

void UGrabber::Released()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Released"));
	PhysicsHandle->ReleaseComponent();

}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector PlayerViewPointLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewRotation);
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewRotation.Vector() * Reach;

	//If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		//move the object that we're holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}
	

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewRotation);
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewRotation.Vector() * Reach;
	//Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Ray-cast out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);

	//See what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		FString ActorName = Hit.GetActor()->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Object hit %s "), *ActorName);
	}

	return Hit;
}

/*
unused codes

//Draw a red trace in the world to visualize
DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255, 0, 0), false, 0.0f, 0, 10.0f);

*/