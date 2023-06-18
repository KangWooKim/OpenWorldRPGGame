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
	// �� ������Ʈ�� �Ӽ��� ���� �⺻�� ����
	UAttributeComponent();
	// �� �����Ӹ��� ȣ��Ǵ� �޼���
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// ���׹̳��� ȸ���ϴ� �޼���, DeltaTime(�ð� ����)�� ���ڷ� ���ϴ�.
	void RegenStamina(float DeltaTime);

protected:
	// ���� ���۽� ȣ��Ǵ� �޼���
	virtual void BeginPlay() override;

private:
	// ���� Health(ü��)
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		float Health;

	// �ִ� Health(ü��)
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		float MaxHealth;

	// ���� Stamina(���׹̳�)
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		float Stamina;

	// �ִ� Stamina(���׹̳�)
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		float MaxStamina;

	// Gold(���)
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		int32 Gold;

	// Souls(��ȥ)
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		int32 Souls;

	// Dodge(ȸ��)�� ����ϴµ� �ʿ��� Stamina(���׹̳�) ���
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		float DodgeCost = 14.f;

	// ���׹̳� ȸ�� �ӵ�
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
		float StaminaRegenRate = 8.f;

public:
	// �������� �޴� �޼���, ������ ���� ���ڷ� ���ϴ�.
	void ReceiveDamage(float Damage);
	// ���׹̳��� ����ϴ� �޼���, ���׹̳� ��뷮�� ���ڷ� ���ϴ�.
	void UseStamina(float StaminaCost);
	// ü�� ������ ��ȯ�ϴ� �޼���
	float GetHealthPercent();
	// ���׹̳� ������ ��ȯ�ϴ� �޼���
	float GetStaminaPercent();
	// ���� ���θ� ��ȯ�ϴ� �޼���
	bool IsAlive();
	// ��ȥ�� �߰��ϴ� �޼���, �߰��� ��ȥ�� ���� ���ڷ� ���ϴ�.
	void AddSouls(int32 NumberOfSouls);
	// ��带 �߰��ϴ� �޼���, �߰��� ����� ���� ���ڷ� ���ϴ�.
	void AddGold(int AmountOfGold);
	// ���� ��� ���� ��ȯ�ϴ� �޼���
	FORCEINLINE int32 GetGold() const { return Gold; }
	// ���� ��ȥ ���� ��ȯ�ϴ� �޼���
	FORCEINLINE int32 GetSouls() const { return Souls; }
	// Dodge(ȸ��)�� ����ϴµ� �ʿ��� ���׹̳� ����� ��ȯ�ϴ� �޼���
	FORCEINLINE float GetDodgeCost() const { return DodgeCost; }
	// ���� ���׹̳� ���� ��ȯ�ϴ� �޼���
	FORCEINLINE float GetStamina() const { return Stamina; }

};
