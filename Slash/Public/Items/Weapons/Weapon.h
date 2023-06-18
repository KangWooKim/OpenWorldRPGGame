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
    // 무기 클래스의 기본 생성자입니다.
    AWeapon();

    // 무기를 장착하는 함수입니다.
    void Equip(
        USceneComponent* InParent,  // 장착될 부모 컴포넌트입니다.
        FName InSocketName,  // 장착될 소켓의 이름입니다.
        AActor* NewOwner,  // 무기의 새로운 소유자입니다.
        APawn* NewInstigator  // 무기의 새로운 Initiator입니다.
    );

    // 불씨를 비활성화하는 함수입니다.
    void DeactivateEmbers();

    // 구체 콜리전을 비활성화하는 함수입니다.
    void DisableSphereCollision();

    // 장착 소리를 재생하는 함수입니다.
    void PlayEquipSound();

    // 메시를 소켓에 부착하는 함수입니다.
    void AttachMeshToSocket(
        USceneComponent* InParent,  // 부착될 부모 컴포넌트입니다.
        const FName& InSocketName  // 부착될 소켓의 이름입니다.
    );

    // 무시해야하는 액터들의 목록입니다.
    TArray<AActor*> IgnoreActors;

protected:
    // 게임 플레이가 시작될 때 호출되는 함수입니다.
    virtual void BeginPlay() override;

    // 오버랩 이벤트가 발생할 때 호출되는 함수입니다.
    UFUNCTION()
        void OnBoxOverlap(
            UPrimitiveComponent* OverlappedComponent,  // 오버랩된 컴포넌트입니다.
            AActor* OtherActor,  // 오버랩된 다른 액터입니다.
            UPrimitiveComponent* OtherComp,  // 오버랩된 다른 컴포넌트입니다.
            int32 OtherBodyIndex,  // 오버랩된 다른 바디의 인덱스입니다.
            bool bFromSweep,  // 스윕으로부터의 오버랩 여부입니다.
            const FHitResult& SweepResult  // 스윕의 결과입니다.
        );

    // 주어진 액터가 같은 타입인지 확인하는 함수입니다.
    bool ActorIsSameType(AActor* OtherActor);

    // 히트 이벤트를 실행하는 함수입니다.
    void ExecuteGetHit(FHitResult& BoxHit);

    // 필드를 생성하는 블루프린트 구현 가능 이벤트입니다.
    UFUNCTION(BlueprintImplementableEvent)
        void CreateFields(const FVector& FieldLocation);

private:
    // 박스 트레이스를 수행하는 함수입니다.
    void BoxTrace(FHitResult& BoxHit);

    // 박스 트레이스의 범위입니다.
    UPROPERTY(EditAnywhere, Category = "Weapon Properties")
        FVector BoxTraceExtent = FVector(5.f);

    // 박스 트레이스 디버그를 표시할지 여부입니다.
    UPROPERTY(EditAnywhere, Category = "Weapon Properties")
        bool bShowBoxDebug = false;

    // 장착 소리입니다.
    UPROPERTY(EditAnywhere, Category = "Weapon Properties")
        USoundBase* EquipSound;

    // 무기의 박스 컴포넌트입니다.
    UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
        UBoxComponent* WeaponBox;

    // 박스 트레이스의 시작점입니다.
    UPROPERTY(VisibleAnywhere)
        USceneComponent* BoxTraceStart;

    // 박스 트레이스의 끝점입니다.
    UPROPERTY(VisibleAnywhere)
        USceneComponent* BoxTraceEnd;

    // 무기의 데미지입니다.
    UPROPERTY(EditAnywhere, Category = "Weapon Properties")
        float Damage = 20.f;

public:
    // 무기 박스를 가져오는 인라인 함수입니다.
    FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }

};