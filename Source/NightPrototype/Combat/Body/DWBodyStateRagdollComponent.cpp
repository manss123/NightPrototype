#include "DWBodyStateRagdollComponent.h"
#include "DWBodyHealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"


UDWBodyStateRagdollComponent::UDWBodyStateRagdollComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDWBodyStateRagdollComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}
	
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(Owner))
	{
		if (USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh())
		{
			InitialMeshRelativeTransform = Mesh->GetRelativeTransform();
		}
	}
	
	UDWBodyHealthComponent* BodyHealth = Owner->FindComponentByClass<UDWBodyHealthComponent>();
	if (!BodyHealth)
	{
		return;
	}
	
	if (bRagdollOnDeath)
	{
		BodyHealth->OnDeath.AddDynamic(this, &UDWBodyStateRagdollComponent::HandleDeath);
	}
	
	if (bRagdollOnDowned)
	{
		BodyHealth->OnDowned.AddDynamic(this, &UDWBodyStateRagdollComponent::HandleDowned);
	}
	
	BodyHealth->OnRecovered.AddDynamic(this, &UDWBodyStateRagdollComponent::HandleRecovered);
}

void UDWBodyStateRagdollComponent::HandleDeath()
{
	EnterRagdoll();
}

void UDWBodyStateRagdollComponent::HandleDowned()
{
	EnterRagdoll();
}

void UDWBodyStateRagdollComponent::HandleRecovered()
{
	ExitRagdoll();
	BeginGetUp();
}

void UDWBodyStateRagdollComponent::EnterRagdoll()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		return;
	}
	
	if (UCharacterMovementComponent* Movement = OwnerCharacter->GetCharacterMovement())
	{
		Movement->DisableMovement();
		Movement->StopMovementImmediately();
	}
	
	USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh();
	if (Mesh)
	{
		Mesh->SetCollisionProfileName(RagdollCollisionProfileName);
		Mesh->SetSimulatePhysics(true);
		Mesh->WakeAllRigidBodies();
	}
	
	if (UCapsuleComponent* Capsule = OwnerCharacter->GetCapsuleComponent())
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void UDWBodyStateRagdollComponent::ExitRagdoll()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		return;
	}
	
	USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh();
	UCapsuleComponent* Capsule = OwnerCharacter->GetCapsuleComponent();
	
	if (Mesh && Capsule)
	{
		const FVector RecoveryBoneLocation = Mesh->GetBoneLocation(RecoveryBoneName);
		const float CapsuleHalfHeight = Capsule->GetScaledCapsuleHalfHeight();
		
		FVector TargetActorLocation = RecoveryBoneLocation + FVector(0.0f, 0.0f, CapsuleHalfHeight);
		
		if (UWorld* World = GetWorld())
		{
			FHitResult GroundHit;
			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(OwnerCharacter);
			
			const FVector TraceStart = RecoveryBoneLocation + FVector(0.0f, 0.0f, 200.0f);
			const FVector TraceEnd = RecoveryBoneLocation - FVector(0.0f, 0.0f, 500.0f);
			
			if (World->LineTraceSingleByChannel(GroundHit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
			{
				TargetActorLocation = GroundHit.ImpactPoint + FVector(0.0f, 0.0f, CapsuleHalfHeight);
			}
		}
		
		OwnerCharacter->SetActorLocation(TargetActorLocation, false, nullptr, ETeleportType::TeleportPhysics);
	}
	
	if (Mesh)
	{
		Mesh->SetAllBodiesPhysicsBlendWeight(0.0f);
		Mesh->SetSimulatePhysics(false);
		Mesh->SetAllBodiesSimulatePhysics(false);
		Mesh->PutAllRigidBodiesToSleep();
		Mesh->ResetAllBodiesSimulatePhysics();

		Mesh->SetCollisionProfileName(TEXT("CharacterMesh"));
		Mesh->AttachToComponent(
			OwnerCharacter->GetCapsuleComponent(),
			FAttachmentTransformRules::KeepRelativeTransform
		);

		Mesh->SetRelativeTransform(InitialMeshRelativeTransform);
		
		if (Mesh->GetAnimationMode() != EAnimationMode::AnimationBlueprint)
		{
			Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		}
	}
}

void UDWBodyStateRagdollComponent::BeginGetUp()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		return;
	}

	float PlayedGetUpDuration = 0.1f;
	float MovementLockDuration = 0.1f;
	
	UAnimMontage* SelectedGetUpMontage = SelectGetUpMontage();
	USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh();
	if (Mesh && SelectedGetUpMontage)
	{
		if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
		{
			const float PlayedDuration = AnimInstance->Montage_Play(SelectedGetUpMontage, GetUpPlayRate);

			if (PlayedDuration > 0.0f)
			{
				const float SafePlayRate = FMath::Max(0.1f, GetUpPlayRate);

				PlayedGetUpDuration = SelectedGetUpMontage->GetPlayLength() / SafePlayRate;
				MovementLockDuration = PlayedGetUpDuration + GetUpFinishDelayBuffer;
			}
		}
	}
	
	if (UDWBodyHealthComponent* BodyHealth = OwnerCharacter->FindComponentByClass<UDWBodyHealthComponent>())
	{
		BodyHealth->StartRecoveryGracePeriod(PlayedGetUpDuration + RecoveryGraceBuffer);
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(GetUpTimerHandle);
		World->GetTimerManager().SetTimer(
			GetUpTimerHandle,
			this,
			&UDWBodyStateRagdollComponent::FinishGetUp,
			MovementLockDuration,
			false
		);
	}
}

void UDWBodyStateRagdollComponent::FinishGetUp()
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		return;
	}
	
	if (UCapsuleComponent* Capsule = OwnerCharacter->GetCapsuleComponent())
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	
	if (UCharacterMovementComponent* Movement = OwnerCharacter->GetCharacterMovement())
	{
		Movement->SetMovementMode(MOVE_Walking);
	}
}

UAnimMontage* UDWBodyStateRagdollComponent::SelectGetUpMontage() const
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		return GetUpFrontMontage ? GetUpFrontMontage : GetUpBackMontage;
	}

	USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh();
	if (!Mesh)
	{
		return GetUpFrontMontage ? GetUpFrontMontage : GetUpBackMontage;
	}

	const FName ChestBoneName = TEXT("spine_03");
	const FVector ChestUp = Mesh->GetSocketTransform(ChestBoneName, RTS_World).GetUnitAxis(EAxis::Z);
	const float UpDot = FVector::DotProduct(ChestUp, FVector::UpVector);

	if (UpDot >= 0.0f && GetUpFrontMontage)
	{
		return GetUpFrontMontage;
	}

	if (GetUpBackMontage)
	{
		return GetUpBackMontage;
	}

	return GetUpFrontMontage;
}

void UDWBodyStateRagdollComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

