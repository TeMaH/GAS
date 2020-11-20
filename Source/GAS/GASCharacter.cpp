// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"
#include "GASAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

//////////////////////////////////////////////////////////////////////////
// AGASCharacter

AGASCharacter::AGASCharacter()
{
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // set our turn rates for input
    BaseTurnRate = 45.f;
    BaseLookUpRate = 45.f;

    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true;            // Character moves in the direction of input...
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
    GetCharacterMovement()->JumpZVelocity = 600.f;
    GetCharacterMovement()->AirControl = 0.2f;

    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f;       // The camera follows at this distance behind the character
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(
        CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match
                                                      // the controller orientation

    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
    // are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

    // Initialize our AbilitySystemComponent
    AbilitySystemComponent = CreateDefaultSubobject<UCharacterAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
    AbilitySystemComponent->SetIsReplicated(true);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGASCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // Set up gameplay key bindings
    check(PlayerInputComponent);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAxis("MoveForward", this, &AGASCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &AGASCharacter::MoveRight);

    // We have 2 versions of the rotation bindings to handle different kinds of devices differently
    // "turn" handles devices that provide an absolute delta, such as a mouse.
    // "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
    PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("TurnRate", this, &AGASCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("LookUpRate", this, &AGASCharacter::LookUpAtRate);

    // handle touch devices
    PlayerInputComponent->BindTouch(IE_Pressed, this, &AGASCharacter::TouchStarted);
    PlayerInputComponent->BindTouch(IE_Released, this, &AGASCharacter::TouchStopped);

    // VR headset functionality
    PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AGASCharacter::OnResetVR);
}

void AGASCharacter::AcquireAbility(TSubclassOf<UGameplayAbility> AbilityToAquire)
{
    if (!HasAuthority())
    {
        return;
    }

    if (ensure(AbilitySystemComponent != nullptr))
    {
        if (AbilityToAquire != nullptr)
        {
            // TODO: maybe need refactor
            // GiveAbility(AbilitySpec)
            // https://github.com/tranek/GASDocumentation#463-granting-abilities
            FGameplayAbilitySpecDef AbilitySpecDef = FGameplayAbilitySpecDef();
            AbilitySpecDef.Ability = AbilityToAquire;
            FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilitySpecDef, 1);
            AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilitySpec));
        }
    }
}

void AGASCharacter::BeginPlay()
{
    GetAbilitySystemComponent()->AddSet<UGASAttributeSet>();
    UGASAttributeSet* Set = GetAttributeSet();
    Set->InitFirstAttr(1.0f);
    Set->InitSecondAttr(2.2f);
    Set->InitThirdAttr(3.33f);
    if (HasAuthority())
    {
        for (TSubclassOf<UGameplayAbility>& Ability : Abilities)
        {
            AcquireAbility(Ability);
        }
    }
    Super::BeginPlay();
}

void AGASCharacter::OnResetVR()
{
    UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AGASCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
    Jump();
}

void AGASCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
    StopJumping();
}

void AGASCharacter::TurnAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}
 
void AGASCharacter::LookUpAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AGASCharacter::MoveForward(float Value)
{
    if ((Controller != NULL) && (Value != 0.0f))
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void AGASCharacter::MoveRight(float Value)
{
    if ((Controller != NULL) && (Value != 0.0f))
    {
        // find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get right vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        // add movement in that direction
        AddMovementInput(Direction, Value);
    }
}

void AGASCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    FGameplayAbilityActorInfo* Info = AbilitySystemComponent->AbilityActorInfo.Get();

    FGameplayTagContainer OwnerTags;
    AbilitySystemComponent->GetOwnedGameplayTags(OwnerTags);

    FString CombinedStrings = GetNameSafe(this);

    int32 TagCount = 1;
    const int32 NumTags = OwnerTags.Num();
    bool HasBug = false;
    for (FGameplayTag Tag : OwnerTags)
    {
        int32 Count = AbilitySystemComponent->GetTagCount(Tag);
        CombinedStrings.Append(FString::Printf(TEXT("\n%s (%d)"), *Tag.ToString(), Count));
    }

    if (HasAuthority())
    {
        UKismetSystemLibrary::PrintString(GetWorld(), CombinedStrings, true, false, IsLocallyControlled() ? FLinearColor::Yellow : FLinearColor::White, 0.0f);
    }
    else
    {
        DrawDebugString(GetWorld(), GetActorLocation() + FVector::UpVector * GetCapsuleComponent()->GetScaledCapsuleHalfHeight(),
            CombinedStrings,
            nullptr,       // class AActor* TestBaseActor = NULL
            IsLocallyControlled() ? FColor::White : FColor::Yellow, // FColor const& TextColor = FColor::White
            0.f            // float Duration = -1.000000
        );
    }
}

const FString EnumToString(const TCHAR* Enum, int32 EnumValue)
{
    const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, Enum, true);
    if (!EnumPtr)
        return NSLOCTEXT("Invalid", "Invalid", "Invalid").ToString();

    return EnumPtr->GetDisplayNameText(EnumValue).ToString();
}

UAbilitySystemComponent* AGASCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

UGASAttributeSet* AGASCharacter::GetAttributeSet() const
{
    return const_cast<UGASAttributeSet*>(GetAbilitySystemComponent()->GetSet<UGASAttributeSet>());
}

void AGASCharacter::ClientOnPossesed_Implementation()
{
    OnPossessed();
}
