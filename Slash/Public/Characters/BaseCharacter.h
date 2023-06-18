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
	// �⺻ ������
	ABaseCharacter();

	// Tick �Լ� �������̵�
	virtual void Tick(float DeltaTime) override;

protected:
	// BeginPlay �Լ� �������̵�
	virtual void BeginPlay() override;

	// Combat
	// �ǰ� ó�� �Լ�
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	// ���� �Լ�
	virtual void Attack();

	// ���� ó�� �Լ� (�������Ʈ ����Ƽ�� �̺�Ʈ)
	UFUNCTION(BlueprintNativeEvent)
		void Die();

	// �ǰ� ���� ó�� �Լ�
	void DirectionalHitReact(const FVector& ImpactPoint);

	// ������ ó�� �Լ�
	virtual void HandleDamage(float DamageAmount);

	// �ǰ� ���� ��� �Լ�
	void PlayHitSound(const FVector& ImpactPoint);

	// �ǰ� ��ƼŬ ���� �Լ�
	void SpawnHitParticles(const FVector& ImpactPoint);

	// ĸ�� ��Ȱ��ȭ �Լ�
	void DisableCapsule();

	// ���� ���� ���� Ȯ�� �Լ�
	virtual bool CanAttack();

	// ����ִ� ���� Ȯ�� �Լ�
	bool IsAlive();

	// �޽� �浹 ��Ȱ��ȭ �Լ�
	void DisableMeshCollision();

	//Montage
	// �ǰ� ���� ��Ÿ�� ��� �Լ�
	void PlayHitReactMontage(const FName& SectionName);

	// ���� ��Ÿ�� ��� �Լ�
	virtual int32 PlayAttackMontage();

	// ���� ��Ÿ�� ��� �Լ�
	virtual int32 PlayDeathMontage();

	// ȸ�� ��Ÿ�� ��� �Լ�
	virtual void PlayDodgeMontage();

	// ���� ��Ÿ�� ���� �Լ�
	void StopAttackMontage();

	// �̵� ���� ��ǥ ��ġ ��� �Լ� (�������Ʈ���� ȣ�� ����)
	UFUNCTION(BlueprintCallable)
		FVector GetTranslationWarpTarget();

	// ȸ�� ���� ��ǥ ��ġ ��� �Լ� (�������Ʈ���� ȣ�� ����)
	UFUNCTION(BlueprintCallable)
		FVector GetRotationWarpTarget();

	// ���� ���� ó�� �Լ� (�������Ʈ���� ȣ�� ����)
	UFUNCTION(BlueprintCallable)
		virtual void AttackEnd();

	// ȸ�� ���� ó�� �Լ� (�������Ʈ���� ȣ�� ����)
	UFUNCTION(BlueprintCallable)
		virtual void DodgeEnd();

	// ���� �浹 ���� ���� �Լ� (�������Ʈ���� ȣ�� ����)
	UFUNCTION(BlueprintCallable)
		void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	// ���� ���� (����â���� Ȯ�� ����)
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		AWeapon* EquippedWeapon;

	// �Ӽ� ������Ʈ (����â���� Ȯ�� ����)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UAttributeComponent* Attributes;

	// ���� ��� (����â���� Ȯ�� ����)
	UPROPERTY(BlueprintReadOnly, Category = Combat)
		AActor* CombatTarget;

	// ���� �������� �Ÿ� (���� ����)
	UPROPERTY(EditAnywhere, Category = Combat)
		double WarpTargetDistance = 75.f;

	// ���� ���� (����â���� Ȯ�� ����)
	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte<EDeathPose> DeathPose;


private:
	// ��Ÿ�� ���� ��� �Լ�
	// @param Montage: ����� ��Ÿ��
	// @param SectionName: ����� ���� �̸�
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

	// ������ ��Ÿ�� ���� ��� �Լ�
	// @param Montage: ����� ��Ÿ��
	// @param SectionNames: ����� ���� �̸� �迭
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	// �ǰ� ���� (���� ����)
	UPROPERTY(EditAnywhere, Category = Combat)
		USoundBase* HitSound;

	// �ǰ� ��ƼŬ (���� ����)
	UPROPERTY(EditAnywhere, Category = Combat)
		UParticleSystem* HitParticles;

	// ���� ��Ÿ�� (�⺻ �� ���� ����)
	UPROPERTY(EditDefaultsOnly, Category = Combat)
		UAnimMontage* AttackMontage;

	// �ǰ� ���� ��Ÿ�� (�⺻ �� ���� ����)
	UPROPERTY(EditDefaultsOnly, Category = Combat)
		UAnimMontage* HitReactMontage;

	// ���� ��Ÿ�� (�⺻ �� ���� ����)
	UPROPERTY(EditDefaultsOnly, Category = Combat)
		UAnimMontage* DeathMontage;

	// ȸ�� ��Ÿ�� (�⺻ �� ���� ����)
	UPROPERTY(EditDefaultsOnly, Category = Combat)
		UAnimMontage* DodgeMontage;

	// ���� ��Ÿ�� ���� �̸� �迭 (���� ����)
	UPROPERTY(EditAnywhere, Category = Combat)
		TArray<FName> AttackMontageSections;

	// ���� ��Ÿ�� ���� �̸� �迭 (���� ����)
	UPROPERTY(EditAnywhere, Category = Combat)
		TArray<FName> DeathMontageSections;

public:
	// ���� ���� getter �Լ�
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }

};