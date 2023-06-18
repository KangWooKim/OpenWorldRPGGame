// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;

/**
 *
 */
UCLASS()
class SLASH_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
    // ���� Ŭ������ �⺻ �������Դϴ�.
    AWeapon();

    // ���⸦ �����ϴ� �Լ��Դϴ�.
    void Equip(
        USceneComponent* InParent,  // ������ �θ� ������Ʈ�Դϴ�.
        FName InSocketName,  // ������ ������ �̸��Դϴ�.
        AActor* NewOwner,  // ������ ���ο� �������Դϴ�.
        APawn* NewInstigator  // ������ ���ο� Initiator�Դϴ�.
    );

    // �Ҿ��� ��Ȱ��ȭ�ϴ� �Լ��Դϴ�.
    void DeactivateEmbers();

    // ��ü �ݸ����� ��Ȱ��ȭ�ϴ� �Լ��Դϴ�.
    void DisableSphereCollision();

    // ���� �Ҹ��� ����ϴ� �Լ��Դϴ�.
    void PlayEquipSound();

    // �޽ø� ���Ͽ� �����ϴ� �Լ��Դϴ�.
    void AttachMeshToSocket(
        USceneComponent* InParent,  // ������ �θ� ������Ʈ�Դϴ�.
        const FName& InSocketName  // ������ ������ �̸��Դϴ�.
    );

    // �����ؾ��ϴ� ���͵��� ����Դϴ�.
    TArray<AActor*> IgnoreActors;

protected:
    // ���� �÷��̰� ���۵� �� ȣ��Ǵ� �Լ��Դϴ�.
    virtual void BeginPlay() override;

    // ������ �̺�Ʈ�� �߻��� �� ȣ��Ǵ� �Լ��Դϴ�.
    UFUNCTION()
        void OnBoxOverlap(
            UPrimitiveComponent* OverlappedComponent,  // �������� ������Ʈ�Դϴ�.
            AActor* OtherActor,  // �������� �ٸ� �����Դϴ�.
            UPrimitiveComponent* OtherComp,  // �������� �ٸ� ������Ʈ�Դϴ�.
            int32 OtherBodyIndex,  // �������� �ٸ� �ٵ��� �ε����Դϴ�.
            bool bFromSweep,  // �������κ����� ������ �����Դϴ�.
            const FHitResult& SweepResult  // ������ ����Դϴ�.
        );

    // �־��� ���Ͱ� ���� Ÿ������ Ȯ���ϴ� �Լ��Դϴ�.
    bool ActorIsSameType(AActor* OtherActor);

    // ��Ʈ �̺�Ʈ�� �����ϴ� �Լ��Դϴ�.
    void ExecuteGetHit(FHitResult& BoxHit);

    // �ʵ带 �����ϴ� �������Ʈ ���� ���� �̺�Ʈ�Դϴ�.
    UFUNCTION(BlueprintImplementableEvent)
        void CreateFields(const FVector& FieldLocation);

private:
    // �ڽ� Ʈ���̽��� �����ϴ� �Լ��Դϴ�.
    void BoxTrace(FHitResult& BoxHit);

    // �ڽ� Ʈ���̽��� �����Դϴ�.
    UPROPERTY(EditAnywhere, Category = "Weapon Properties")
        FVector BoxTraceExtent = FVector(5.f);

    // �ڽ� Ʈ���̽� ����׸� ǥ������ �����Դϴ�.
    UPROPERTY(EditAnywhere, Category = "Weapon Properties")
        bool bShowBoxDebug = false;

    // ���� �Ҹ��Դϴ�.
    UPROPERTY(EditAnywhere, Category = "Weapon Properties")
        USoundBase* EquipSound;

    // ������ �ڽ� ������Ʈ�Դϴ�.
    UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
        UBoxComponent* WeaponBox;

    // �ڽ� Ʈ���̽��� �������Դϴ�.
    UPROPERTY(VisibleAnywhere)
        USceneComponent* BoxTraceStart;

    // �ڽ� Ʈ���̽��� �����Դϴ�.
    UPROPERTY(VisibleAnywhere)
        USceneComponent* BoxTraceEnd;

    // ������ �������Դϴ�.
    UPROPERTY(EditAnywhere, Category = "Weapon Properties")
        float Damage = 20.f;

public:
    // ���� �ڽ��� �������� �ζ��� �Լ��Դϴ�.
    FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }

};