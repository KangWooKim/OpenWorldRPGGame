// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class UNiagaraComponent;

enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped
};

UCLASS()
class SLASH_API AItem : public AActor
{
	GENERATED_BODY()
	
public:
	// �� ������ ������Ƽ�� ���� �⺻ �� ����
	AItem();

protected:
	// ���� �����̳� ������ �� ȣ���
	virtual void BeginPlay() override;

	// ���� �Ķ���� - ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
		float Amplitude = 0.25f;

	// ���� �Ķ���� - �ð� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
		float TimeConstant = 5.f;

	// ��ȯ�� ���ΰ��� ��ȯ�ϴ� �Լ�
	UFUNCTION(BlueprintPure)
		float TransformedSin();

	// ��ȯ�� �ڻ��ΰ��� ��ȯ�ϴ� �Լ�
	UFUNCTION(BlueprintPure)
		float TransformedCos();

	// �� ������ ��հ��� ����ϴ� ���ø� �Լ�
	template<typename T>
	T Avg(T First, T Second);  // First: ù ��° ��, Second: �� ��° ��

	// �� ������ ��ġ�� ������ �� ȣ��Ǵ� �Լ�
	UFUNCTION()
		virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// �� ������ ��ġ�Ⱑ ���� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
		virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// �Ⱦ� �ý����� �����ϴ� ���� �Լ�
	virtual void SpawnPickupSystem();

	// �Ⱦ� ���带 �����ϴ� ���� �Լ�
	virtual void SpawnPickupSound();

	// ������ �޽� ������Ʈ
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* ItemMesh;

	// ������ ���� (�⺻���� ȣ����)
	EItemState ItemState = EItemState::EIS_Hovering;

	// �� ������ ������Ʈ
	UPROPERTY(VisibleAnywhere)
		USphereComponent* Sphere;

	// ������ ȿ��
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* ItemEffect;

	// �Ⱦ� ����
	UPROPERTY(EditAnywhere)
		USoundBase* PickupSound;



public:
	// �� �����Ӹ��� ȣ���
	virtual void Tick(float DeltaTime) override;  // DeltaTime: ���� �����Ӱ��� �ð� ��

private:
	// ���� �ð� (Private ���� ���)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float RunningTime;

	// �Ⱦ� ȿ��
	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* PickupEffect;

};

// �� ������ ��հ��� ����ϴ� ���ø� �Լ��� ����
template<typename T>
inline T AItem::Avg(T First, T Second)  // First: ù ��° ��, Second: �� ��° ��
{
	return (First + Second) / 2;
}

