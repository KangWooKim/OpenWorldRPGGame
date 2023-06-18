// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

// Sets default values for this component's properties
// �Ӽ� ������Ʈ ������
UAttributeComponent::UAttributeComponent()
{
    // �� ������Ʈ�� ���� ���۽� �ʱ�ȭ�Ǹ� �� �����Ӹ��� tick�� �߻��մϴ�. �� ��ɵ��� �ʿ����� ������ ���� ������ ���˴ϴ�.
    PrimaryComponentTick.bCanEverTick = false;
}

// ���� ���۽� ȣ��Ǵ� �޼���
void UAttributeComponent::BeginPlay()
{
    Super::BeginPlay();
    // ...
}

// �������� �޾��� �� ȣ��Ǵ� �޼���, ���ڷδ� ���� ������ ���Դϴ�.
void UAttributeComponent::ReceiveDamage(float Damage)
{
    // �ǰ� ��ġ�� ���ҽ�Ű��, 0�� �ִ� �ǰ� ��ġ ���̷� �����մϴ�.
    Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

// ���׹̳��� ����� �� ȣ��Ǵ� �޼���, ���ڷδ� ����� ���׹̳� ���Դϴ�.
void UAttributeComponent::UseStamina(float StaminaCost)
{
    // ���׹̳��� ���ҽ�Ű��, 0�� �ִ� ���׹̳� �� ���̷� �����մϴ�.
    Stamina = FMath::Clamp(Stamina - StaminaCost, 0.f, MaxStamina);
}

// �ǰ� ��ġ�� ������ ��ȯ�ϴ� �޼���
float UAttributeComponent::GetHealthPercent()
{
    return Health / MaxHealth;
}

// ���׹̳��� ������ ��ȯ�ϴ� �޼���
float UAttributeComponent::GetStaminaPercent()
{
    return Stamina / MaxStamina;
}

// ���� ���θ� �Ǵ��ϴ� �޼���
bool UAttributeComponent::IsAlive()
{
    return Health > 0.f;
}

// ��ȥ�� �߰��ϴ� �޼���, ���ڷδ� �߰��� ��ȥ�� ���Դϴ�.
void UAttributeComponent::AddSouls(int32 NumberOfSouls)
{
    Souls += NumberOfSouls;
}

// ��带 �߰��ϴ� �޼���, ���ڷδ� �߰��� ����� ���Դϴ�.
void UAttributeComponent::AddGold(int AmountOfGold)
{
    Gold += AmountOfGold;
}

// �� �����Ӹ��� ȣ��Ǵ� �޼���
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // ...
}

// ���׹̳��� ȸ���ϴ� �޼���, ���ڷδ� �ð� �����Դϴ�.
void UAttributeComponent::RegenStamina(float DeltaTime)
{
    // ���׹̳��� ȸ���ϰ�, 0�� �ִ� ���׹̳� �� ���̷� �����մϴ�.
    Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}


