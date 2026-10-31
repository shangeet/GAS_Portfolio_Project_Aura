// Copyright Shantanu Mantri


#include "Controllers/AuraPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	LastActor = nullptr;
	CurrentActor = nullptr;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);
	
	UEnhancedInputLocalPlayerSubsystem* PlayerSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(PlayerSubsystem);
	
	PlayerSubsystem->AddMappingContext(AuraContext, 0);
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
	
	const FRotationMatrix RotationMatrix(YawRotation);
	const FVector ForwardDirection = RotationMatrix.GetUnitAxis(EAxis::X);
	const FVector RightDirection = RotationMatrix.GetUnitAxis(EAxis::Y);
	
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	
	if (!CursorHit.bBlockingHit) { return; }
	
	LastActor = CurrentActor;
	CurrentActor = CursorHit.GetActor();
	
	UE_LOG(LogTemp, Warning, TEXT("Hit: %s"),
	CursorHit.GetActor() ? *CursorHit.GetActor()->GetName() : TEXT("None"));
	
	/**
	 * Line Trace from cursor scenarios:
	 *   1. Last Actor is null && Current Actor is null
	 *		- Do nothing
	 *   2. Last Actor is null && Current Actor is valid
	 *		- Hovering over this actor (first time)
	 *		- Highlight on actor
	 *	 3. Last Actor is valid && Current Actor is null
	 *		- Last Actor should be unhighlighted
	 *   4. Both Actors are valid (LastActor != ThisActor)
	 *		- Last Actor should be unhighlighted
	 *		- Current Actor shoudl be highlighted
	 */
	
	if (LastActor != CurrentActor)
	{
		if (LastActor)
		{
			LastActor->UnHighlightActor();
		}
		if (CurrentActor)
		{
			CurrentActor->HighlightActor();
		}
	}

}
