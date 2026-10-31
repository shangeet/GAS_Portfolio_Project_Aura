// Copyright Shantanu Mantri


#include "Character/AuraPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AAuraPlayerCharacter::AAuraPlayerCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));

	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->TargetArmLength = 300.f; // The camera follows at this distance from the character 
	SpringArmComponent->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	
	// Character turns in the direction the controller is moving, but only on the Y axis
	TObjectPtr<UCharacterMovementComponent> MovementComponent = GetCharacterMovement();

	MovementComponent->bOrientRotationToMovement = true;
	MovementComponent->RotationRate = FRotator(0.f, 540.f, 0.f);
	MovementComponent->bConstrainToPlane = true;
	MovementComponent->bSnapToPlaneAtStart = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}
