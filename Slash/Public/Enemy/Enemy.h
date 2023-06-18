// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class UPawnSensingComponent;

UCLASS()
class SLASH_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ANemy();

	/** <AActor> */
	// �� �����Ӹ��� ������Ʈ �Ǵ� �Լ�, DeltaTime�� ���� ������ ������ �ð��� �� ������ ��Ÿ��
	virtual void Tick(float DeltaTime) override;

	// ĳ���Ͱ� �������� ���� �� ȣ��Ǵ� �Լ�
	// DamageAmount : ���� ������ ��
	// DamageEvent : �������� ���� �̺�Ʈ�� ���� �� ����
	// EventInstigator : �� �̺�Ʈ�� �ߵ���Ų ��Ʈ�ѷ�
	// DamageCauser : �������� ���� ����
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// ���Ͱ� �ı��� �� ȣ��Ǵ� �Լ�
	virtual void Destroyed() override;
	/** </AActor> */

	/** <IHitInterface> */
	// �ش� ���Ͱ� Ÿ�ݴ����� �� ȣ��Ǵ� �Լ�
	// ImpactPoint : Ÿ�� ����
	// Hitter : Ÿ���� ����
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/** </IHitInterface> */

protected:
	/** <AActor> */
	// ���� ���� �� �� �� ȣ��Ǵ� �Լ�
	virtual void BeginPlay() override;
	/** </AActor> */

	/** <ABaseCharacter> */
	// ĳ���Ͱ� ������� �� ȣ��Ǵ� �Լ�
	virtual void Die_Implementation() override;

	// ��ȥ ���� �Լ�
	void SpawnSoul();

	// ���� ���� �Լ�
	virtual void Attack() override;

	// ���� ���� ���θ� �Ǵ��ϴ� �Լ�
	virtual bool CanAttack() override;

	// ���� ���� �Լ�
	virtual void AttackEnd() override;

	// ������ ó�� �Լ�, DamageAmount�� ó���� ������ ��
	virtual void HandleDamage(float DamageAmount) override;
	/** </ABaseCharacter> */

	// ���� ���¸� ��Ÿ���� ����, �⺻������ ���� ���·� ����
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		EEnemyState EnemyState = EENemyState::EES_Patrolling;


private:

	/** AI Behavior */

	// �� AI �ʱ�ȭ �Լ�
	void InitializeEnemy();

	// ���� ��ǥ �˻� �Լ�
	void CheckPatrolTarget();

	// ���� ��ǥ �˻� �Լ�
	void CheckCombatTarget();

	// ���� Ÿ�̸� ���� �Լ�
	void PatrolTimerFinished();

	// ü�¹� ����� �Լ�
	void HideHealthBar();

	// ü�¹� ���̱� �Լ�
	void ShowHealthBar();

	// ���� ��ǥ�� ���� ������ ���� �� ȣ���ϴ� �Լ�
	void LoseInterest();

	// ���� ���� �Լ�
	void StartPatrolling();

	// ��ǥ �߰� ���� �Լ�
	void ChaseTarget();

	// ��ǥ�� ���� ���� �ۿ� �ִ��� Ȯ���ϴ� �Լ�
	bool IsOutsideCombatRadius();

	// ��ǥ�� ���� ���� �ۿ� �ִ��� Ȯ���ϴ� �Լ�
	bool IsOutsideAttackRadius();

	// ��ǥ�� ���� ���� �ȿ� �ִ��� Ȯ���ϴ� �Լ�
	bool IsInsideAttackRadius();

	// ���� �߰� ������ Ȯ���ϴ� �Լ�
	bool IsChasing();

	// ���� ���� ������ Ȯ���ϴ� �Լ�
	bool IsAttacking();

	// ���� ���� �������� Ȯ���ϴ� �Լ�
	bool IsDead();

	// ���� ��ǥ�� �ο�� �ִ��� Ȯ���ϴ� �Լ�
	bool IsEngaged();

	// ���� Ÿ�̸Ӹ� ������Ű�� �Լ�
	void ClearPatrolTimer();

	// ���� Ÿ�̸Ӹ� �����ϴ� �Լ�
	void StartAttackTimer();

	// ���� Ÿ�̸Ӹ� ������Ű�� �Լ�
	void ClearAttackTimer();

	// ��ǥ�� Ư�� �ݰ� ���� �ִ��� Ȯ���ϴ� �Լ�
	// Target : ��ǥ ����, Radius : �ݰ�
	bool InTargetRange(AActor* Target, double Radius);

	// ��ǥ���� �̵��ϴ� �Լ�
	// Target : ��ǥ ����
	void MoveToTarget(AActor* Target);

	// ���� ��ǥ ���� �Լ�
	AActor* ChoosePatrolTarget();

	// �⺻ ���� ���� �Լ�
	void SpawnDefaultWeapon();


	UFUNCTION()
		// ������ ���� ó���ϴ� �Լ�
		// SeenPawn : ������ ��
		void PawnSeen(APawn* SeenPawn); // Callback for OnPawnSeen in UPawnSensingComponent

	UPROPERTY(VisibleAnywhere)
		// ü�¹� ����
		UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
		// Pawn ���� ������Ʈ
		UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
		// ����� ���� Ŭ����
		TSubclassOf<class AWeapon> WeaponClass;

	

	UPROPERTY(EditAnywhere, Category = Combat)
		double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere, Category = Combat)
		double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = Combat)
		double AcceptanceRadius = 50.f;

	UPROPERTY()
		class AAIController* EnemyController;

	// �� AI�� ���� ���� ��ǥ
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		AActor* PatrolTarget;

	// ������ ��ǥ�� �迭
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		TArray<AActor*> PatrolTargets;

	// ���� �ݰ�
	UPROPERTY(EditAnywhere)
		double PatrolRadius = 200.f;

	// ���� Ÿ�̸� �ڵ�
	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float PatrolWaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float PatrolWaitMax = 10.f;

	UPROPERTY(EditAnywhere, Category = Combat)
		float PatrollingSpeed = 125.f;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
		float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = Combat)
		float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = Combat)
		float ChasingSpeed = 300.f;

	// ���� ���� �� ������� �� �ɸ��� �ð�
	UPROPERTY(EditAnywhere, Category = Combat)
		float DeathLifeSpan = 8.f;

	// ������ ��ȥ Ŭ����
	UPROPERTY(EditAnywhere, Category = Combat)
		TSubclassOf<class ASoul> SoulClass;
};