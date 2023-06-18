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
	// �� ���Ͱ� �� ƽ���� ������Ʈ �ǵ��� �����մϴ�.
	PrimaryActorTick.bCanEverTick = true;

	// �޽��� �浹 ������ �����մϴ�.
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECCollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECCollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	// HealthBarWidget ���� �� ����
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	// ĳ������ ������ ����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// PawnSensing ������Ʈ ����
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
}

void ANemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// ���� ���¸� Tick ����
	if (IsDead()) return;
	// ���¿� ���� ���� ����
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
	// ������ ó��
	HandleDamage(DamageAmount);
	// �����ڸ� Ÿ������ ����
	CombatTarget = EventInstigator->GetPawn();

	// ���� �ݰ� ���� ������ ����, �ƴϸ� ����
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
	// �ı��� �� ���⵵ �Բ� �ı�
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}


void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	// �θ� Ŭ������ GetHit �޼ҵ带 ȣ��
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	// ���� ���� �ʾҴٸ� ü�� �ٸ� ǥ��
	if (!IsDead()) ShowHealthBar();

	// ���� Ÿ�̸ӿ� ���� Ÿ�̸Ӹ� �ʱ�ȭ
	ClearPatrolTimer();
	ClearAttackTimer();

	// ������ �浹�� ��Ȱ��ȭ
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

	// ���� ��Ÿ�ָ� ����
	StopAttackMontage();

	// ���� ���� ���� �ִٸ�
	if (IsInsideAttackRadius()) {
		// ���� ���� �ʾҴٸ� ���� Ÿ�̸Ӹ� ����
		if (!IsDead()) {
			StartAttackTimer();
		}
	}
}

void ANemy::BeginPlay()
{
	// �θ� Ŭ������ BeginPlay �޼ҵ带 ȣ��
	Super::BeginPlay();

	// PawnSensing�� �����ϸ� PawnSeen �̺�Ʈ�� ����
	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);

	// ���� �ʱ�ȭ
	InitializeEnemy();

	// �±� ����
	Tags.Add(FName("Enemy"));
}

void AEnemy::Die_Implementation()
{
	// �θ� Ŭ������ Die �޼ҵ带 ȣ��
	Super::Die_Implementation();

	// ���� ���¸� '����'���� ����
	EnemyState = EEnemyState::EES_Dead;

	// ���� Ÿ�̸Ӹ� �ʱ�ȭ�ϰ� ü�� �ٸ� ����
	ClearAttackTimer();
	HideHealthBar();

	// ĸ���� ��Ȱ��ȭ
	DisableCapsule();

	// ���� �� ���� �ð� �Ŀ� ����
	SetLifeSpan(DeathLifeSpan);

	// �����ӿ� ���� ȸ���� ��Ȱ��ȭ
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// ������ �浹�� ��Ȱ��ȭ
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

	// �ҿ� ����
	SpawnSoul();
}

void ANemy::SpawnSoul()
{
	// �ҿ� Ŭ������ �Ӽ��� �����ϸ� �ҿ��� ����
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
	// �θ� Ŭ������ Attack �޼ҵ带 ȣ��
	Super::Attack();

	// Ÿ���� ���ٸ� �Լ� ����
	if (CombatTarget == nullptr) return;

	// ���� ���¸� '���� ��'���� �����ϰ� ���� ��Ÿ�ָ� ���
	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();
}

bool ANemy::CanAttack()
{
	// ���� ���� ���� �Ǵ�
	bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();

	return bCanAttack;
}


