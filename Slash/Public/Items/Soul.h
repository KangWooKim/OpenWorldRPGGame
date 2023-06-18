// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Soul.generated.h"

/**
 * 
 */
UCLASS()
class SLASH_API ASoul : public AItem
{
	GENERATED_BODY()

public:
    // �� �����Ӹ��� ȣ��Ǵ� �Լ�
    virtual void Tick(float DeltaTime) override; // DeltaTime: ���� �����Ӱ��� �ð� ��

protected:
    // ���� �����̳� ������ �� ȣ��Ǵ� �Լ�
    virtual void BeginPlay() override;

    // ������ �̺�Ʈ �߻� �� ȣ��Ǵ� �Լ�
    virtual void OnSphereOverlap(
        UPrimitiveComponent* OverlappedComponent,  // �������� ������Ʈ
        AActor* OtherActor,  // �������� �ٸ� ����
        UPrimitiveComponent* OtherComp,  // �������� �ٸ� ������Ʈ
        int32 OtherBodyIndex,  // �������� �ٸ� �ٵ� �ε���
        bool bFromSweep,  // �������κ����� ������ ����
        const FHitResult& SweepResult  // ���� ���
    ) override;

private:

    // �����Ϳ��� ���� ������ ��ȥ�� ��
    UPROPERTY(EditAnywhere, Category = "Soul Properties")
        int32 Souls;  // ��ȥ�� ��

    // ���Ͱ� �̵��ϰ��� �ϴ� Z ��ġ
    float DesiredZ;

    // �����Ϳ��� ���� ������ �帮��Ʈ �ӵ�
    UPROPERTY(EditAnywhere)
        double DriftRate = -15.f;  // �帮��Ʈ �ӵ�

public:
    // ��ȥ�� ���� �������� �Լ�
    FORCEINLINE int32 GetSouls() const { return Souls; }

    // ��ȥ�� ���� �����ϴ� �Լ�
    FORCEINLINE void SetSouls(int32 NumberOfSouls) { Souls = NumberOfSouls; }  // NumberOfSouls: ������ ��ȥ�� ��

};
