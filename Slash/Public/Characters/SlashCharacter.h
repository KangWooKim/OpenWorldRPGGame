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
	// 기본 생성자입니다.
	ASlashCharacter();

	// 프레임마다 호출되는 함수입니다. DeltaTime은 이전 프레임 이후의 시간을 나타냅니다.
	virtual void Tick(float DeltaTime) override;

	// 플레이어 입력 구성을 설정하는 함수입니다. PlayerInputComponent는 플레이어 입력을 처리하는 컴포넌트입니다.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 캐릭터가 점프하는 함수입니다.
	virtual void Jump() override;

	// 캐릭터가 데미지를 받는 함수입니다. DamageAmount는 데미지 양을, DamageEvent는 데미지 이벤트를, 
	// EventInstigator는 이벤트를 발생시킨 컨트롤러를, DamageCauser는 데미지를 준 액터를 나타냅니다.
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// 캐릭터가 히트를 받는 함수입니다. ImpactPoint는 충격 지점을, Hitter는 히트를 준 액터를 나타냅니다.
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	// 아이템과 겹치는 상태를 설정하는 함수입니다. Item은 겹치는 아이템을 나타냅니다.
	virtual void SetOverlappingItem(AItem* Item) override;

	// 영혼을 추가하는 함수입니다. Soul은 추가할 영혼을 나타냅니다.
	virtual void AddSouls(ASoul* Soul) override;

	// 골드를 추가하는 함수입니다. Treasure는 추가할 보물을 나타냅니다.
	virtual void AddGold(ATreasure* Treasure) override;

protected:
	// 게임 플레이가 시작될 때 호출되는 함수입니다.
	virtual void BeginPlay() override;

	/** 입력에 대한 콜백들입니다. */
	void MoveForward(float Value); // 앞으로 이동하는 함수입니다. Value는 이동량을 나타냅니다.
	void MoveRight(float Value); // 오른쪽으로 이동하는 함수입니다. Value는 이동량을 나타냅니다.
	void Turn(float Value); // 회전하는 함수입니다. Value는 회전량을 나타냅니다.
	void LookUp(float Value); // 위를 보는 함수입니다. Value는 보는 양을 나타냅니다.
	void EKeyPressed(); // E 키가 눌렸을 때의 함수입니다.
	virtual void Attack() override; // 공격하는 함수입니다.
	void Dodge(); // 회피하는 함수입니다.

	bool HasEnoughStamina(); // 스테미나가 충분한지를 판단하는 함수입니다.
	bool IsOccupied(); // 현재 Occupied 상태인지 판단하는 함수입니다.

	/** 전투 관련 함수들입니다. */
	void EquipWeapon(AWeapon* Weapon); // 무기를 장착하는 함수입니다. Weapon은 장착할 무기를 나타냅니다.
	virtual void AttackEnd() override; // 공격이 끝났을 때의 함수입니다.
	virtual void DodgeEnd() override; // 회피가 끝났을 때의 함수입니다.
	virtual bool CanAttack() override; // 공격할 수 있는지를 판단하는 함수입니다.
	bool CanDisarm(); // 무장해제할 수 있는지 판단하는 함수입니다.
	bool CanArm(); // 무장할 수 있는지 판단하는 함수입니다.
	void Disarm(); // 무장해제하는 함수입니다.
	void Arm(); // 무장하는 함수입니다.
	void PlayEquipMontage(const FName& SectionName); // 무기 장착 애니메이션을 재생하는 함수입니다. SectionName은 재생할 섹션 이름을 나타냅니다.
	virtual void Die_Implementation() override; // 캐릭터가 사망하는 함수입니다.

	UFUNCTION(BlueprintCallable)
		void AttachWeaponToBack(); // 무기를 등에 부착하는 함수입니다.

	UFUNCTION(BlueprintCallable)
		void AttachWeaponToHand(); // 무기를 손에 부착하는 함수입니다.

	UFUNCTION(BlueprintCallable)
		void FinishEquipping(); // 무기 장착을 완료하는 함수입니다.

	UFUNCTION(BlueprintCallable)
		void HitReactEnd(); // 히트 반응이 끝났을 때의 함수입니다.

private:

	// 캐릭터가 비어 있는지 확인하는 함수입니다.
	bool IsUnoccupied();

	// SlashOverlay를 초기화하는 함수입니다.
	void InitializeSlashOverlay();

	// HUD의 건강 상태를 설정하는 함수입니다.
	void SetHUDHealth();

	/** 캐릭터 컴포넌트들 */

	// 카메라 붐에 대한 속성입니다. 이는 게임 엔진에서 카메라와 캐릭터 사이의 거리를 조절합니다.
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* CameraBoom;

	// 캐릭터가 보는 카메라에 대한 속성입니다.
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ViewCamera;

	// 캐릭터와 겹치는 아이템에 대한 속성입니다.
	UPROPERTY(VisibleInstanceOnly)
		AItem* OverlappingItem;

	// 무기를 장착할 때 재생되는 애니메이션 몽타주에 대한 속성입니다.
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		UAnimMontage* EquipMontage;

	// 캐릭터의 상태를 나타내는 속성입니다. 초기 값은 '무장하지 않음'입니다.
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	// 캐릭터의 행동 상태를 나타내는 속성입니다. 초기 값은 '바쁘지 않음'입니다.
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EActionState ActionState = EActionState::EAS_Unoccupied;

	// SlashOverlay에 대한 속성입니다.
	UPROPERTY()
		USlashOverlay* SlashOverlay;

public:
	// 캐릭터 상태를 반환하는 인라인 함수입니다.
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	// 행동 상태를 반환하는 인라인 함수입니다.
	FORCEINLINE EActionState GetActionState() const { return ActionState; }

};