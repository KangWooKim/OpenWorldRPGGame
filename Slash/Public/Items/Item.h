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
	// 이 액터의 프로퍼티를 위한 기본 값 설정
	AItem();

protected:
	// 게임 시작이나 생성될 때 호출됨
	virtual void BeginPlay() override;

	// 사인 파라미터 - 진폭
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
		float Amplitude = 0.25f;

	// 사인 파라미터 - 시간 상수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
		float TimeConstant = 5.f;

	// 변환된 사인값을 반환하는 함수
	UFUNCTION(BlueprintPure)
		float TransformedSin();

	// 변환된 코사인값을 반환하는 함수
	UFUNCTION(BlueprintPure)
		float TransformedCos();

	// 두 인자의 평균값을 계산하는 템플릿 함수
	template<typename T>
	T Avg(T First, T Second);  // First: 첫 번째 값, Second: 두 번째 값

	// 구 영역과 겹치기 시작할 때 호출되는 함수
	UFUNCTION()
		virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 구 영역과 겹치기가 끝날 때 호출되는 함수
	UFUNCTION()
		virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 픽업 시스템을 생성하는 가상 함수
	virtual void SpawnPickupSystem();

	// 픽업 사운드를 생성하는 가상 함수
	virtual void SpawnPickupSound();

	// 아이템 메쉬 컴포넌트
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* ItemMesh;

	// 아이템 상태 (기본값은 호버링)
	EItemState ItemState = EItemState::EIS_Hovering;

	// 구 형태의 컴포넌트
	UPROPERTY(VisibleAnywhere)
		USphereComponent* Sphere;

	// 아이템 효과
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* ItemEffect;

	// 픽업 사운드
	UPROPERTY(EditAnywhere)
		USoundBase* PickupSound;



public:
	// 매 프레임마다 호출됨
	virtual void Tick(float DeltaTime) override;  // DeltaTime: 이전 프레임과의 시간 차

private:
	// 실행 시간 (Private 접근 허용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float RunningTime;

	// 픽업 효과
	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* PickupEffect;

};

// 두 인자의 평균값을 계산하는 템플릿 함수의 구현
template<typename T>
inline T AItem::Avg(T First, T Second)  // First: 첫 번째 값, Second: 두 번째 값
{
	return (First + Second) / 2;
}

