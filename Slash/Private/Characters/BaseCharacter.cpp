// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Components/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// 기본 캐릭터 설정
ABaseCharacter::ABaseCharacter()
{
	// 이 액터가 매 프레임마다 Tick() 함수를 호출하도록 설정합니다.
	PrimaryActorTick.bCanEverTick = true;

	// Attributes 컴포넌트를 기본 서브오브젝트로 생성합니다.
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	// Capsule 컴포넌트의 충돌 응답을 카메라 채널에 대해 무시하도록 설정합니다.
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECCollisionResponse::ECR_Ignore);
}

// 게임이 시작하거나 캐릭터가 생성될 때 호출되는 함수
void ABaseCharacter::BeginPlay()
{
	// 부모 클래스의 BeginPlay() 함수를 호출합니다.
	Super::BeginPlay();
}

// 충돌 처리를 위한 함수. FVector 타입의 충격 지점과 AActor 타입의 히터를 인자로 받습니다.
void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	// 캐릭터가 살아있고, 히터가 있다면 충돌 반응을 실행합니다.
	if (IsAlive() && Hitter)
	{
		DirectionalHitReact(Hitter->GetActorLocation());
	}
	else // 그렇지 않다면 캐릭터를 죽입니다.
		Die();

	// 충돌 사운드와 파티클을 생성합니다.
	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}

// 캐릭터의 공격 함수
void ABaseCharacter::Attack()
{
	// 만약 전투 대상이 존재하고, 그 대상이 죽었다면 전투 대상을 제거합니다.
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead"))) {
		CombatTarget = nullptr;
	}
}

// 캐릭터의 죽음을 처리하는 함수
void ABaseCharacter::Die_Implementation()
{
	// "Dead" 태그를 추가합니다.
	Tags.Add(FName("Dead"));

	// 죽음 모션을 실행합니다.
	PlayDeathMontage();
}

// 충돌 반응 모션을 재생하는 함수. 모션의 섹션 이름을 인자로 받습니다.
void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	// 애니메이션 인스턴스를 받아옵니다.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	// 애니메이션 인스턴스와 충돌 반응 모션 모두 유효하다면 충돌 반응 모션을 재생합니다.
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}


// 방향성 충격 반응 처리 함수. 충격 지점인 FVector 타입의 ImpactPoint를 인자로 받습니다.
void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	// 캐릭터의 전방 벡터를 가져옵니다.
	const FVector Forward = GetActorForwardVector();

	// 충격 지점의 Z 좌표를 캐릭터의 위치 Z 좌표로 낮춥니다.
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);

	// 충격 지점으로의 벡터를 계산하고, 안전하게 정규화합니다.
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// 두 벡터 사이의 각도를 코사인으로 계산합니다.
	const double CosTheta = FVector::DotProduct(Forward, ToHit);

	// 코사인 각도를 아크코사인으로 변환해 실제 각도(라디안)를 얻습니다.
	double Theta = FMath::Acos(CosTheta);

	// 라디안을 도로 변환합니다.
	Theta = FMath::RadiansToDegrees(Theta);

	// 벡터의 외적이 Z 방향으로 향하면, 각도는 음수가 됩니다.
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("FromBack");

	// 각도에 따라 섹션 이름을 결정합니다.
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

	// 결정된 섹션에 따라 충격 반응 모션을 재생합니다.
	PlayHitReactMontage(Section);
}

// 충격 사운드 재생 함수. 충격 지점인 FVector 타입의 ImpactPoint를 인자로 받습니다.
void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	// 충격 사운드가 설정되어 있다면, 그 위치에서 사운드를 재생합니다.
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}

// 충격 파티클 생성 함수. 충격 지점인 FVector 타입의 ImpactPoint를 인자로 받습니다.
void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	// 충격 파티클이 설정되어 있고, 월드가 유효하다면, 그 위치에서 파티클을 생성합니다.
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}


// 데미지 처리 함수. 인자로 받은 데미지 양(float 타입의 DamageAmount)을 속성에 적용합니다.
void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes)
	{
		// 속성이 있으면, 해당 속성의 데미지 수신 함수를 호출합니다.
		Attributes->ReceiveDamage(DamageAmount);
	}
}

