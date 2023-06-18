// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypes.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAttributeComponent;
class UAnimMontage;

UCLASS()
class SLASH_API ABaseCharacter : public ACharacter, public IHitinterface
{
	GENERATED_BODY()

public:
	// 기본 생성자
	ABaseCharacter();

	// Tick 함수 오버라이드
	virtual void Tick(float DeltaTime) override;

protected:
	// BeginPlay 함수 오버라이드
	virtual void BeginPlay() override;

	// Combat
	// 피격 처리 함수
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	// 공격 함수
	virtual void Attack();

	// 죽음 처리 함수 (블루프린트 네이티브 이벤트)
	UFUNCTION(BlueprintNativeEvent)
		void Die();

	// 피격 반응 처리 함수
	void DirectionalHitReact(const FVector& ImpactPoint);

	// 데미지 처리 함수
	virtual void HandleDamage(float DamageAmount);

	// 피격 사운드 재생 함수
	void PlayHitSound(const FVector& ImpactPoint);

	// 피격 파티클 생성 함수
	void SpawnHitParticles(const FVector& ImpactPoint);

	// 캡슐 비활성화 함수
	void DisableCapsule();

	// 공격 가능 상태 확인 함수
	virtual bool CanAttack();

	// 살아있는 상태 확인 함수
	bool IsAlive();

	// 메쉬 충돌 비활성화 함수
	void DisableMeshCollision();

	//Montage
	// 피격 반응 몽타주 재생 함수
	void PlayHitReactMontage(const FName& SectionName);

	// 공격 몽타주 재생 함수
	virtual int32 PlayAttackMontage();

	// 죽음 몽타주 재생 함수
	virtual int32 PlayDeathMontage();

	// 회피 몽타주 재생 함수
	virtual void PlayDodgeMontage();

	// 공격 몽타주 중지 함수
	void StopAttackMontage();

	// 이동 전송 목표 위치 계산 함수 (블루프린트에서 호출 가능)
	UFUNCTION(BlueprintCallable)
		FVector GetTranslationWarpTarget();

	// 회전 전송 목표 위치 계산 함수 (블루프린트에서 호출 가능)
	UFUNCTION(BlueprintCallable)
		FVector GetRotationWarpTarget();

	// 공격 종료 처리 함수 (블루프린트에서 호출 가능)
	UFUNCTION(BlueprintCallable)
		virtual void AttackEnd();

	// 회피 종료 처리 함수 (블루프린트에서 호출 가능)
	UFUNCTION(BlueprintCallable)
		virtual void DodgeEnd();

	// 무기 충돌 상태 설정 함수 (블루프린트에서 호출 가능)
	UFUNCTION(BlueprintCallable)
		void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	// 장착 무기 (편집창에서 확인 가능)
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		AWeapon* EquippedWeapon;

	// 속성 컴포넌트 (편집창에서 확인 가능)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UAttributeComponent* Attributes;

	// 전투 대상 (편집창에서 확인 가능)
	UPROPERTY(BlueprintReadOnly, Category = Combat)
		AActor* CombatTarget;

	// 전송 대상까지의 거리 (편집 가능)
	UPROPERTY(EditAnywhere, Category = Combat)
		double WarpTargetDistance = 75.f;

	// 죽음 포즈 (편집창에서 확인 가능)
	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte<EDeathPose> DeathPose;


private:
	// 몽타주 섹션 재생 함수
	// @param Montage: 재생할 몽타주
	// @param SectionName: 재생할 섹션 이름
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

	// 랜덤한 몽타주 섹션 재생 함수
	// @param Montage: 재생할 몽타주
	// @param SectionNames: 재생할 섹션 이름 배열
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	// 피격 사운드 (편집 가능)
	UPROPERTY(EditAnywhere, Category = Combat)
		USoundBase* HitSound;

	// 피격 파티클 (편집 가능)
	UPROPERTY(EditAnywhere, Category = Combat)
		UParticleSystem* HitParticles;

	// 공격 몽타주 (기본 값 편집 가능)
	UPROPERTY(EditDefaultsOnly, Category = Combat)
		UAnimMontage* AttackMontage;

	// 피격 반응 몽타주 (기본 값 편집 가능)
	UPROPERTY(EditDefaultsOnly, Category = Combat)
		UAnimMontage* HitReactMontage;

	// 죽음 몽타주 (기본 값 편집 가능)
	UPROPERTY(EditDefaultsOnly, Category = Combat)
		UAnimMontage* DeathMontage;

	// 회피 몽타주 (기본 값 편집 가능)
	UPROPERTY(EditDefaultsOnly, Category = Combat)
		UAnimMontage* DodgeMontage;

	// 공격 몽타주 섹션 이름 배열 (편집 가능)
	UPROPERTY(EditAnywhere, Category = Combat)
		TArray<FName> AttackMontageSections;

	// 죽음 몽타주 섹션 이름 배열 (편집 가능)
	UPROPERTY(EditAnywhere, Category = Combat)
		TArray<FName> DeathMontageSections;

public:
	// 죽음 포즈 getter 함수
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }

};