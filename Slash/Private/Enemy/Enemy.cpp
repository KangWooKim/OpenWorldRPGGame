// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Items/Weapons/Weapon.h"
#include "Items/Soul.h"

AEnemy::AEnemy()
{
	// 이 액터가 매 틱마다 업데이트 되도록 설정합니다.
	PrimaryActorTick.bCanEverTick = true;

	// 메시의 충돌 설정을 정의합니다.
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECCollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECCollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	// HealthBarWidget 생성 및 부착
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	// 캐릭터의 움직임 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// PawnSensing 컴포넌트 설정
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
}

void ANemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 죽은 상태면 Tick 종료
	if (IsDead()) return;
	// 상태에 따라 적을 감지
	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 데미지 처리
	HandleDamage(DamageAmount);
	// 공격자를 타겟으로 설정
	CombatTarget = EventInstigator->GetPawn();

	// 공격 반경 내에 있으면 공격, 아니면 추적
	if (IsInsideAttackRadius()) {
		EnemyState = EEnemyState::EES_Attacking;
	}
	else if (IsOutsideAttackRadius()) {
		ChaseTarget();
	}

	return DamageAmount;
}

void AEnemy::Destroyed()
{
	// 파괴될 때 무기도 함께 파괴
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}


void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	// 부모 클래스의 GetHit 메소드를 호출
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	// 적이 죽지 않았다면 체력 바를 표시
	if (!IsDead()) ShowHealthBar();

	// 순찰 타이머와 공격 타이머를 초기화
	ClearPatrolTimer();
	ClearAttackTimer();

	// 무기의 충돌을 비활성화
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

	// 공격 몽타주를 중지
	StopAttackMontage();

	// 공격 범위 내에 있다면
	if (IsInsideAttackRadius()) {
		// 적이 죽지 않았다면 공격 타이머를 시작
		if (!IsDead()) {
			StartAttackTimer();
		}
	}
}

void ANemy::BeginPlay()
{
	// 부모 클래스의 BeginPlay 메소드를 호출
	Super::BeginPlay();

	// PawnSensing이 존재하면 PawnSeen 이벤트를 연결
	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);

	// 적을 초기화
	InitializeEnemy();

	// 태그 설정
	Tags.Add(FName("Enemy"));
}

void AEnemy::Die_Implementation()
{
	// 부모 클래스의 Die 메소드를 호출
	Super::Die_Implementation();

	// 적의 상태를 '죽음'으로 설정
	EnemyState = EEnemyState::EES_Dead;

	// 공격 타이머를 초기화하고 체력 바를 숨김
	ClearAttackTimer();
	HideHealthBar();

	// 캡슐을 비활성화
	DisableCapsule();

	// 죽은 후 일정 시간 후에 제거
	SetLifeSpan(DeathLifeSpan);

	// 움직임에 따른 회전을 비활성화
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// 무기의 충돌을 비활성화
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

	// 소울 생성
	SpawnSoul();
}

void ANemy::SpawnSoul()
{
	// 소울 클래스와 속성이 존재하면 소울을 생성
	UWorld* World = GetWorld();
	if (World && SoulClass && Attributes) {
		const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 125.f);
		ASoul* SpawnedSoul = World->SpawnActor<ASoul>(SoulClass, SpawnLocation, GetActorRotation());
		if (SpawnedSoul) {
			SpawnedSoul->SetSouls(Attributes->GetSouls());
			SpawnedSoul->SetOwner(this);
		}
	}
}

void ANemy::Attack()
{
	// 부모 클래스의 Attack 메소드를 호출
	Super::Attack();

	// 타겟이 없다면 함수 종료
	if (CombatTarget == nullptr) return;

	// 적의 상태를 '전투 중'으로 설정하고 공격 몽타주를 재생
	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();
}

bool ANemy::CanAttack()
{
	// 공격 가능 여부 판단
	bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();

	return bCanAttack;
}


void ANemy::AttackEnd()
{
	// 공격이 끝난 후 적의 상태를 'NoState'로 설정하고 적의 대상을 확인
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

void AEnemy::HandleDamage(float DamageAmount)
{
	// 부모 클래스의 HandleDamage 메소드를 호출
	Super::HandleDamage(DamageAmount);

	// 속성과 체력바 위젯이 있으면 체력바의 퍼센트를 설정
	if (Attributes && HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}


void AEnemy::InitializeEnemy()
{
	// 적의 컨트롤러를 설정하고 대상으로 이동, 체력바를 숨기고 기본 무기를 생성
	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);
	HideHealthBar();
	SpawnDefaultWeapon();
}

void ANemy::CheckPatrolTarget()
{
	// 타겟의 범위 내에 있다면 순찰 대상을 선택하고 대기 시간을 설정한 후 타이머를 시작
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
	}
}

