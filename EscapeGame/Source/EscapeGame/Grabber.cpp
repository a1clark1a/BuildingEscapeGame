// Copyright PerfectoGames 2018

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
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

//Set Physics Handle
void UGrabber::FindPhysicsComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics handle %s"), *GetOwner()->GetName());
		return;
	}
	
}

//Function to handle player controller
void UGrabber::InputController()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
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
	if (ActorHit)
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, ActorHit->GetActorLocation(),ActorHit->GetActorRotation());
	};
}

void UGrabber::Released()
{
	if (!PhysicsHandle) { return; }
	UE_LOG(LogTemp, Warning, TEXT("Grab Released"));
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }
	//If the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		//move the object that we're holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd());
	}
}
	

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	//Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Ray-cast out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation, LineTraceEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);

	//See what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		FString ActorName = Hit.GetActor()->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Object hit %s "), *ActorName);
	}
	return Hit;
}

const FVector UGrabber::LineTraceEnd()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewRotation);
	FVector LineTrace = PlayerViewPointLocation + PlayerViewRotation.Vector()* Reach;
	return LineTrace;
}
