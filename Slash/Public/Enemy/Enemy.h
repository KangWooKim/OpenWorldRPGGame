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
	// 매 프레임마다 업데이트 되는 함수, DeltaTime은 이전 프레임 이후의 시간을 초 단위로 나타냄
	virtual void Tick(float DeltaTime) override;

	// 캐릭터가 데미지를 입을 때 호출되는 함수
	// DamageAmount : 입은 데미지 양
	// DamageEvent : 데미지를 입은 이벤트에 대한 상세 정보
	// EventInstigator : 이 이벤트를 발동시킨 컨트롤러
	// DamageCauser : 데미지를 입힌 액터
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// 액터가 파괴될 때 호출되는 함수
	virtual void Destroyed() override;
	/** </AActor> */

	/** <IHitInterface> */
	// 해당 액터가 타격당했을 때 호출되는 함수
	// ImpactPoint : 타격 지점
	// Hitter : 타격한 액터
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/** </IHitInterface> */

protected:
	/** <AActor> */
	// 게임 시작 시 한 번 호출되는 함수
	virtual void BeginPlay() override;
	/** </AActor> */

	/** <ABaseCharacter> */
	// 캐릭터가 사망했을 때 호출되는 함수
	virtual void Die_Implementation() override;

	// 영혼 생성 함수
	void SpawnSoul();

	// 공격 시작 함수
	virtual void Attack() override;

	// 공격 가능 여부를 판단하는 함수
	virtual bool CanAttack() override;

	// 공격 종료 함수
	virtual void AttackEnd() override;

	// 데미지 처리 함수, DamageAmount은 처리할 데미지 양
	virtual void HandleDamage(float DamageAmount) override;
	/** </ABaseCharacter> */

	// 적의 상태를 나타내는 변수, 기본적으로 순찰 상태로 설정
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		EEnemyState EnemyState = EENemyState::EES_Patrolling;


private:

	/** AI Behavior */

	// 적 AI 초기화 함수
	void InitializeEnemy();

	// 순찰 목표 검사 함수
	void CheckPatrolTarget();

	// 전투 목표 검사 함수
	void CheckCombatTarget();

	// 순찰 타이머 종료 함수
	void PatrolTimerFinished();

	// 체력바 숨기기 함수
	void HideHealthBar();

	// 체력바 보이기 함수
	void ShowHealthBar();

	// 적이 목표에 대한 관심을 잃을 때 호출하는 함수
	void LoseInterest();

	// 순찰 시작 함수
	void StartPatrolling();

	// 목표 추격 시작 함수
	void ChaseTarget();

	// 목표가 전투 범위 밖에 있는지 확인하는 함수
	bool IsOutsideCombatRadius();

	// 목표가 공격 범위 밖에 있는지 확인하는 함수
	bool IsOutsideAttackRadius();

	// 목표가 공격 범위 안에 있는지 확인하는 함수
	bool IsInsideAttackRadius();

	// 현재 추격 중인지 확인하는 함수
	bool IsChasing();

	// 현재 공격 중인지 확인하는 함수
	bool IsAttacking();

	// 현재 죽은 상태인지 확인하는 함수
	bool IsDead();

	// 현재 목표와 싸우고 있는지 확인하는 함수
	bool IsEngaged();

	// 순찰 타이머를 정지시키는 함수
	void ClearPatrolTimer();

	// 공격 타이머를 시작하는 함수
	void StartAttackTimer();

	// 공격 타이머를 정지시키는 함수
	void ClearAttackTimer();

	// 목표가 특정 반경 내에 있는지 확인하는 함수
	// Target : 목표 액터, Radius : 반경
	bool InTargetRange(AActor* Target, double Radius);

	// 목표에게 이동하는 함수
	// Target : 목표 액터
	void MoveToTarget(AActor* Target);

	// 순찰 목표 선택 함수
	AActor* ChoosePatrolTarget();

	// 기본 무기 생성 함수
	void SpawnDefaultWeapon();


	UFUNCTION()
		// 포착한 적을 처리하는 함수
		// SeenPawn : 포착한 적
		void PawnSeen(APawn* SeenPawn); // Callback for OnPawnSeen in UPawnSensingComponent

	UPROPERTY(VisibleAnywhere)
		// 체력바 위젯
		UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
		// Pawn 감지 컴포넌트
		UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
		// 사용할 무기 클래스
		TSubclassOf<class AWeapon> WeaponClass;

	

	UPROPERTY(EditAnywhere, Category = Combat)
		double CombatRadius = 500.f;

	UPROPERTY(EditAnywhere, Category = Combat)
		double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = Combat)
		double AcceptanceRadius = 50.f;

	UPROPERTY()
		class AAIController* EnemyController;

	// 적 AI의 현재 순찰 목표
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		AActor* PatrolTarget;

	// 순찰할 목표의 배열
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		TArray<AActor*> PatrolTargets;

	// 순찰 반경
	UPROPERTY(EditAnywhere)
		double PatrolRadius = 200.f;

	// 순찰 타이머 핸들
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

	// 적이 죽은 후 사라지는 데 걸리는 시간
	UPROPERTY(EditAnywhere, Category = Combat)
		float DeathLifeSpan = 8.f;

	// 생성될 영혼 클래스
	UPROPERTY(EditAnywhere, Category = Combat)
		TSubclassOf<class ASoul> SoulClass;
};