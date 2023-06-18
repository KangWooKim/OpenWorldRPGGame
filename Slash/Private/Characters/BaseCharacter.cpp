// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// �⺻ ĳ���� ����
ABaseCharacter::ABaseCharacter()
{
	// �� ���Ͱ� �� �����Ӹ��� Tick() �Լ��� ȣ���ϵ��� �����մϴ�.
	PrimaryActorTick.bCanEverTick = true;

	// Attributes ������Ʈ�� �⺻ ���������Ʈ�� �����մϴ�.
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	// Capsule ������Ʈ�� �浹 ������ ī�޶� ä�ο� ���� �����ϵ��� �����մϴ�.
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECCollisionResponse::ECR_Ignore);
}

// ������ �����ϰų� ĳ���Ͱ� ������ �� ȣ��Ǵ� �Լ�
void ABaseCharacter::BeginPlay()
{
	// �θ� Ŭ������ BeginPlay() �Լ��� ȣ���մϴ�.
	Super::BeginPlay();
}

// �浹 ó���� ���� �Լ�. FVector Ÿ���� ��� ������ AActor Ÿ���� ���͸� ���ڷ� �޽��ϴ�.
void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	// ĳ���Ͱ� ����ְ�, ���Ͱ� �ִٸ� �浹 ������ �����մϴ�.
	if (IsAlive() && Hitter)
	{
		DirectionalHitReact(Hitter->GetActorLocation());
	}
	else // �׷��� �ʴٸ� ĳ���͸� ���Դϴ�.
		Die();

	// �浹 ����� ��ƼŬ�� �����մϴ�.
	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

// ĳ������ ���� �Լ�
void ABaseCharacter::Attack()
{
	// ���� ���� ����� �����ϰ�, �� ����� �׾��ٸ� ���� ����� �����մϴ�.
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead"))) {
		CombatTarget = nullptr;
	}
}

// ĳ������ ������ ó���ϴ� �Լ�
void ABaseCharacter::Die_Implementation()
{
	// "Dead" �±׸� �߰��մϴ�.
	Tags.Add(FName("Dead"));

	// ���� ����� �����մϴ�.
	PlayDeathMontage();
}

// �浹 ���� ����� ����ϴ� �Լ�. ����� ���� �̸��� ���ڷ� �޽��ϴ�.
void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	// �ִϸ��̼� �ν��Ͻ��� �޾ƿɴϴ�.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	// �ִϸ��̼� �ν��Ͻ��� �浹 ���� ��� ��� ��ȿ�ϴٸ� �浹 ���� ����� ����մϴ�.
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}


// ���⼺ ��� ���� ó�� �Լ�. ��� ������ FVector Ÿ���� ImpactPoint�� ���ڷ� �޽��ϴ�.
void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	// ĳ������ ���� ���͸� �����ɴϴ�.
	const FVector Forward = GetActorForwardVector();

	// ��� ������ Z ��ǥ�� ĳ������ ��ġ Z ��ǥ�� ����ϴ�.
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);

	// ��� ���������� ���͸� ����ϰ�, �����ϰ� ����ȭ�մϴ�.
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// �� ���� ������ ������ �ڻ������� ����մϴ�.
	const double CosTheta = FVector::DotProduct(Forward, ToHit);

	// �ڻ��� ������ ��ũ�ڻ������� ��ȯ�� ���� ����(����)�� ����ϴ�.
	double Theta = FMath::Acos(CosTheta);

	// ������ ���� ��ȯ�մϴ�.
	Theta = FMath::RadiansToDegrees(Theta);

	// ������ ������ Z �������� ���ϸ�, ������ ������ �˴ϴ�.
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("FromBack");

	// ������ ���� ���� �̸��� �����մϴ�.
	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}

	// ������ ���ǿ� ���� ��� ���� ����� ����մϴ�.
	PlayHitReactMontage(Section);
}

// ��� ���� ��� �Լ�. ��� ������ FVector Ÿ���� ImpactPoint�� ���ڷ� �޽��ϴ�.
void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	// ��� ���尡 �����Ǿ� �ִٸ�, �� ��ġ���� ���带 ����մϴ�.
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}

// ��� ��ƼŬ ���� �Լ�. ��� ������ FVector Ÿ���� ImpactPoint�� ���ڷ� �޽��ϴ�.
void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	// ��� ��ƼŬ�� �����Ǿ� �ְ�, ���尡 ��ȿ�ϴٸ�, �� ��ġ���� ��ƼŬ�� �����մϴ�.
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}


// ������ ó�� �Լ�. ���ڷ� ���� ������ ��(float Ÿ���� DamageAmount)�� �Ӽ��� �����մϴ�.
void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes)
	{
		// �Ӽ��� ������, �ش� �Ӽ��� ������ ���� �Լ��� ȣ���մϴ�.
		Attributes->ReceiveDamage(DamageAmount);
	}
}

// �ִϸ��̼� ��Ÿ�� ���� ��� �Լ�. ���ڷ� �ִϸ��̼� ��Ÿ�ֿ� ���� �̸��� �޽��ϴ�.
void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	// �޽��� �ִϸ��̼� �ν��Ͻ��� ����ϴ�.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		// �ִϸ��̼� �ν��Ͻ��� ��Ÿ�ְ� ��ȿ�ϸ�, ��Ÿ�ָ� ����ϰ� �ش� �������� �̵��մϴ�.
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

