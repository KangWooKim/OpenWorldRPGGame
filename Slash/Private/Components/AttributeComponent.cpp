// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

// Sets default values for this component's properties
// 속성 컴포넌트 생성자
UAttributeComponent::UAttributeComponent()
{
    // 이 컴포넌트는 게임 시작시 초기화되며 매 프레임마다 tick이 발생합니다. 이 기능들은 필요하지 않으면 끄면 성능이 향상됩니다.
    PrimaryComponentTick.bCanEverTick = false;
}

// 게임 시작시 호출되는 메서드
void UAttributeComponent::BeginPlay()
{
    Super::BeginPlay();
    // ...
}

// 데미지를 받았을 때 호출되는 메서드, 인자로는 받은 데미지 양입니다.
void UAttributeComponent::ReceiveDamage(float Damage)
{
    // 건강 수치를 감소시키고, 0과 최대 건강 수치 사이로 제한합니다.
    Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

// 스테미나를 사용할 때 호출되는 메서드, 인자로는 사용한 스테미나 양입니다.
void UAttributeComponent::UseStamina(float StaminaCost)
{
    // 스테미나를 감소시키고, 0과 최대 스테미나 값 사이로 제한합니다.
    Stamina = FMath::Clamp(Stamina - StaminaCost, 0.f, MaxStamina);
}

// 건강 수치의 비율을 반환하는 메서드
float UAttributeComponent::GetHealthPercent()
{
    return Health / MaxHealth;
}

// 스테미나의 비율을 반환하는 메서드
float UAttributeComponent::GetStaminaPercent()
{
    return Stamina / MaxStamina;
}

// 생존 여부를 판단하는 메서드
bool UAttributeComponent::IsAlive()
{
    return Health > 0.f;
}

// 영혼을 추가하는 메서드, 인자로는 추가할 영혼의 수입니다.
void UAttributeComponent::AddSouls(int32 NumberOfSouls)
{
    Souls += NumberOfSouls;
}

// 골드를 추가하는 메서드, 인자로는 추가할 골드의 양입니다.
void UAttributeComponent::AddGold(int AmountOfGold)
{
    Gold += AmountOfGold;
}

// 매 프레임마다 호출되는 메서드
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // ...
}

// 스테미나를 회복하는 메서드, 인자로는 시간 간격입니다.
void UAttributeComponent::RegenStamina(float DeltaTime)
{
    // 스테미나를 회복하고, 0과 최대 스테미나 값 사이로 제한합니다.
    Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}


