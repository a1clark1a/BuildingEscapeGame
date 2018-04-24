// Copyright PerfectoGames 2018

#pragma once

#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "Components/ActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseRequest);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEGAME_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(BlueprintAssignable)
	FOnOpenRequest OnOpen;

	UPROPERTY(BlueprintAssignable)
	FOnCloseRequest OnClose;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	AActor* Owner = nullptr;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	float TriggerMass = 30.0f;

	//Returns total mass
	float GetTotalMassOfActorsOnPlate();
};