void ANemy::AttackEnd()
{
	// ������ ���� �� ���� ���¸� 'NoState'�� �����ϰ� ���� ����� Ȯ��
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

void AEnemy::HandleDamage(float DamageAmount)
{
	// �θ� Ŭ������ HandleDamage �޼ҵ带 ȣ��
	Super::HandleDamage(DamageAmount);

	// �Ӽ��� ü�¹� ������ ������ ü�¹��� �ۼ�Ʈ�� ����
	if (Attributes && HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}


void AEnemy::InitializeEnemy()
{
	// ���� ��Ʈ�ѷ��� �����ϰ� ������� �̵�, ü�¹ٸ� ����� �⺻ ���⸦ ����
	EnemyController = Cast<AAIController>(GetController());
	MoveToTarget(PatrolTarget);
	HideHealthBar();
	SpawnDefaultWeapon();
}

void ANemy::CheckPatrolTarget()
{
	// Ÿ���� ���� ���� �ִٸ� ���� ����� �����ϰ� ��� �ð��� ������ �� Ÿ�̸Ӹ� ����
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
	}
}

void ANemy::CheckCombatTarget()
{
	// ���� ���� ���̸� ���� Ÿ�̸Ӹ� �ʱ�ȭ�ϰ� ������ �Ұ� ������ ����
	// ���� ���� ���̰� ���� ���� �ƴϸ� ������ ����
	// ���� �����ϸ� ���� Ÿ�̸Ӹ� ����
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
	// ���� Ÿ�̸Ӱ� ������ ������� �̵�
	MoveToTarget(PatrolTarget);
}

void ANemy::HideHealthBar()
{
	// ü�¹� ������ ������ ü�¹ٸ� ����
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void ANemy::ShowHealthBar()
{
	// ü�¹� ������ ������ ü�¹ٸ� ǥ��
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

void AEnemy::LoseInterest()
{
	// ���� ����� ���ְ� ü�¹ٸ� ����
	CombatTarget = nullptr;
	HideHealthBar();
}


void ANemy::StartPatrolling()
{
	// ���� ���¸� ���� ���·� �����ϰ�, �̵� �ӵ��� ���� �ӵ��� ������ ��, ���� ������� �̵�
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;
	MoveToTarget(PatrolTarget);
}

void ANemy::ChaseTarget()
{
	// ���� ���¸� ���� ���·� �����ϰ�, �̵� �ӵ��� ���� �ӵ��� ������ ��, ���� ������� �̵�
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;
	MoveToTarget(CombatTarget);
}

bool AEnemy::IsOutsideCombatRadius()
{
	// ���� ����� ���� ���� ������ Ȯ��
	return !InTargetRange(CombatTarget, CombatRadius);
}

bool AEnemy::IsOutsideAttackRadius()
{
	// ���� ����� ���� ���� ������ Ȯ��
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsInsideAttackRadius()
{
	// ���� ����� ���� ���� ���� �ִ��� Ȯ��
	return InTargetRange(CombatTarget, AttackRadius);
}

bool ANemy::IsChasing()
{
	// ���� ���°� ���� �������� Ȯ��
	return EnemyState == EEnemyState::EES_Chasing;
}

bool ANemy::IsAttacking()
{
	// ���� ���°� ���� �������� Ȯ��
	return EnemyState == EEnemyState::EES_Attacking;
}

bool ANemy::IsDead()
{
	// ���� ���°� ��� �������� Ȯ��
	return EnemyState == EEnemyState::EES_Dead;
}

bool AEnemy::IsEngaged()
{
	// ���� ���°� ���� �������� Ȯ��
	return EnemyState == EEnemyState::EES_Engaged;
}

void ANemy::ClearPatrolTimer()
{
	// ���� Ÿ�̸Ӹ� �ʱ�ȭ
	GetWorldTimerManager().ClearTimer(PatrolTimer);
}

void ANemy::StartAttackTimer()
{
	// ���� ���¸� ���� ���·� �����ϰ�, ���� �ð��� �������� ������ �� ���� Ÿ�̸Ӹ� ����
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

void ANemy::ClearAttackTimer()
{
	// ���� Ÿ�̸Ӹ� �ʱ�ȭ
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	// ����� ���� ���� �ִ��� Ȯ��. ����� null�� ���, false ��ȯ
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	// ��Ʈ�ѷ��� ����� null�� �ƴ� ���, ������� �̵�
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
	EnemyController->MoveTo(MoveRequest);
}


AActor* AEnemy::ChoosePatrolTarget()
{
	// ���� ���� �߿��� ���� ���� ����� �ƴ� ������ �˻��Ͽ� ��ȿ�� ��� �迭�� �߰�
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	// ��ȿ�� ���� ���� �߿��� �������� �ϳ��� �����Ͽ� ��ȯ, ��ȿ�� ����� ���� ��� nullptr ��ȯ
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
	// ����� ���� Ŭ������ ��ȿ�� ���, �⺻ ���⸦ �����ϰ� ����
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
	// ���� ��� ���°� �ƴϰ�, ���� ���°� �ƴϸ�, ���� ���� �̸��̸�, �߰ߵ� ���� EngageableTarget �±׸� ������ �ִ� ���
	// �߰ߵ� ���� ���� ������� �����ϰ�, ���� Ÿ�̸Ӹ� �ʱ�ȭ�� ��, ���� ����� ����
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
