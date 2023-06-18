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
    // 매 프레임마다 호출되는 함수
    virtual void Tick(float DeltaTime) override; // DeltaTime: 이전 프레임과의 시간 차

protected:
    // 게임 시작이나 스폰될 때 호출되는 함수
    virtual void BeginPlay() override;

    // 오버랩 이벤트 발생 시 호출되는 함수
    virtual void OnSphereOverlap(
        UPrimitiveComponent* OverlappedComponent,  // 오버랩된 컴포넌트
        AActor* OtherActor,  // 오버랩된 다른 액터
        UPrimitiveComponent* OtherComp,  // 오버랩된 다른 컴포넌트
        int32 OtherBodyIndex,  // 오버랩된 다른 바디 인덱스
        bool bFromSweep,  // 스윕으로부터의 오버랩 여부
        const FHitResult& SweepResult  // 스윕 결과
    ) override;

private:

    // 에디터에서 설정 가능한 영혼의 수
    UPROPERTY(EditAnywhere, Category = "Soul Properties")
        int32 Souls;  // 영혼의 수

    // 액터가 이동하고자 하는 Z 위치
    float DesiredZ;

    // 에디터에서 설정 가능한 드리프트 속도
    UPROPERTY(EditAnywhere)
        double DriftRate = -15.f;  // 드리프트 속도

public:
    // 영혼의 수를 가져오는 함수
    FORCEINLINE int32 GetSouls() const { return Souls; }

    // 영혼의 수를 설정하는 함수
    FORCEINLINE void SetSouls(int32 NumberOfSouls) { Souls = NumberOfSouls; }  // NumberOfSouls: 설정할 영혼의 수

};