void ANemy::CheckCombatTarget()
{
	// 전투 범위 밖이면 공격 타이머를 초기화하고 관심을 잃고 순찰을 시작
	// 공격 범위 밖이고 추적 중이 아니면 추적을 시작
	// 공격 가능하면 공격 타이머를 시작
	if (IsOutsideCombatRadius())
	{
		ClearAttackTimer();
		LoseInterest();
		if (!IsEngaged()) StartPatrolling();
	}
	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		ClearAttackTimer();
		if (!IsEngaged()) ChaseTarget();
	}
	else if (CanAttack())
	{
		StartAttackTimer();
	}
}

void AEnemy::PatrolTimerFinished()
{
	// 순찰 타이머가 끝나면 대상으로 이동
	MoveToTarget(PatrolTarget);
}

void ANemy::HideHealthBar()
{
	// 체력바 위젯이 있으면 체력바를 숨김
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void ANemy::ShowHealthBar()
{
	// 체력바 위젯이 있으면 체력바를 표시
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

void AEnemy::LoseInterest()
{
	// 전투 대상을 없애고 체력바를 숨김
	CombatTarget = nullptr;
	HideHealthBar();
}


void ANemy::StartPatrolling()
{
	// 적의 상태를 순찰 상태로 설정하고, 이동 속도를 순찰 속도로 설정한 후, 순찰 대상으로 이동
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(PatrolTarget);
}

void ANemy::ChaseTarget()
{
	// 적의 상태를 추적 상태로 설정하고, 이동 속도를 추적 속도로 설정한 후, 전투 대상으로 이동
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

bool AEnemy::IsOutsideCombatRadius()
{
	// 전투 대상이 전투 범위 밖인지 확인
	return !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
	// 전투 대상이 공격 범위 밖인지 확인
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsInsideAttackRadius()
{
	// 전투 대상이 공격 범위 내에 있는지 확인
	return InTargetRange(CombatTarget, AttackRadius);
}

bool ANemy::IsChasing()
{
	// 적의 상태가 추적 상태인지 확인
	return EnemyState == EEnemyState::EES_Chasing;
}

bool ANemy::IsAttacking()
{
	// 적의 상태가 공격 상태인지 확인
	return EnemyState == EEnemyState::EES_Attacking;
}

bool ANemy::IsDead()
{
	// 적의 상태가 사망 상태인지 확인
	return EnemyState == EEnemyState::EES_Dead;
}

bool AEnemy::IsEngaged()
{
	// 적의 상태가 참여 상태인지 확인
	return EnemyState == EEnemyState::EES_Engaged;
}

void ANemy::ClearPatrolTimer()
{
	// 순찰 타이머를 초기화
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void ANemy::StartAttackTimer()
{
	// 적의 상태를 공격 상태로 설정하고, 공격 시간을 무작위로 설정한 후 공격 타이머를 시작
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

void ANemy::ClearAttackTimer()
{
	// 공격 타이머를 초기화
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	// 대상이 범위 내에 있는지 확인. 대상이 null인 경우, false 반환
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	// 컨트롤러나 대상이 null이 아닌 경우, 대상으로 이동
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
	EnemyController->MoveTo(MoveRequest);
}


AActor* AEnemy::ChoosePatrolTarget()
{
	// 순찰 대상들 중에서 현재 순찰 대상이 아닌 대상들을 검색하여 유효한 대상 배열에 추가
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	// 유효한 순찰 대상들 중에서 무작위로 하나를 선택하여 반환, 유효한 대상이 없는 경우 nullptr 반환
	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}

void AEnemy::SpawnDefaultWeapon()
{
	// 월드와 무기 클래스가 유효한 경우, 기본 무기를 생성하고 장착
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket"), this, this);
		EquippedWeapon = DefaultWeapon;
	}
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	// 적이 사망 상태가 아니고, 추적 상태가 아니며, 공격 상태 미만이며, 발견된 폰이 EngageableTarget 태그를 가지고 있는 경우
	// 발견된 폰을 전투 대상으로 설정하고, 순찰 타이머를 초기화한 후, 전투 대상을 추적
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking&&
		SeenPawn->ActorHasTag(FName("EngageableTarget"));

	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		ChaseTarget();
	}
}
