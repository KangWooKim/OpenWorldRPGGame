// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SLASH_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// 이 컴포넌트의 속성에 대한 기본값 설정
	UAttributeComponent();
	// 매 프레임마다 호출되는 메서드
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// 스테미나를 회복하는 메서드, DeltaTime(시간 간격)이 인자로 들어갑니다.
	void RegenStamina(float DeltaTime);

protected:
	// 게임 시작시 호출되는 메서드
	virtual void BeginPlay() override;

private:
	// 현재 Health(체력)
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		float Health;

	// 최대 Health(체력)
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		float MaxHealth;

	// 현재 Stamina(스테미나)
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		float Stamina;

	// 최대 Stamina(스테미나)
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		float MaxStamina;

	// Gold(골드)
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		int32 Gold;

	// Souls(영혼)
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		int32 Souls;

	// Dodge(회피)를 사용하는데 필요한 Stamina(스테미나) 비용
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		float DodgeCost = 14.f;

	// 스테미나 회복 속도
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		float StaminaRegenRate = 8.f;

public:
	// 데미지를 받는 메서드, 데미지 양이 인자로 들어갑니다.
	void ReceiveDamage(float Damage);
	// 스테미나를 사용하는 메서드, 스테미나 사용량이 인자로 들어갑니다.
	void UseStamina(float StaminaCost);
	// 체력 비율을 반환하는 메서드
	float GetHealthPercent();
	// 스테미나 비율을 반환하는 메서드
	float GetStaminaPercent();
	// 생존 여부를 반환하는 메서드
	bool IsAlive();
	// 영혼을 추가하는 메서드, 추가할 영혼의 수가 인자로 들어갑니다.
	void AddSouls(int32 NumberOfSouls);
	// 골드를 추가하는 메서드, 추가할 골드의 양이 인자로 들어갑니다.
	void AddGold(int AmountOfGold);
	// 현재 골드 양을 반환하는 메서드
	FORCEINLINE int32 GetGold() const { return Gold; }
	// 현재 영혼 수를 반환하는 메서드
	FORCEINLINE int32 GetSouls() const { return Souls; }
	// Dodge(회피)를 사용하는데 필요한 스테미나 비용을 반환하는 메서드
	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }
	// 현재 스테미나 양을 반환하는 메서드
	FORCEINLINE float GetStamina() const { return Stamina; }

};
