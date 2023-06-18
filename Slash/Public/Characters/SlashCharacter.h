// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "interfaces/PickupInterface.h"
#include "SlashCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AItem;
class ASoul;
class ATreasure;
class UAnimMontage;
class USlashOverlay;

UCLASS()
class SLASH_API ASlashCharacter : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	// �⺻ �������Դϴ�.
	ASlashCharacter();

	// �����Ӹ��� ȣ��Ǵ� �Լ��Դϴ�. DeltaTime�� ���� ������ ������ �ð��� ��Ÿ���ϴ�.
	virtual void Tick(float DeltaTime) override;

	// �÷��̾� �Է� ������ �����ϴ� �Լ��Դϴ�. PlayerInputComponent�� �÷��̾� �Է��� ó���ϴ� ������Ʈ�Դϴ�.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ĳ���Ͱ� �����ϴ� �Լ��Դϴ�.
	virtual void Jump() override;

	// ĳ���Ͱ� �������� �޴� �Լ��Դϴ�. DamageAmount�� ������ ����, DamageEvent�� ������ �̺�Ʈ��, 
	// EventInstigator�� �̺�Ʈ�� �߻���Ų ��Ʈ�ѷ���, DamageCauser�� �������� �� ���͸� ��Ÿ���ϴ�.
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// ĳ���Ͱ� ��Ʈ�� �޴� �Լ��Դϴ�. ImpactPoint�� ��� ������, Hitter�� ��Ʈ�� �� ���͸� ��Ÿ���ϴ�.
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	// �����۰� ��ġ�� ���¸� �����ϴ� �Լ��Դϴ�. Item�� ��ġ�� �������� ��Ÿ���ϴ�.
	virtual void SetOverlappingItem(AItem* Item) override;

	// ��ȥ�� �߰��ϴ� �Լ��Դϴ�. Soul�� �߰��� ��ȥ�� ��Ÿ���ϴ�.
	virtual void AddSouls(ASoul* Soul) override;

	// ��带 �߰��ϴ� �Լ��Դϴ�. Treasure�� �߰��� ������ ��Ÿ���ϴ�.
	virtual void AddGold(ATreasure* Treasure) override;

protected:
	// ���� �÷��̰� ���۵� �� ȣ��Ǵ� �Լ��Դϴ�.
	virtual void BeginPlay() override;

	/** �Է¿� ���� �ݹ���Դϴ�. */
	void MoveForward(float Value); // ������ �̵��ϴ� �Լ��Դϴ�. Value�� �̵����� ��Ÿ���ϴ�.
	void MoveRight(float Value); // ���������� �̵��ϴ� �Լ��Դϴ�. Value�� �̵����� ��Ÿ���ϴ�.
	void Turn(float Value); // ȸ���ϴ� �Լ��Դϴ�. Value�� ȸ������ ��Ÿ���ϴ�.
	void LookUp(float Value); // ���� ���� �Լ��Դϴ�. Value�� ���� ���� ��Ÿ���ϴ�.
	void EKeyPressed(); // E Ű�� ������ ���� �Լ��Դϴ�.
	virtual void Attack() override; // �����ϴ� �Լ��Դϴ�.
	void Dodge(); // ȸ���ϴ� �Լ��Դϴ�.

	bool HasEnoughStamina(); // ���׹̳��� ��������� �Ǵ��ϴ� �Լ��Դϴ�.
	bool IsOccupied(); // ���� Occupied �������� �Ǵ��ϴ� �Լ��Դϴ�.

	/** ���� ���� �Լ����Դϴ�. */
	void EquipWeapon(AWeapon* Weapon); // ���⸦ �����ϴ� �Լ��Դϴ�. Weapon�� ������ ���⸦ ��Ÿ���ϴ�.
	virtual void AttackEnd() override; // ������ ������ ���� �Լ��Դϴ�.
	virtual void DodgeEnd() override; // ȸ�ǰ� ������ ���� �Լ��Դϴ�.
	virtual bool CanAttack() override; // ������ �� �ִ����� �Ǵ��ϴ� �Լ��Դϴ�.
	bool CanDisarm(); // ���������� �� �ִ��� �Ǵ��ϴ� �Լ��Դϴ�.
	bool CanArm(); // ������ �� �ִ��� �Ǵ��ϴ� �Լ��Դϴ�.
	void Disarm(); // ���������ϴ� �Լ��Դϴ�.
	void Arm(); // �����ϴ� �Լ��Դϴ�.
	void PlayEquipMontage(const FName& SectionName); // ���� ���� �ִϸ��̼��� ����ϴ� �Լ��Դϴ�. SectionName�� ����� ���� �̸��� ��Ÿ���ϴ�.
	virtual void Die_Implementation() override; // ĳ���Ͱ� ����ϴ� �Լ��Դϴ�.

	UFUNCTION(BlueprintCallable)
		void AttachWeaponToBack(); // ���⸦ � �����ϴ� �Լ��Դϴ�.

	UFUNCTION(BlueprintCallable)
		void AttachWeaponToHand(); // ���⸦ �տ� �����ϴ� �Լ��Դϴ�.

	UFUNCTION(BlueprintCallable)
		void FinishEquipping(); // ���� ������ �Ϸ��ϴ� �Լ��Դϴ�.

	UFUNCTION(BlueprintCallable)
		void HitReactEnd(); // ��Ʈ ������ ������ ���� �Լ��Դϴ�.

private:

	// ĳ���Ͱ� ��� �ִ��� Ȯ���ϴ� �Լ��Դϴ�.
	bool IsUnoccupied();

	// SlashOverlay�� �ʱ�ȭ�ϴ� �Լ��Դϴ�.
	void InitializeSlashOverlay();

	// HUD�� �ǰ� ���¸� �����ϴ� �Լ��Դϴ�.
	void SetHUDHealth();

	/** ĳ���� ������Ʈ�� */

	// ī�޶� �տ� ���� �Ӽ��Դϴ�. �̴� ���� �������� ī�޶�� ĳ���� ������ �Ÿ��� �����մϴ�.
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* CameraBoom;

	// ĳ���Ͱ� ���� ī�޶� ���� �Ӽ��Դϴ�.
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ViewCamera;

	// ĳ���Ϳ� ��ġ�� �����ۿ� ���� �Ӽ��Դϴ�.
	UPROPERTY(VisibleInstanceOnly)
		AItem* OverlappingItem;

	// ���⸦ ������ �� ����Ǵ� �ִϸ��̼� ��Ÿ�ֿ� ���� �Ӽ��Դϴ�.
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		UAnimMontage* EquipMontage;

	// ĳ������ ���¸� ��Ÿ���� �Ӽ��Դϴ�. �ʱ� ���� '�������� ����'�Դϴ�.
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	// ĳ������ �ൿ ���¸� ��Ÿ���� �Ӽ��Դϴ�. �ʱ� ���� '�ٻ��� ����'�Դϴ�.
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EActionState ActionState = EActionState::EAS_Unoccupied;

	// SlashOverlay�� ���� �Ӽ��Դϴ�.
	UPROPERTY()
		USlashOverlay* SlashOverlay;

public:
	// ĳ���� ���¸� ��ȯ�ϴ� �ζ��� �Լ��Դϴ�.
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	// �ൿ ���¸� ��ȯ�ϴ� �ζ��� �Լ��Դϴ�.
	FORCEINLINE EActionState GetActionState() const { return ActionState; }

};