// 애니메이션 몽타주 섹션 재생 함수. 인자로 애니메이션 몽타주와 섹션 이름을 받습니다.
void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	// 메쉬의 애니메이션 인스턴스를 얻습니다.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		// 애니메이션 인스턴스와 몽타주가 유효하면, 몽타주를 재생하고 해당 섹션으로 이동합니다.
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

// 무작위 몽타주 섹션 재생 함수. 인자로 애니메이션 몽타주와 섹션 이름들을 받습니다.
int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	// 섹션 이름이 없으면 -1을 반환합니다.
	if (SectionNames.Num() <= 0) return -1;

	// 무작위 섹션을 선택하고, 해당 섹션을 재생합니다.
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

// 공격 애니메이션 몽타주 재생 함수.
int32 ABaseCharacter::PlayAttackMontage()
{
	// 무작위 섹션을 재생하는 함수를 호출하고, 선택된 섹션의 인덱스를 반환합니다.
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

// 사망 애니메이션 몽타주 재생 함수.
int32 ABaseCharacter::PlayDeathMontage()
{
	// 무작위 섹션을 재생하는 함수를 호출하고, 사망 포즈를 설정합니다.
	const int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	TEnumAsByte<EDeathPose> Pose(Selection);
	if (Pose < EDeathPose::EDP_MAX)
	{
		DeathPose = Pose;
	}
	return Selection;
}

// 회피 애니메이션 몽타주 재생 함수.
void ABaseCharacter::PlayDodgeMontage()
{
	// "Default" 섹션을 재생합니다.
	PlayMontageSection(DodgeMontage, FName("Default"));
}

// 공격 애니메이션 몽타주 중지 함수.
void ABaseCharacter::StopAttackMontage()
{
	// 메쉬의 애니메이션 인스턴스를 얻습니다.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) {
		// 애니메이션 인스턴스가 유효하면, 공격 몽타주를 중지합니다.
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
	}
}


// 이동 전송 대상의 위치를 계산하는 함수
FVector ABaseCharacter::GetTranslationWarpTarget()
{
	// 전투 대상이 없으면 0 벡터를 반환합니다.
	if (CombatTarget == nullptr) return FVector();

	// 전투 대상과 자신의 위치를 계산합니다.
	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();

	// 전투 대상으로부터 자신까지의 방향 벡터를 계산하고, 그 길이를 전송 대상 거리로 조정합니다.
	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;

	// 전송 대상 위치를 계산하여 반환합니다.
	return CombatTargetLocation + TargetToMe;
}

// 회전 전송 대상의 위치를 반환하는 함수
FVector ABaseCharacter::GetRotationWarpTarget()
{
	// 전투 대상이 있으면 그 위치를 반환하고, 없으면 0 벡터를 반환합니다.
	if (CombatTarget) {
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}

// 캡슐의 충돌을 비활성화하는 함수
void ABaseCharacter::DisableCapsule()
{
	// 캡슐 컴포넌트의 충돌을 비활성화합니다.
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// 공격 가능 여부를 반환하는 함수
bool ABaseCharacter::CanAttack()
{
	// 항상 false를 반환합니다.
	return false;
}

// 살아있는지 여부를 반환하는 함수
bool ABaseCharacter::IsAlive()
{
	// 속성이 유효하고, 속성의 IsAlive 함수가 true를 반환하면 true를 반환합니다.
	return Attributes && Attributes->IsAlive();
}

// 메쉬의 충돌을 비활성화하는 함수
void ABaseCharacter::DisableMeshCollision()
{
	// 메쉬의 충돌을 비활성화합니다.
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// 공격 종료 함수
void ABaseCharacter::AttackEnd()
{
	// 아무 것도 수행하지 않습니다.
}

// 회피 종료 함수
void ABaseCharacter::DodgeEnd()
{
	// 아무 것도 수행하지 않습니다.
}

// 매 프레임 호출되는 함수
void ABaseCharacter::Tick(float DeltaTime)
{
	// 부모 클래스의 Tick 함수를 호출합니다.
	Super::Tick(DeltaTime);
}

// 무기의 충돌 활성화 상태를 설정하는 함수. 인자로 충돌 활성화 타입을 받습니다.
void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	// 무기가 장착되어 있고, 무기 상자가 유효하면, 무기 상자의 충돌 활성화 상태를 설정합니다.
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}