// ������ ��Ÿ�� ���� ��� �Լ�. ���ڷ� �ִϸ��̼� ��Ÿ�ֿ� ���� �̸����� �޽��ϴ�.
int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	// ���� �̸��� ������ -1�� ��ȯ�մϴ�.
	if (SectionNames.Num() <= 0) return -1;

	// ������ ������ �����ϰ�, �ش� ������ ����մϴ�.
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

// ���� �ִϸ��̼� ��Ÿ�� ��� �Լ�.
int32 ABaseCharacter::PlayAttackMontage()
{
	// ������ ������ ����ϴ� �Լ��� ȣ���ϰ�, ���õ� ������ �ε����� ��ȯ�մϴ�.
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

// ��� �ִϸ��̼� ��Ÿ�� ��� �Լ�.
int32 ABaseCharacter::PlayDeathMontage()
{
	// ������ ������ ����ϴ� �Լ��� ȣ���ϰ�, ��� ��� �����մϴ�.
	const int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	TEnumAsByte<EDeathPose> Pose(Selection);
	if (Pose < EDeathPose::EDP_MAX)
	{
		DeathPose = Pose;
	}
	return Selection;
}

// ȸ�� �ִϸ��̼� ��Ÿ�� ��� �Լ�.
void ABaseCharacter::PlayDodgeMontage()
{
	// "Default" ������ ����մϴ�.
	PlayMontageSection(DodgeMontage, FName("Default"));
}

// ���� �ִϸ��̼� ��Ÿ�� ���� �Լ�.
void ABaseCharacter::StopAttackMontage()
{
	// �޽��� �ִϸ��̼� �ν��Ͻ��� ����ϴ�.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) {
		// �ִϸ��̼� �ν��Ͻ��� ��ȿ�ϸ�, ���� ��Ÿ�ָ� �����մϴ�.
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
	}
}


// �̵� ���� ����� ��ġ�� ����ϴ� �Լ�
FVector ABaseCharacter::GetTranslationWarpTarget()
{
	// ���� ����� ������ 0 ���͸� ��ȯ�մϴ�.
	if (CombatTarget == nullptr) return FVector();

	// ���� ���� �ڽ��� ��ġ�� ����մϴ�.
	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();

	// ���� ������κ��� �ڽű����� ���� ���͸� ����ϰ�, �� ���̸� ���� ��� �Ÿ��� �����մϴ�.
	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;

	// ���� ��� ��ġ�� ����Ͽ� ��ȯ�մϴ�.
	return CombatTargetLocation + TargetToMe;
}

// ȸ�� ���� ����� ��ġ�� ��ȯ�ϴ� �Լ�
FVector ABaseCharacter::GetRotationWarpTarget()
{
	// ���� ����� ������ �� ��ġ�� ��ȯ�ϰ�, ������ 0 ���͸� ��ȯ�մϴ�.
	if (CombatTarget) {
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}

// ĸ���� �浹�� ��Ȱ��ȭ�ϴ� �Լ�
void ABaseCharacter::DisableCapsule()
{
	// ĸ�� ������Ʈ�� �浹�� ��Ȱ��ȭ�մϴ�.
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// ���� ���� ���θ� ��ȯ�ϴ� �Լ�
bool ABaseCharacter::CanAttack()
{
	// �׻� false�� ��ȯ�մϴ�.
	return false;
}

// ����ִ��� ���θ� ��ȯ�ϴ� �Լ�
bool ABaseCharacter::IsAlive()
{
	// �Ӽ��� ��ȿ�ϰ�, �Ӽ��� IsAlive �Լ��� true�� ��ȯ�ϸ� true�� ��ȯ�մϴ�.
	return Attributes && Attributes->IsAlive();
}

// �޽��� �浹�� ��Ȱ��ȭ�ϴ� �Լ�
void ABaseCharacter::DisableMeshCollision()
{
	// �޽��� �浹�� ��Ȱ��ȭ�մϴ�.
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// ���� ���� �Լ�
void ABaseCharacter::AttackEnd()
{
	// �ƹ� �͵� �������� �ʽ��ϴ�.
}

// ȸ�� ���� �Լ�
void ABaseCharacter::DodgeEnd()
{
	// �ƹ� �͵� �������� �ʽ��ϴ�.
}

// �� ������ ȣ��Ǵ� �Լ�
void ABaseCharacter::Tick(float DeltaTime)
{
	// �θ� Ŭ������ Tick �Լ��� ȣ���մϴ�.
	Super::Tick(DeltaTime);
}

// ������ �浹 Ȱ��ȭ ���¸� �����ϴ� �Լ�. ���ڷ� �浹 Ȱ��ȭ Ÿ���� �޽��ϴ�.
void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	// ���Ⱑ �����Ǿ� �ְ�, ���� ���ڰ� ��ȿ�ϸ�, ���� ������ �浹 Ȱ��ȭ ���¸� �����մϴ�.
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}
